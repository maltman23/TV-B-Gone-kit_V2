# TV-B-Gone Kit V2 — Arduino Firmware

Arduino firmware for the TV-B-Gone Kit V2, targeting the **ESP32-C3 Super Mini** board.

## Overview

This sketch transmits IR power-off codes for a wide range of television brands. It supports two regional databases:

- **NA** — North America, Asia, and the rest of the world not covered by EU (~130 codes)
- **EU** — Europe, Middle East, Australia, New Zealand, and parts of Africa and South America (~230 codes)

Press the NA (green) or EU (red) pushbutton to start transmitting the corresponding code database. The visible LED blinks 3 times for NA and 6 times for EU at the start and end of each transmission sequence.

## Hardware

The firmware targets the **ESP32-C3 Super Mini** board with the following wiring:

| Signal | GPIO Pin | Description |
|--------|----------|-------------|
| Visible LED | GPIO 8 | Built-in LED on the ESP32-C3 Super Mini |
| IR LED | GPIO 2 | IR LED via IRLU024NPBF MOSFET amplifier |
| NA Button | GPIO 10 | Green pushbutton to GND (internal pull-up) |
| EU Button | GPIO 9 | Red pushbutton to GND (internal pull-up) |

**Power:** 3×AA battery pack connected to the 5V and GND pins (Vcc ≈ 4.5V).

**IR LED circuit:**
- MOSFET Source → GND
- MOSFET Gate → 1 kΩ resistor → GPIO 2 (with 47 kΩ pull-down to GND)
- MOSFET Drain → 4.7 Ω (½W) resistor → IR333 LED → +4.5V (anode to +4.5V)
- Multiple IR LED/resistor pairs can be connected in parallel for longer range

## Arduino IDE Configuration

| Setting | Value |
|---------|-------|
| Board | ESP32C3 Dev Module |
| USB CDC on Boot | Enabled |
| CPU Frequency | 160 MHz (WiFi) |
| Flash Frequency | 80 MHz |
| Flash Mode | DIO |
| Flash Size | 4MB (32Mb) |
| Partition Scheme | Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS) |
| Upload Speed | 921600 |

## Files

| File | Description |
|------|-------------|
| `TV-B-Gone_kit_V2.ino` | Main sketch — setup, loop, and IR transmission logic |
| `main.h` | Pin definitions, region constants, and macro definitions |
| `WORLDcodes.cpp` | NA and EU IR power-code databases |

## Usage

1. Open `TV-B-Gone_kit_V2.ino` in the Arduino IDE.
2. Configure the board settings as shown above.
3. Compile and upload to the ESP32-C3 Super Mini.
4. Press the **NA** (green) button or the **EU** (red) button to begin transmitting codes.
5. The visible LED blinks briefly before each code is sent.
6. Press either button again during transmission to restart from the beginning.

Serial output is available at **9600 baud** for debugging.

## License

Creative Commons **CC BY-SA 4.0** — you are free to distribute, remix, adapt, and build upon this work, including for commercial use, as long as attribution is given and adaptations are shared under the same terms.

Original firmware by Mitch Altman and Limor Fried (2009). Ported to Arduino by Ken Shirriff (2009). Updated for ESP32-C3 Super Mini by Mitch Altman (2026).
