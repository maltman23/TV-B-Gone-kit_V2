"""Host checks; no connected board or IR emission required. CC BY-SA 4.0."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from convert_codes import generate


class Pin:
    OUT, IN, PULL_UP = 1, 0, 2

    def __init__(self, number, mode, pull=None, value=None):
        self.number = number
        self.init(mode, value=1 if value is None else value)

    def init(self, mode, value):
        self.level = value

    def value(self, value=None):
        if value is not None:
            self.level = value
        return self.level


class RMT:
    PULSE_MAX = 32767
    instances = []
    failure = None
    busy = False

    def __init__(self, channel, **kwargs):
        self.kwargs = kwargs
        self.closed = False
        self.instances.append(self)

    def write_pulses(self, durations, levels):
        assert len(durations) == len(levels)
        assert all(0 < d <= self.PULSE_MAX for d in durations)
        self.pulses = list(zip(durations, levels))
        if self.failure:
            raise self.failure

    def wait_done(self, timeout):
        return not self.busy

    def deinit(self):
        self.closed = True


spec = importlib.util.spec_from_file_location("tvbgone", ROOT / "tvbgone.py")
tvbgone = importlib.util.module_from_spec(spec)
with patch.dict(sys.modules, {
    "machine": types.SimpleNamespace(Pin=Pin),
    "esp32": types.SimpleNamespace(RMT=RMT),
}):
    spec.loader.exec_module(tvbgone)


def merge(pulses):
    result = []
    for duration, level in pulses:
        if not duration:
            continue
        if result and result[-1][1] == level:
            result[-1][0] += duration
        else:
            result.append([duration, level])
    return result


class PortTests(unittest.TestCase):
    def setUp(self):
        RMT.instances = []
        RMT.failure = None
        RMT.busy = False
        self.elapsed = 0
        self.delays = []
        def sleep(ms):
            self.elapsed += ms
            self.delays.append(ms)
        self.clock = patch.object(tvbgone, "time", types.SimpleNamespace(
            sleep_ms=sleep, ticks_ms=lambda: self.elapsed,
            ticks_diff=lambda a, b: a - b,
        ))
        self.clock.start()
        self.addCleanup(self.clock.stop)
        self.tv = tvbgone.TVBGone(data_dir=str(ROOT), verbose=False)

    def test_generated_data_is_current(self):
        for name, content in generate().items():
            self.assertEqual((ROOT / name).read_text(), content)

    def test_all_waveforms_against_compiled_cpp(self):
        compiler = shutil.which("c++")
        if compiler is None:
            self.skipTest("C++ compiler needed for independent source parity check")
        # Read the actual arrays through C++, independently of the converter.
        probe = r'''
#include <iostream>
#include "WORLDcodes.cpp"
void dump(IrCode **codes, int count) {
    for (int i = 0; i < count; ++i) {
        auto c = codes[i];
        std::cout << "[" << c->carrier_freq << ",[";
        for (int j = 0; j < c->num_pairs; ++j) {
            if (j) std::cout << ",";
            int k = 2 * c->sequence[j];
            std::cout << c->pairs[k] << "," << c->pairs[k+1];
        }
        std::cout << "]]\n";
    }
}
int main() { dump(NApowerCodes, num_NAcodes); dump(EUpowerCodes, num_EUcodes); }
'''
        with tempfile.TemporaryDirectory() as directory:
            cpp = Path(directory) / "probe.cpp"
            binary = Path(directory) / "probe"
            cpp.write_text(probe)
            subprocess.run([compiler, "-std=c++11", "-I", str(ROOT.parent / "TV-B-Gone_kit_V2"),
                            str(cpp), "-o", str(binary)], check=True, capture_output=True)
            original = [json.loads(line) for line in subprocess.check_output([str(binary)], text=True).splitlines()]
        converted = list(self.tv.codes("NA")) + list(self.tv.codes("EU"))
        self.assertEqual(len(converted), 287)
        self.assertEqual(len(original), len(converted))
        for code, (frequency, waveform) in zip(converted, original):
            durations, levels = tvbgone._pulses(code, RMT.PULSE_MAX)
            expected = merge((d, 1 - i % 2) for i, d in enumerate(waveform))
            self.assertEqual(code[1], frequency, code[0])
            self.assertEqual(merge(zip(durations, levels)), expected, code[0])
            self.assertTrue(all(0 < d <= RMT.PULSE_MAX for d in durations))

    def test_long_and_zero_spans(self):
        durations, levels = tvbgone._pulses(["test", 0, [10, 100000, 5, 0], [0, 1, 1]], 32767)
        self.assertEqual(merge(zip(durations, levels)), [[10, 1], [100000, 0], [10, 1]])

    def test_mark_safety_limit_before_hardware_setup(self):
        for frequency in (0, 38000):
            for mark in (90001, 99999, 100000, 100001, 200000):
                with self.assertRaisesRegex(ValueError, "90 ms safety limit"):
                    self.tv._transmit(["unsafe", frequency, [mark, 1000], [0]])
                self.assertEqual(self.tv.ir.level, 0)
        self.assertEqual(RMT.instances, [])

    def test_zero_spaces_cannot_bypass_safety_limit(self):
        # Each mark fits; combined high time across the zero space does not.
        with self.assertRaisesRegex(ValueError, "90 ms safety limit"):
            self.tv._transmit(["unsafe", 0, [50000, 0], [0, 0]])
        self.assertEqual(RMT.instances, [])
        self.assertEqual(self.tv.ir.level, 0)

    def test_safe_boundary_and_long_low_gaps(self):
        self.tv._transmit(["safe", 0, [90000, 200000], [0, 0]])
        self.assertEqual(merge(RMT.instances[-1].pulses),
                         [[90000, 1], [200000, 0], [90000, 1], [200000, 0]])
        self.assertTrue(RMT.instances[-1].closed)
        self.assertEqual(self.tv.ir.level, 0)

    def test_database_maximum_mark(self):
        longest = 0
        for region in ("NA", "EU"):
            for code in self.tv.codes(region):
                durations, levels = tvbgone._pulses(code, RMT.PULSE_MAX)
                longest = max(longest, max(d for d, level in merge(zip(durations, levels)) if level))
        self.assertEqual(longest, 19530)
        self.assertLess(longest, tvbgone.MAX_MARK_US)

    def test_board_and_button_priority(self):
        self.assertEqual([self.tv.ir.number, self.tv.led.number,
                          self.tv.na_button.number, self.tv.eu_button.number], [4, 8, 10, 9])
        self.assertEqual((self.tv.ir.level, self.tv.led.level), (0, 1))
        self.assertIsNone(self.tv._pressed_region())
        self.tv.eu_button.value(0)
        self.assertEqual(self.tv._pressed_region(), "EU")
        self.tv.na_button.value(0)
        self.assertEqual(self.tv._pressed_region(), "NA")

    def test_carrier_and_unmodulated(self):
        for frequency in (38400, 0):
            self.tv._transmit(["test", frequency, [600, 600], [0]])
            rmt = RMT.instances[-1]
            self.assertEqual(rmt.kwargs["tx_carrier"], (frequency, 50, 1) if frequency else None)
            self.assertEqual(rmt.kwargs["clock_div"], 80)
            self.assertFalse(rmt.kwargs["idle_level"])
            self.assertTrue(rmt.closed)
            self.assertEqual(self.tv.ir.level, 0)

    def test_all_region_codes_and_delays(self):
        for region, count in (("NA", 142), ("EU", 145)):
            RMT.instances = []
            self.delays = []
            self.assertEqual(self.tv.send_region(region, check_buttons=False), region)
            self.assertEqual(len(RMT.instances), count)
            self.assertEqual(self.delays.count(180), count)
            self.assertEqual(self.delays.count(25), count + (6 if region == "NA" else 12))
            self.assertTrue(all(r.closed for r in RMT.instances))

    def test_restart_switch_after_code(self):
        with patch.object(self.tv, "_pressed_region", side_effect=["EU"] + [None] * 145):
            self.assertEqual(self.tv.send_region("NA"), "EU")
        self.assertEqual(len(RMT.instances), 146)

    def test_restart_same_region(self):
        with patch.object(self.tv, "_pressed_region", side_effect=["NA"] + [None] * 142):
            self.assertEqual(self.tv.send_region("NA"), "NA")
        self.assertEqual(len(RMT.instances), 143)
        self.assertEqual(RMT.instances[0].pulses, RMT.instances[1].pulses)

    def test_cleanup_on_interrupt_and_error(self):
        for failure in (KeyboardInterrupt(), OSError("write failed")):
            RMT.failure = failure
            with self.assertRaises(type(failure)):
                self.tv.send_code("NA", 0)
            self.assertTrue(RMT.instances[-1].closed)
            self.assertEqual((self.tv.ir.level, self.tv.led.level), (0, 1))

    def test_timeout(self):
        RMT.busy = True
        with self.assertRaisesRegex(RuntimeError, "timed out"):
            self.tv.send_code("NA", 0)
        self.assertTrue(RMT.instances[-1].closed)
        self.assertEqual(self.tv.ir.level, 0)

    def test_invalid_inputs_and_missing_data(self):
        with self.assertRaises(ValueError):
            self.tv.send_region("XX")
        with self.assertRaises(ValueError):
            self.tv.send_code("NA", -1)
        with self.assertRaises(IndexError):
            self.tv.send_code("NA", 142)
        self.tv.data_dir = "/missing-tvbgone-data"
        with self.assertRaises(OSError):
            self.tv.send_region("NA")
        self.assertEqual(len(RMT.instances), 0)


if __name__ == "__main__":
    unittest.main()
