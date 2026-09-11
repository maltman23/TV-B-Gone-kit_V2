# MicroPython application workflow for the TV-B-Gone V2 ESP32-C3 kit.
set positional-arguments

default:
    @just --list

# Install the application upload and firmware flashing tools (run once).
setup:
    python3 -m venv .venv-micropython
    .venv-micropython/bin/python -m pip install mpremote 'esptool>=5,<6'

# Erase ALL board data and flash a downloaded ESP32_GENERIC_C3 .bin at 0x0.
# Usage: just flash PORT FIRMWARE.bin [BAUD]
flash port firmware baud="460800": _require-esptool
    @test -f "$2" || { echo 'Firmware file not found; download the ESP32_GENERIC_C3 .bin first.' >&2; exit 1; }
    .venv-micropython/bin/python -m esptool --chip esp32c3 --port "$1" --baud "$3" write-flash --erase-all 0x0 "$2"

# Regenerate the databases, run host checks, and stage the four device files.
build:
    python3 firmware/micropython/tools/convert_codes.py
    python3 -m unittest discover -s firmware/micropython/tests -v
    mkdir -p build/micropython
    cp firmware/micropython/tvbgone.py firmware/micropython/main.py firmware/micropython/codes_na.jsonl firmware/micropython/codes_eu.jsonl build/micropython/

# Check the source and generated databases without regenerating them.
check:
    python3 firmware/micropython/tools/convert_codes.py --check
    python3 -m unittest discover -s firmware/micropython/tests -v

# List connected serial devices and their identifiers.
devices: _require-mpremote
    .venv-micropython/bin/python -m mpremote connect list

# Build, copy to a MicroPython board, and reset into button operation.
# Usage: just upload [auto | /dev/cu.usbmodem... | id:SERIAL]
upload port="auto": _require-mpremote build
    .venv-micropython/bin/python -m mpremote connect "$1" fs cp build/micropython/tvbgone.py build/micropython/codes_na.jsonl build/micropython/codes_eu.jsonl build/micropython/main.py : + reset

# Open the board's serial REPL (Ctrl-] exits mpremote).
repl port="auto": _require-mpremote
    .venv-micropython/bin/python -m mpremote connect "$1" repl

# Restart the installed application without uploading it again.
reset port="auto": _require-mpremote
    .venv-micropython/bin/python -m mpremote connect "$1" reset

_require-mpremote:
    @test -x .venv-micropython/bin/python || { echo 'Run `just setup` first to install mpremote.' >&2; exit 1; }
    @.venv-micropython/bin/python -c 'import mpremote' || { echo 'Run `just setup` to install mpremote.' >&2; exit 1; }

_require-esptool:
    @test -x .venv-micropython/bin/python || { echo 'Run `just setup` first to install esptool.' >&2; exit 1; }
    @.venv-micropython/bin/python -c 'import esptool' || { echo 'Run `just setup` to install esptool.' >&2; exit 1; }
