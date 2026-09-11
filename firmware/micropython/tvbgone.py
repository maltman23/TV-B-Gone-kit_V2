"""TV-B-Gone Kit V2 for ESP32-C3 Super Mini, using hardware RMT.

Adapted from Mitch Altman, Limor Fried, Ken Shirriff and BenBE's sources.
License: CC BY-SA 4.0. See README.md for attribution and installation.
"""
import json
import time
from machine import Pin
from esp32 import RMT

NA = 1
EU = 0
# Hardware constraint: never schedule a continuous mark near 100 ms.
# Apply this conservative 90 ms limit even when a carrier is enabled.
MAX_MARK_US = 90_000
_DATA_DIR = __file__.rsplit("/", 1)[0] if "/" in __file__ else "."


def _region_name(region):
    if region == NA or region == "NA":
        return "NA"
    if region == EU or region == "EU":
        return "EU"
    raise ValueError("region must be NA, EU, 'NA', or 'EU'")


def _pulses(code, pulse_max):
    """Expand microsecond pairs, splitting long spans without changing level."""
    name, frequency, pairs, sequence = code
    if frequency < 0 or not sequence or len(pairs) % 2:
        raise ValueError("Invalid code: " + name)
    durations, levels = [], []
    continuous_mark = 0
    for index in sequence:
        if index < 0 or index * 2 + 1 >= len(pairs):
            raise ValueError("Invalid pair index: " + name)
        for offset in (0, 1):
            remaining = pairs[index * 2 + offset]
            if remaining < 0:
                raise ValueError("Negative duration: " + name)
            if offset == 0:
                continuous_mark += remaining
                if continuous_mark > MAX_MARK_US:
                    raise ValueError("IR mark exceeds 90 ms safety limit: " + name)
            elif remaining:
                continuous_mark = 0
            # Zero-length spaces in the source are omitted, never sent as
            # RMT zero durations (which would terminate the transmission).
            while remaining:
                span = min(remaining, pulse_max)
                durations.append(span)
                levels.append(1 - offset)
                remaining -= span
    if not durations:
        raise ValueError("Empty waveform: " + name)
    return durations, levels


class TVBGone:
    def __init__(self, ir_pin=4, led_pin=8, na_pin=10, eu_pin=9,
                 channel=0, data_dir=None, verbose=True):
        self.ir = Pin(ir_pin, Pin.OUT, value=0)
        self.led = Pin(led_pin, Pin.OUT, value=1)
        self.na_button = Pin(na_pin, Pin.IN, Pin.PULL_UP)
        self.eu_button = Pin(eu_pin, Pin.IN, Pin.PULL_UP)
        self.channel = channel
        self.data_dir = _DATA_DIR if data_dir is None else data_dir
        self.verbose = verbose
        self._rmt = None

    def _log(self, *args):
        if self.verbose:
            print(*args)

    def _open_codes(self, region):
        return open(self.data_dir + "/codes_" + _region_name(region).lower() + ".jsonl")

    def codes(self, region):
        """Yield [source name, carrier Hz, flat pairs in us, sequence]."""
        with self._open_codes(region) as stream:
            for line in stream:
                yield json.loads(line)

    def count(self, region):
        with self._open_codes(region) as stream:
            return sum(1 for _ in stream)

    def _stop_ir(self):
        try:
            if self._rmt is not None:
                rmt, self._rmt = self._rmt, None
                rmt.deinit()
        finally:
            self.ir.init(Pin.OUT, value=0)

    def deinit(self):
        """Release RMT and leave both LEDs off; safe to call repeatedly."""
        try:
            self._stop_ir()
        finally:
            self.led.value(1)

    def _transmit(self, code):
        try:
            # Validate the entire waveform before configuring hardware. Splits
            # at PULSE_MAX and zero-length spaces must not bypass the limit.
            durations, levels = _pulses(code, RMT.PULSE_MAX)
            frequency = code[1]
            # 80 MHz / 80 = 1 MHz: every tick is exactly one microsecond.
            # clock_div is also supported by newer resolution_hz-based firmware.
            carrier = (frequency, 50, 1) if frequency else None
            self._rmt = RMT(self.channel, pin=self.ir, clock_div=80,
                            idle_level=False, tx_carrier=carrier)
            self._rmt.write_pulses(durations, levels)
            started = time.ticks_ms()
            timeout = (sum(durations) + 999) // 1000 + 1000
            while not self._rmt.wait_done(timeout=0):
                if time.ticks_diff(time.ticks_ms(), started) > timeout:
                    raise RuntimeError("RMT transmission timed out")
                time.sleep_ms(1)
        finally:
            self._stop_ir()

    def send_code(self, region, index):
        """Send one code by zero-based position in the region's source table."""
        if not isinstance(index, int) or index < 0:
            raise ValueError("index must be a nonnegative integer")
        try:
            with self._open_codes(region) as stream:
                for position, line in enumerate(stream):
                    if position == index:
                        code = json.loads(line)
                        self._transmit(code)
                        return code[0]
            raise IndexError("power-code index out of range")
        finally:
            self.deinit()

    def _pressed_region(self):
        # Match the sketch: active-low buttons, NA wins if both are held.
        if not self.na_button.value():
            return "NA"
        if not self.eu_button.value():
            return "EU"
        return None

    def _blip(self):
        self.led.value(0)
        try:
            time.sleep_ms(25)
        finally:
            self.led.value(1)

    def _blink(self, count):
        for _ in range(count):
            self._blip()
            time.sleep_ms(125)

    def send_region(self, region=NA, check_buttons=True):
        """Send a region; buttons restart/switch after a complete code.

        With check_buttons=False, always send the requested region once.
        Returns the name of the region that completed.
        """
        region = _region_name(region)
        try:
            while True:
                # Open before blinking so missing data fails immediately.
                with self._open_codes(region) as stream:
                    time.sleep_ms(250)
                    blinks = 3 if region == "NA" else 6
                    self._blink(blinks)
                    self._log("Transmitting", region)
                    time.sleep_ms(500)
                    restart = None
                    for index, line in enumerate(stream):
                        code = json.loads(line)
                        self._log("POWER-Code", index, code[0])
                        self._blip()
                        self._transmit(code)
                        # 180 ms + the next 25 ms blip = source's 205 ms.
                        time.sleep_ms(180)
                        if check_buttons:
                            restart = self._pressed_region()
                            if restart is not None:
                                break
                if restart is not None:
                    region = restart
                    continue
                time.sleep_ms(500)
                self._blink(blinks)
                time.sleep_ms(500)
                self._log("Finished", region)
                return region
        finally:
            self.deinit()

    def run(self):
        """Wait for NA/EU buttons indefinitely; Ctrl-C returns to the REPL."""
        self._log("TV-B-Gone V2 / MicroPython / ESP32-C3 Super Mini")
        try:
            while True:
                region = self._pressed_region()
                if region is not None:
                    self.send_region(region)
                else:
                    time.sleep_ms(10)
        finally:
            self.deinit()
