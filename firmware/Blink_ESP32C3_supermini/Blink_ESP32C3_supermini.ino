/*
  Blink (for ESP32-C3 Super Mini)

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman
  modified 22 Jan 2026 for ESP32-C3 Super Mini
  by Mitch Altman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

/*
  Arduino IDE settings for ESP32-C3 Super Mini:
     Board:  Tools --> Board --> ESP32 --> ESP32C3 Dev Module
     USB CDC on Boot:  enabled
     CPU Frequency:  160MHz (WiFi)
     Flash Frequency:  80MHz
     Flash Mode:  DIO
     Flash Size:  4MB (32Mb)
     Partition Scheme:  Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
     Upload Speed:  921600
*/

#define LED_PIN 8

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital built-in LED pin as an output.
  pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
// The ESP32-C3 Super Mini board's built-in LED is active LOW (so LOW to turn it ON)
void loop() {
  digitalWrite(LED_PIN, LOW);      // turn the LED on
  delay(100);                      // wait for a 0.1 second
  digitalWrite(LED_PIN, HIGH);     // turn the LED off
  delay(1000);                     // wait for a 1 second
}
