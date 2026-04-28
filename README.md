# TV-B-Gone Kit V2

The original (V1.2) <a href="http://www.adafruit.com/products/73">TV-B-Gone kit</a> is still available at Adafruit.  
  
I am currently working on a new (V2) TV-B-Gone kit. The first iteration uses a small-form ESP32 board, making it easier to solder, and easier to re-program for total beginners. This new version also uses a MOSFET, which allows for fewer parts, and allows for a longer range for turning off TVs in public places!  
  
Photo of a completed first draft of the kit, for giving workshops:  
![PXL_20260212_211537771](https://github.com/user-attachments/assets/2c68db52-bae4-463f-90b8-3bed8a5e421f)

## Flash the Firmware

You can flash the latest firmware directly to your ESP32-C3 Super Mini from your browser — no toolchain required:

[![Open the TV-B-Gone webflasher](graphics/tv-b-gone-flash.png)](https://pedrominatel.github.io/TV-B-Gone-kit_V2/)

Connect your board, click the badge above, press **Connect**, select the serial port for the board, then press **Flash Firmware**.

## Project Info  
  
Imagine a dystopian future, filled with monitors, marketing at us everywhere we go. Sadly, we are living that future now. This kit is super useful, not only for yourself, but for everyone! And while maybe that's dramatic, the TV-B-Gone is perfect for playing pranks on your friends during the Super Bowl or getting some peace and quiet during dinner.  
  
The TV-B-Gone is a kit that, when soldered together, allows you to turn off almost any television within 50 meters or more.  It works on over 260 total power codes - 130 American/Asian and another 230 European/African codes.  You can select which zone with a button push.  
<img width="1800" height="1350" alt="TV-B-Gone_turning_TVs_off_graphic" src="https://github.com/user-attachments/assets/e1758c44-abcb-499a-b5c6-1da04a4d4314" />  
  
This is an unassembled kit which means that soldering is required - but it's very easy and a great introduction to soldering in general.  The kit is powered by 3x AA batteries - that aren't included - and the output comes from 2x narrow beam IR LEDs and 2x wide-beam IR LEDs.  
  
The TV-B-Gone covers almost any television from the brands listed below, including the latest LCDs.  
  
Here is a (partial) list of the brands of monitors that the TV-B-Gone kit V2 will turn off!  
  
Acer, Admiral, Aiko, Alleron, Anam National, AOC, Apex, Baur, Bell&Howell, Brillian, Bush, Candle, Citizen, Contec, Cony, Crown, Curtis Mathes, Daiwoo, Dimensia, Electrograph, Electrohome, Emerson, Fisher, Fujitsu, Funai, Gateway, GE, Goldstar, Grundig, Grunpy, Hisense, Hitachi, Infinity, JBL, JC Penney, JVC, LG, Logik, Loewe, LXI, Majestic, Magnavox, Marantz, Maxent, Memorex, Mitsubishi, MGA, Montgomery Ward, Motorola, MTC, NEC, Neckermann, NetTV, Nikko, NTC, Otto Versand, Palladium, Panasonic, Philco, Philips, Pioneer, Portland, Proscan, Proton, Pulsar, Pye, Quasar, Quelle, Radio Shack, Realistic, RCA, Samsung, Sampo, Sansui, Sanyo, Scott, Sears, SEI, Sharp, Signature, Simpson, Sinudyne, Sonolor, Sony, Soundesign, Sylviana, Tatung, TCL, Teknika, Thompson, Toshiba, Universum, Viewsonic, Wards, White Westinghouse, Zenith  
  
## Firmware

The firmware is available in two flavours, both targeting the **ESP32-C3 Super Mini** board.

### ESP-IDF Application (`firmware/TV-B-Gone-ESP-IDF`)

A standalone ESP-IDF project written in plain C.

- **ESP-IDF version:** v5.5 or later
- **Target:** `esp32c3`
- Uses the **RMT peripheral** for microsecond-accurate IR pulse generation
- All transmission logic runs in a **FreeRTOS task**
- GPIO assignments (IR LED, visible LED, NA/EU buttons) are fully configurable via `idf.py menuconfig` under **TV-B-Gone Configuration** — no source edits required

Quick start:
```bash
cd firmware/TV-B-Gone-ESP-IDF
idf.py set-target esp32c3
idf.py build
idf.py -p <PORT> flash monitor
```

See [`firmware/TV-B-Gone-ESP-IDF/README.md`](firmware/TV-B-Gone-ESP-IDF/README.md) for full build, flash, and configuration instructions.

### Reusable ESP-IDF Component

The TV-B-Gone ESP-IDF application now consumes the reusable `tvbgone_core`
implementation from the [Espressif Component Registry](https://components.espressif.com)
as `pedrominatel/tv-b-gone`.

The board-specific wrapper remains in
[`firmware/TV-B-Gone-ESP-IDF`](firmware/TV-B-Gone-ESP-IDF), where GPIO
assignments stay configurable through `idf.py menuconfig`.

### Arduino Sketch (`firmware/TV-B-Gone-Arduino`)

An Arduino IDE sketch targeting the ESP32-C3 Super Mini. See [`firmware/TV-B-Gone-Arduino/README.md`](firmware/TV-B-Gone-Arduino/README.md) for setup and IDE configuration.

---

## Future Ideas/Plans  
  
* Add an IR receiver for learning new OFF-Codes  
* Make use of the ESP32's web-server to create a web-browser interface for updating the TV-B-Gone firmware  
* Use the web-browser interface for TV-B-Gone users to share new OFF-Codes  
* Maybe make use of the ESP32's BLE abilities and create an app for smartphones to use (though, at this point I'm not sure what for)  
  
