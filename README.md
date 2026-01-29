# TV-B-Gone Kit V2

The original (V1.2) <a href="http://www.adafruit.com/products/73">TV-B-Gone kit</a> is still available at Adafruit.  
  
I am currently working on a new (V2) TV-B-Gone kit. The first iteration uses a small-form ESP32 board, making it easier to solder, and easier to re-program for total beginners. This new version also uses a MOSFET, which allows for fewer parts, and allows for a longer range for turning off TVs in public places!  
  
Here is a 3D model of the preliminary PCB, for giving workshops:  
![tvbgone_kit_v2 0_3Dview](https://github.com/user-attachments/assets/32d0d371-b955-40d1-87db-9208d72992a9)  
  
## Project Info  
  
Imagine a dystopian future, filled with monitors, marketing at us everywhere we go. Sadly, we are living that future now. This kit is super useful, not only for yourself, but for everyone! And while maybe that's dramatic, the TV-B-Gone is perfect for playing pranks on your friends during the Super Bowl or getting some peace and quiet during dinner.  
  
The TV-B-Gone is a kit that, when soldered together, allows you to turn off almost any television within 50 meters or more.  It works on over 260 total power codes - 130 American/Asian and another 230 European/African codes.  You can select which zone with a button push.  
<img width="1800" height="1350" alt="TV-B-Gone_turning_TVs_off_graphic" src="https://github.com/user-attachments/assets/e1758c44-abcb-499a-b5c6-1da04a4d4314" />  
  
This is an unassembled kit which means that soldering is required - but it's very easy and a great introduction to soldering in general.  The kit is powered by 3x AA batteries - that aren't included - and the output comes from 2x narrow beam IR LEDs and 2x wide-beam IR LEDs.  
  
The TV-B-Gone covers almost any television from the brands listed below, including the latest LCDs.  
  
Here is a (partial) list of the brands of monitors that the TV-B-Gone kit V2 will turn off!  
  
Acer, Admiral, Aiko, Alleron, Anam National, AOC, Apex, Baur, Bell&Howell, Brillian, Bush, Candle, Citizen, Contec, Cony, Crown, Curtis Mathes, Daiwoo, Dimensia, Electrograph, Electrohome, Emerson, Fisher, Fujitsu, Funai, Gateway, GE, Goldstar, Grundig, Grunpy, Hisense, Hitachi, Infinity, JBL, JC Penney, JVC, LG, Logik, Loewe, LXI, Majestic, Magnavox, Marantz, Maxent, Memorex, Mitsubishi, MGA, Montgomery Ward, Motorola, MTC, NEC, Neckermann, NetTV, Nikko, NTC, Otto Versand, Palladium, Panasonic, Philco, Philips, Pioneer, Portland, Proscan, Proton, Pulsar, Pye, Quasar, Quelle, Radio Shack, Realistic, RCA, Samsung, Sampo, Sansui, Sanyo, Scott, Sears, SEI, Sharp, Signature, Simpson, Sinudyne, Sonolor, Sony, Soundesign, Sylviana, Tatung, TCL, Teknika, Thompson, Toshiba, Universum, Viewsonic, Wards, White Westinghouse, Zenith  
  
## Future Ideas/Plans  
  
* Add an IR receiver for learning new OFF-Codes  
* Make use of the ESP32's web-server to create a web-browser interface for updating the TV-B-Gone firmware  
* Use the web-browser interface for TV-B-Gone users to share new OFF-Codes  
* Maybe make use of the ESP32's BLE abilities and create an app for smartphones to use (though, at this point I'm not sure what for)  
  
