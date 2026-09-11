# TV-B-Gone V2 for MicroPython

MicroPython port of `../TV-B-Gone_kit_V2/TV-B-Gone_kit_V2.ino` and its
`WORLDcodes.cpp`, for the same ESP32-C3 Super Mini board. The generated data
contains **142 NA and 145 EU entries**, preserving the source's region order,
shared codes, carrier frequencies, microsecond timing pairs and sequences.

## Board and firmware

For the **ESP32-C3 Super Mini with 4 MB flash**, use the official
**ESP32_GENERIC_C3 MicroPython v1.29.0** release:

- [Download ESP32_GENERIC_C3-20260824-v1.29.0.bin](https://micropython.org/resources/firmware/ESP32_GENERIC_C3-20260824-v1.29.0.bin).
- [Official ESP32-C3 downloads and flashing instructions](https://micropython.org/download/ESP32_GENERIC_C3/).

Choose the full **`.bin`** image, not `.app-bin`, `.elf`, a preview build, or
the generic ESP32/S2/S3 firmware. This build supports C3 boards with at least
4 MB flash and revision 3 or newer silicon, including our revision 0.4 board.
No custom MicroPython build is needed.

The code requires `esp32.RMT`, targeting MicroPython **v1.26.0 or later**
with the documented RMT API. The port uses
`RMT(channel, clock_div=80, tx_carrier=...)` and the two-list form of
`write_pulses`. Newer firmware retains `clock_div` as a compatibility argument.
On-device smoke checks passed on an ESP32-C3 revision 0.4 (4 MB flash) running
official MicroPython v1.29.0: startup, all 287 records, file hashes, the 90 ms
guard, and low-only RMT output with both carrier configurations. Actual IR
waveforms and television response have not yet been measured.

| Function | GPIO | Behavior |
| --- | --- | --- |
| IR MOSFET gate | 4 | Active high, idle low |
| Onboard visible LED | 8 | Active low |
| NA button | 10 | Button to ground, internal pull-up |
| EU button | 9 | Button to ground, internal pull-up |
| IR receiver | 3 | Reserved; learning is unimplemented, as in the Arduino source |

Keep the kit's existing MOSFET/IR LED circuit. **Do not move the IR output to
GPIO2**: the original source notes that its bootloader-high behavior can burn
out the LEDs. This port targets the GPIO4 revision. On older PCBs, check and
adapt the wiring before use. GPIO9 is also the ESP32-C3 boot button: release
it during normal power-up/reset; holding it is used to enter the bootloader.

## Install

### Download and flash MicroPython with just

Download the v1.29.0 `.bin` linked above. Connect the board with a USB data
cable and close Thonny or any serial monitor. From the repository root
(`TV-B-Gone-kit_V2`), run:

```sh
just setup       # Install mpremote and esptool in .venv-micropython
just devices     # Find the board's serial port
just flash /dev/cu.usbmodem1234561 ~/Downloads/ESP32_GENERIC_C3-20260824-v1.29.0.bin
```

Replace the example port with your board's port (typically `/dev/cu.usbmodem...`
on macOS or `/dev/ttyACM0` on Linux). `flash` requires an explicit serial port
and a downloaded image path; it does not accept mpremote's `auto` or `id:...`
selectors. Quote the image path if it contains spaces. If you previously ran
`just setup`, run it again to install the newly added esptool dependency.

**`just flash` erases all existing firmware and files on the board**, then
writes the image at **`0x0`** using esptool. Back up any device files you need
before running it. A successful flash installs MicroPython; install the
TV-B-Gone application with `just upload` as described below.

If esptool cannot connect, hold **BOOT (GPIO9)**, press and release **RESET**,
then release BOOT and retry. Alternatively, hold BOOT while plugging in USB,
then release it. Run `just devices` again if the port changes. After flashing,
press RESET with BOOT released (or unplug and reconnect) if the board stays
in the bootloader. For an unreliable connection, retry at a lower baud rate:

```sh
just flash /dev/cu.usbmodem1234561 ~/Downloads/ESP32_GENERIC_C3-20260824-v1.29.0.bin 115200
```

See the [esptool flashing reference](https://docs.espressif.com/projects/esptool/en/latest/esp32c3/esptool/basic-commands.html)
for `write-flash` and `--erase-all` details.

### Build and upload the application with just

With [just](https://just.systems/) installed, run these commands from the
repository root (`TV-B-Gone-kit_V2`):

```sh
just setup       # One-time: install mpremote and esptool in .venv-micropython
just build       # Generate data, run host tests, stage build/micropython
just devices     # List serial devices after plugging in the board
just upload      # Build, upload all four files, and reset the board
```

`build` creates a deployable MicroPython source bundle, not an ESP32 firmware
image. Install MicroPython on the board first using `just flash` above.
Python 3 is required; a `c++` compiler enables the independent waveform parity
test. `just check` checks the existing data and runs tests without regenerating.

`just upload` connects to the first available USB serial device using
mpremote's `auto` selection. With multiple devices, choose the intended board
explicitly using the port or serial identifier shown by `just devices`:

```sh
just upload /dev/cu.usbmodem1234561
just upload id:YOUR_DEVICE_SERIAL
just repl /dev/cu.usbmodem1234561
just reset /dev/cu.usbmodem1234561
```

Upload overwrites `tvbgone.py`, `codes_na.jsonl`, `codes_eu.jsonl` and `main.py`
in the device's root directory, then resets into the button-driven application.
Close Thonny or another serial monitor before uploading. Run `just upload`
after connecting the board; no background USB watcher is installed. The upload
tool is checked before building, and a failed build prevents uploading.

### Install firmware and copy files manually

To flash without `just`, download the same full v1.29.0 `.bin` linked above,
install [esptool](https://docs.espressif.com/projects/esptool/en/latest/esp32c3/installation.html),
and run the following with your board's serial port and downloaded image path.
This erases all existing firmware and device files:

```sh
python3 -m esptool --chip esp32c3 --port /dev/cu.usbmodem1234561 --baud 460800 write-flash --erase-all 0x0 ~/Downloads/ESP32_GENERIC_C3-20260824-v1.29.0.bin
```

With MicroPython installed, use Thonny's device file browser or `mpremote`.
From this `firmware/micropython` directory on your computer:

```sh
python3 -m pip install mpremote
mpremote connect auto fs cp tvbgone.py :tvbgone.py
mpremote connect auto fs cp codes_na.jsonl :codes_na.jsonl
mpremote connect auto fs cp codes_eu.jsonl :codes_eu.jsonl
```

If more than one board is connected, replace `auto` with its serial port.
These three files are sufficient for library use. To start button operation
automatically at power-up, also install the supplied entry point and reset:

```sh
mpremote connect auto fs cp main.py :main.py
mpremote connect auto reset
```

Copying `main.py` replaces an existing file of that name. For an existing
MicroPython application, call `TVBGone().run()` from your own entry point instead.
Keep the data files alongside `tvbgone.py`, or set `data_dir` explicitly.
The converter, tests and manifest are for the computer and need not be uploaded.

## Use from Python / REPL

```python
from tvbgone import TVBGone, NA, EU

tv = TVBGone()
tv.send_region(EU)                        # Full EU sweep, buttons can restart it
tv.send_region(NA, check_buttons=False)   # Exactly one NA sweep
tv.send_code("EU", 0)                    # One code, zero-based source-table index
tv.run()                                 # Wait for the physical buttons
```

`send_region()` returns the region that completed. `send_code()` returns its
C++ code name. The module accepts `NA`/`EU` constants or uppercase strings.
`tv.count("NA")` returns 142. `tv.codes("EU")` lazily yields records of the form
`[source_name, carrier_hz, flat_mark_space_pairs_us, pair_index_sequence]`.
Do not turn this iterator into a list on the board unless you need all codes
in RAM. For example, print just the first code with `next(tv.codes("EU"))`.

Pin assignments, RMT channel, data directory and logging can be customized:

```python
tv = TVBGone(ir_pin=4, led_pin=8, na_pin=10, eu_pin=9,
             channel=0, data_dir="/", verbose=False)
```

Ctrl-C interrupts `run()` or a sweep. Transmission methods release RMT and
return both LEDs to their off states in `finally` blocks, including on errors.
`tv.deinit()` can also be called explicitly and repeatedly. Methods are
synchronous; use one instance at a time, with no concurrent IR transmissions.

## Behavior and timing

As in the sketch, an NA/EU press waits 250 ms, flashes the indicator three/six
times (25 ms on, 125 ms off), then waits 500 ms before transmitting. Each code
has a 25 ms indicator blip and is followed by 180 ms of delay. Together with
the next blip this gives the source's nominal 205 ms inter-code pause, plus
file decoding, logging and peripheral setup overhead. The final indication
has the source's 500 ms pauses before and after it.

Buttons are sampled after each code and its pause. A held button restarts
the sweep at code zero, and the other region's button switches regions.
NA wins if both buttons are held. Like the original sketch, very brief presses
between sampling points can be missed, and holding a button can repeatedly
restart transmission. Idle polling adds at most approximately 10 ms latency.

RMT emits a complete code in one submission, with a 1 microsecond time unit
and nominal 50% carrier duty. Carrier-zero codes use unmodulated high marks.
Durations longer than `RMT.PULSE_MAX` are split into adjacent spans with the
**same level**, preserving long gaps without inserting extra IR pulses.
Zero-length spans are omitted because RMT uses zero durations as terminators.
The database is streamed one record at a time to keep heap usage small.

The LEDs must not be driven continuously high for 100 ms. Before configuring
RMT, the port rejects any continuous mark envelope longer than **90 ms**,
leaving 10 ms below that limit. This applies to both modulated and unmodulated
codes, includes adjacent marks separated by zero-length spaces, and cannot
be bypassed by splitting a mark into RMT-sized spans. Long low spaces remain
allowed. Rejected codes raise `ValueError` and leave the IR output low.

The Arduino `xmitPair()` enables PWM (or drives GPIO4 high for carrier-zero
codes), waits `onTime` microseconds, detaches PWM, then drives GPIO4 low for
`offTime`. It has no explicit maximum-on-time guard. The longest continuous
mark envelope in the supplied database is **19,530 us (19.53 ms)** in
`code_na065Code`, so all existing codes pass the new guard unchanged. The
Arduino source also explicitly moved IR from GPIO2 to GPIO4 to avoid GPIO2's
approximately two-second high state during programming.

The 90 ms check limits scheduled waveforms; it is not an independent hardware
cutoff for a firmware/peripheral fault or bootloader behavior. Physical IR
timing has not yet been measured on the board.

The requested envelope durations match the data exactly at 1 microsecond
resolution. Actual carrier frequency is subject to the ESP32 peripheral's
quantization. RMT removes Python execution from individual mark/space timing;
physical output still needs measurement, especially under other system load.
See the official [v1.26 RMT API](https://docs.micropython.org/en/v1.26.0/library/esp32.html#rmt)
and [current RMT API](https://docs.micropython.org/en/latest/library/esp32.html#rmt).

## Regenerate and verify

On the computer, from this directory:

```sh
python3 tools/convert_codes.py
python3 tools/convert_codes.py --check
python3 -m unittest discover -s tests -v
```

The converter resolves the actual C++ region pointer tables rather than
assuming codes occur in numeric order. The manifest records the source SHA-256
and counts. Tests cover all 287 waveforms against an independently compiled
C++ probe, long and zero-length spans, carrier-free output, pins, region
restarts, delays, invalid inputs, timeout and interruption cleanup. A C++
compiler named `c++` is needed for the independent parity test; that test
explicitly skips if no compiler is installed. Other tests use hardware mocks.

For board validation, capture GPIO4 with a logic analyzer or oscilloscope:

1. Confirm GPIO4 idles low and GPIO8 is high after startup and Ctrl-C.
2. Compare `send_code("NA", 0)` with the corresponding first record in
   `codes_na.jsonl`: carrier, marks, spaces and the complete sequence.
3. Check at least one zero-carrier record and one record with a space longer
   than 32767 us; confirm no IR mark is inserted within the long space.
4. Check both full sweeps, restart/switch buttons and indicator flashes.
5. Verify operation with the intended television.

Host tests cannot establish electrical behavior, IR range or receiver response.

## Attribution and license

This module, conversion tool and generated data are adaptations of the
TV-B-Gone sources in this repository and are distributed under
[Creative Commons Attribution-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-sa/4.0/).

Original firmware and captured code database: **Mitch Altman and Limor Fried**
(2009 and subsequent updates). Arduino port: **Ken Shirriff**. Database
conversion work: **BenBE and Mitch Altman**. ESP32-C3 Kit V2 firmware and
subsequent code updates: **Mitch Altman**. This adaptation replaces Arduino
GPIO/LEDC transmission with MicroPython RMT, adds streamed JSON-lines data,
a reusable Python interface and host verification. Original source files and
their notices remain unchanged.
