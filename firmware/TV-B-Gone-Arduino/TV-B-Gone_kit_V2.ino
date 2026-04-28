/*
  This TV-B-Gone sketch is for the TV-B-Gone V2 kit
     using the ESP32-C3 Super Mini board.
        This sketch will also work for both: 
           ESP32 WEMOS D1 MINI and HOPE badge Rel 0.4.18 (with ESP32-C3)
              -- (but with different pin definitions)

  NOTE: Please see the comments at the "xmitPair()" function (down below)
        for an explanation of the TV-B-Gone project as well as for how this sketch works.


  These are the Arduino IDE settings to use:
     Board:  ESP32C3 Dev Module
     USB CDC on Boot:  enabled
     CPU Frequency:  160MHz (WiFi)
     Flash Frequency:  80MHz
     Flash Mode:  DIO
     Flash Size:  4MB (32Mb)
     Partition Scheme:  Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
     Upload Speed:  921600


  When you click the NA or EU pushbutton:
      quickly blink the visible LED 3x for NA and 6x for EU
      for all codes for the given region (NA or EU):
           blip the visible LED
           grab the info for the next POWER-Code from WORLDcodes.cpp
           transmit the POWER-Code on the IR LED (using the ESP32 LEDC library for PWM)
           delay 205 msec
           If the EU or NA push-button is pushed while transmitting the database of POWER-Codes,
                then start transmitting the entire sequence again from the beginning.
      quickly blink the visible LED 3x for NA and 6x for EU

  The hardware to connect to the ESP32-C3:
  - 3xAA battery pack connected between the "5V" pin and the "GND" pin (Vcc is thus 4.5V)
  - IR LED connected to digital output pin 2 through an IRLU024NPBF MOSFET amplifier:
       -- Source to GND
       -- Gate through a 1K resistor to digital output pin GPIO2
       --    47K Pull-down resistor between Gate and GND
       -- Drain to 4.7 Ohm (1/2W) resistor to IR333 LED to +4.5V (LED Anode to +4.5V)
       --    (It is possible to use severl IR333/4.7 Ohm resistor pairs in parallel)
  - NA pushbutton attached between digital input pin GPIO10 and GND (with internal pullup resistor enabled)
  - EU pushbutton attached between digital input pin GPIO9 and GND (with internal pullup resistor enabled)
  - (For the visible LED indicator, we use the built-in LED on the ESP32-C3 Super Mini, which is internally connected to GPIO8)


  12-Jul-2025   Mitch Altman
  hacked from Arduino example sketches
     and from the Random Nerd Tutorials on "ESP32 PWM with Arduino IDE"
     https://randomnerdtutorials.com/esp32-pwm-arduino-ide/#esp32-pwm-ledc-example-code

  17-Jul-2025   Mitch Altman
  Got the pointers to work well, for pointing to the appropriate tables for transmitting each POWER-Code
  (but I couldn't figure out how to make the pointers work with the database of POWER-Codes as constants, so they are all variables).

  21-Jan-2026   Mitch Altman
  Updated for ESP32-C3 Super Mini board


  Creative Commons CC BY-SA 4.0
  This license enables reusers to distribute, remix, adapt, and build
  upon the material in any medium or format, so long as attribution is
  given to the creator. The license allows for commercial use. If you remix, adapt, or
  build upon the material, you must license the modified material under identical
  terms. CC BY-SA includes the following elements:
     BY: credit must be given to the creator.
     SA: Adaptations must be shared under the same terms.

*/


#include "main.h"


// definitions for timings
#define VIS_LED_BLIP_TIME 25     // blip the visible LED this long (in msec) before transmitting a POWER-Code
#define TIME_BETWEEN_CODES 205   // time to wait (in msec) after sending a POWER-Code and before sending the next POWER-Code
#define BLIP_TIME 125            // length of delay between blinks when quickly blinking the LED

// definitions needed for the ESP32 LEDC library for Arduino
#define DUTY_RESOLUTION 8        // use 8-bit PWM duty cycle resolution (which allows DUTY_CYCLE to have a value of 0 to 255)
#define DUTY_CYCLE 128           // PWM duty cycle (since DUTY_RESOLUTION=8, 128 is a 50% duty cycle)
#define PWM_CHANNEL 0            // PWM channel to use (for this sketch, which channel we use isn't important)

// variables
int buttonStateNA = 0;         // for reading the NA pushbutton status
int buttonStateEU = 0;         // for reading the EU pushbutton status
int buttonStateNA_again = 0;   // for reading the NA pushbutton status if it is pushed during the transmission of the POWER-Codes
int buttonStateEU_again = 0;   // for reading the EU pushbutton status if it is pushed during the transmission of the POWER-Codes
int region;                    // keep track of either NA or EU
int startOver;                 // for starting the transmission over from the beginning, if the user presses a button while transmitting
int numCodes;                  // number of POWER-Codes to transmit (will be set to either num_NAcodes or num_EUcodes, depending on which region we're transmitting)
int numPairs;                  // number of On-Time/Off-Time pairs that comprise a POWER-Code (which is gotten from the POWER-Code's Code table)
uint32_t carrierFreq;          // for reading the Carrier Frequency from the Codes table for each POWER-Code
uint8_t pairsIndex;            // for reading the next index from the POWER-Code's Sequence table -- this is an index to the POWER-Code's Pairs table (of unique On-Time/Off-Time pairs)
uint32_t OnTime;               // for reading the On-Time from the POWER-Code's Pairs table
uint32_t OffTime;              // for reading the Off-Time from the POWER-Code's Pairs table

// constants and variables from "WORLDcodes.cpp"
extern "C" struct IrCode * NApowerCodes[];
extern "C" struct IrCode * EUpowerCodes[];
extern uint8_t num_NAcodes;
extern uint8_t num_EUcodes;

// pointers for grabbing the appropriate info for each POWER-Code to transmit
struct IrCode * pwrCode_ptr;   // points to either the table of NApowerCodes or EUpowerCodes
uint32_t * pairsTab_ptr;       // points to the Pairs table for a POWER-Code (which contains all of the unique On-Time/Off-Time pairs for a given POWER-Code)
uint8_t * sequenceTab_ptr;     // points to the Sequence table for a POWER-Code (which contains indices into the Pairs table, for transmitting the proper sequence of On-Time/Off-time pairs for the POWER-Code)



/****************************************/
/****************************************/
void setup() {
  Serial.begin(9600);                 // use Serial Monitor for debugging purposes
  delay(100);                         // short delay to let the serial hardware start

  pinMode(VISLED, OUTPUT);            // initialize the visible LED pin as an output (Green LED)
  pinMode(IRLED, OUTPUT);             // initialize the IR LED pin as an output
  pinMode(BUTTON_NA, INPUT_PULLUP);   // initialize the NA pushbutton pin as an input with pull-up resistor enabled (Green button)
  pinMode(BUTTON_EU, INPUT_PULLUP);   // initialize the EU pushbutton pin as an input with pull-up resistor enabled (Red button)

  // "startOver" will become TRUE if the user pushes either the NA or EU push-button while transmitting the POWER-Codes
  startOver = FALSE;

  Serial.println(".....TV-B-Gone on an ESP32-C3 Super Mini board.....");
  Serial.println("                       V0.2");
  Serial.println("");
  Serial.println("");
}


/****************************************/
/****************************************/
void loop() {
  // check the two push-buttons
  buttonStateNA = digitalRead(BUTTON_NA);      // buttonStateNA is normally HIGH, but LOW when NA button is pressed
  buttonStateEU = digitalRead(BUTTON_EU);      // buttonStateEU is normally HIGH, but LOW when EU button is pressed

Start_transmission:

  if (startOver == TRUE) {
    buttonStateNA = buttonStateNA_again;
    buttonStateEU = buttonStateEU_again;
    // "startOver" will become TRUE if the user pushes either the NA or EU push-button while transmitting the POWER-Codes
    startOver = FALSE;
    Serial.println("");
    Serial.println("");
    Serial.println("Transmission of all POWER-Codes about to start again from the beginning");
    Serial.println("");
    Serial.println("");
  }

  // check if either of the pushbuttons are pressed (both are LOW while pressed)
  // NOTE: if both the NA and the EU push-buttons are pushed, then NA will be chosen
  if ( (!buttonStateNA || !buttonStateEU) == HIGH ) {

    delay(250);   // wait a while after pressing the NA or EU push-button

    if (buttonStateNA == LOW) {
      region = NA;
      numCodes = num_NAcodes;   // number of POWER-Codes to transmit for the NA database
      blinkLEDnTimes(3);        // blink the visible LED quickly 3 times to indicate NA
      Serial.println("NA button pressed");
      Serial.print("# of NA POWER-Codes: ");
      Serial.println(numCodes);
      //Serial.print("sizeof NApowerCodes: ");
      //Serial.println( sizeof(NApowerCodes) );
      //Serial.print("sizeof *NApowerCodes: ");
      //Serial.println( sizeof(*NApowerCodes) );
      Serial.println("");
    }
    else if (buttonStateEU == LOW) {
      region = EU;
      numCodes = num_EUcodes;  // number of POWER-Codes to transmit for the EU database
      blinkLEDnTimes(6);       // blink the visible LED quickly 6 times to indicate EU
      Serial.println("EU button pressed");
      Serial.print("# of EU POWER-Codes: ");
      Serial.println(numCodes);
      //Serial.print("sizeof EUpowerCodes: ");
      //Serial.println( sizeof(EUpowerCodes) );
      //Serial.print("sizeof *EUpowerCodes: ");
      //Serial.println( sizeof(*EUpowerCodes) );
      Serial.println("");
    }
    else {
      blinkLEDnTimes(25);    // blink the visible LED quickly 25 times to indicate an error (this should never happen)
      DEBUGP( Serial.println("ERROR: no button pressed, yet transmission started") );
      DEBUGP( Serial.println("") );
    }

    delay(500);   // wait a while after quickly blinking the visible LED and before starting transmission of POWER-Codes

    // loop to transmit all POWER-Codes
    for (int powerCodeCount=0; powerCodeCount<numCodes; powerCodeCount++) {

      // get the address of the next POWER-Code into a pointer
      if (region == NA) {
        pwrCode_ptr = NApowerCodes[powerCodeCount];
      }
      else if (region == EU) {
        pwrCode_ptr = EUpowerCodes[powerCodeCount];
      }

      // blip the visible LED (to indicate that a new POWER-Code is about to be transmitted on the IR LED)
      digitalWrite(VISLED, LOW);   // the built-in LED on the ESP32-C3 Super Mini is active-LOW (LOW turns ON the LED)
      delay(VIS_LED_BLIP_TIME);
      digitalWrite(VISLED, HIGH);

      // read the info from the IrCode table for this POWER-Code
      carrierFreq = pwrCode_ptr->carrier_freq;     // Carrier Frequency for this POWER-Code
      numPairs = pwrCode_ptr->num_pairs;           // number of On-Time/Off-Time pairs to transmit for this POWER-Code
      pairsTab_ptr = (pwrCode_ptr->pairs);         // point to the Pairs table of unique On-Time/Off-Time pairs for this POWER-Code
      sequenceTab_ptr = (pwrCode_ptr->sequence);   // point to the Sequence table of indices to the unique On-Time/Off-Time pairs for this POWER-Code

      // now that we have read all of the needed info from the current POWER-Code,
      // increment the pointer to the next POWER-Code in the table
      pwrCode_ptr++;

      #if DEBUG > 0
      DEBUGP( Serial.print("The Carrier Frequency for this POWER-Code is: ") );
      DEBUGP( Serial.println(carrierFreq) );
      DEBUGP( Serial.print("The # On-Time/Off-Time pairs for this POWER-Code is: ") );
      DEBUGP( Serial.println(numPairs) );
      DEBUGP( Serial.print("First 4 unique On-Time/Off-Time pairs: ") );
              for (int i=0; i<8; i=i+2) {
      DEBUGP(   Serial.print(pairsTab_ptr[i]) );
      DEBUGP(   Serial.print(",  ") );
      DEBUGP(   Serial.print(pairsTab_ptr[i+1]) );
      DEBUGP(   Serial.print("     ") );
              }
      DEBUGP( Serial.println("") );
      DEBUGP( Serial.print("Sequence: ") );
              for (int i=0; i<numPairs; i++) {
      DEBUGP(   Serial.print(sequenceTab_ptr[i]) );
      DEBUGP(   Serial.print(",  ") );
              }
      DEBUGP( Serial.println("") );
      #endif

      Serial.print("Transmitting POWER-Code ");
      Serial.print(powerCodeCount);
      Serial.print("...");

      // transmit all On-Time/Off-Time pairs for this POWER-Code
      for (int i=0; i<numPairs; i++) {
        pairsIndex = sequenceTab_ptr[i];        // read the next index from the POWER-Code's Sequence table -- this is an index to the POWER-Code's Pairs table (of unique On-Time/Off-Time pairs)
        DEBUGP( Serial.print(" ") );
        DEBUGP( Serial.print(pairsIndex) );
        DEBUGP( Serial.print(": ") );
        pairsIndex = pairsIndex * 2;            // multiply the index by 2 since we want to index pairs in the Pairs table
        OnTime = pairsTab_ptr[pairsIndex];      // read the On-Time from the POWER-Code's Pairs table
        OffTime = pairsTab_ptr[pairsIndex+1];   // read the Off-Time from the POWER-Code's Pairs table
        xmitPair( carrierFreq, OnTime, OffTime );
      }

      Serial.println("Done");
      DEBUGP( Serial.println("") );

      // delay for a while between sending POWER-Codes (to give IR receiver on TVs long enough to reset themselves after receiving other TV-B-Gone POWER-Codes)
      delay(TIME_BETWEEN_CODES - VIS_LED_BLIP_TIME);   // Since we already waited for VIS_LED_BLIP_TIME msec, we can subtract this amount from TIME_BETWEEN_CODES

      // check the two push-buttons to see if the user wants to start the transmission of POWER-Codes over from the start of the database
      buttonStateNA_again = digitalRead(BUTTON_NA);      // buttonStateNA is normally HIGH, but LOW when NA button is pressed
      buttonStateEU_again = digitalRead(BUTTON_EU);      // buttonStateEU is normally HIGH, but LOW when EU button is pressed
      // check if either of the pushbuttons are pressed (both are LOW while pressed)
      // if a button is pushed, then break from this main "for" loop so we can start the transmission of all POWER-Codes over from the beginning
      if ( (!buttonStateNA_again || !buttonStateEU_again) == HIGH ) {
        startOver = TRUE;
        break;
      }

    }

    if (startOver) goto Start_transmission;

    Serial.println("");
    Serial.print("Finished transmitting entire database for ");
    if (buttonStateNA == LOW) {
      Serial.println("NA.");
    }
    else if (buttonStateEU == LOW) {
      Serial.println("EU.");
    }
    else {
      DEBUGP( Serial.println("ERROR.") );
    }
    Serial.println("");
    Serial.println("");

    delay(500);   // wait a while after transmission of POWER-Codes
    if (buttonStateNA == LOW) {
      blinkLEDnTimes(3);   // blink the visible LED quickly 3 times to indicate end of NA transmission
    }
    else if (buttonStateEU == LOW) {
      blinkLEDnTimes(6);   // blink the visible LED quickly 6 times to indicate end of EU transmission
    }
    else {
      blinkLEDnTimes(25);    // blink the visible LED quickly 25 times to indicate an error (this should never happen)
    }
    delay(500);   // wait a while after transmission of POWER-Codes

  }

  // neither NA nor EU push-button were pressed
  else {
    digitalWrite(IRLED, LOW);    // make sure that the IR LED is off
    digitalWrite(VISLED, HIGH);  // make sure that the visible LED is off (the built-in LED on the ESP32-C3 Super Mini is active-LOW (HIGH turns OFF the LED) )
  }

}



/*****************************************************/
//      Functions
/*****************************************************/



/****************************************
/   xmitPair()
/
/ Transmit one On-Time/Off-Time pair
/
/
/ Explanation of how TV-B-Gone transmits POWER-Codes
/ --------------------------------------------------
/
/ There are many kinds of encoding schemes for IR remote control codes.
/ Yet, from my perspective, I don't care what type of IR code each of my POWER-Codes are.
/ As long as TV-B-Gone turns TVs off, we're good!
/ To accomplish this, each IR POWER-Code simply needs to be the same as the one the TV is expecting so that it turns Off.
/
/ TV-B-Gone transmits only POWER-Codes (and none of the other useless IR remote control codes, such as Channel-Up or Volume-Down...).
/ With a few hundred Power-Codes, we can turn off virtually all TVs in public places in the world.
/
/ Since the TVs in public places are different in different parts of the world,
/    the database of Power-Codes in TV-B-Gone is divided into two databases:
/       * NA (North America, South America, Asia)
/       * EU (Europe, Middle East, Africa, Australia, New Zealand)
/
/ The transmission sequence is:
/   - most popular Power-Code
/   - second most popular Power-Code
/   - third most popular Power-Code
/   - . . .
/   - least popular Power-Code
/
/ There is a separate transmission sequence for NA and for EU.
/
/ To mimic any IR remote control code, I consider all IR POWER-Codes to be a series of "On-Times" and "Off-Times".
/ When the IR LED is "On", it is usually pulsing at the "Carrier Frequency" of the POWER-Code.
/ The length of time it is "On" is an "On-Time".
/    (NOTE: Some POWER-Codes have no Carrier Frequency -- for these, "On" simply means that the IR LED is lit up for the On-Time.)
/ When the IR LED is "Off", it is emitting no light at all.
/ The length of time it is "Off" is an "Off-Time".
/ Any POWER-Code is a sequence of the IR LED being "On" for a while, then "Off" for a while, "On" for a while, then "Off" for a while...
/ If all of the various "On" times and "Off" times in the sequence mimic the timing that the TV expects,
/    then the TV will turn off.  (Yay!)
/ All POWER-Codes have a limited number of these "On-Time/Off-Time pairs".
/ All of the POWER-Codes that I have seen have less than 16 unique pairs.
/ So, transmitting any given POWER-Code requires determining all of the unique On-Time/Off-Time pairs that comprise that particular POWER-Code,
/    and transmitting the appropriate sequence of On-Time/Off-Time pairs.
/ In 2003-2004 I spent a year and a half of my life acquiring all of the POWER-Codes I could find.
/    From each POWER-Code, I determined:
/        the code's Carrier Frequency,
/        the code's unique On-Time/Off-Time pairs,
/        and the sequence of them that comprises each POWER-Code.
/ All of this data is now open source (and has been since I released it in 2006).
/
/ This will be more clear with an example!
/
/ So, here is an example POWER-Code:
/ ---------------------------------
/
/    Almost all of the SONY TVs in the world use one POWER-Code.
/           The Carrier Frequency for the Sony POWER-Code is:  38,400 Hz (38.4 Khz).
/           The Sony POWER-Code has only 4 unique On-Time/Off-Time pairs (in microseconds)
/                       pair 0:   600 /   600
/                       pair 1:   600 / 27000
/                       pair 2:  1200 /   600
/                       pair 3:  2400 /   600
/    (NOTE: in C/C++, counting starts from 0, so the above pairs are numbered starting with "0".)
/           The Sony POWER-Code is the following sequence of the above pairs:
/                       3 2 0 2 0 2 0 0 2 0 0 0 1 3 0 0 2 0 2 0 0 2 0 0 0 1
/           So, the first pair to be transmitted to the IR LED is: pair 3.
/               That means that the IR LED blinks at its Carrier Frequency (38.4KHz) for 2400 usec, followed by the IR LED being off for 600 usec.
/               Then pair 2: the IR LED blinks for 1200 usec followed by being off for 600 usec.
/               Then pair 0: the IR LED blinks for 600 usec followed by being off for 600 usec.
/               Then pair 2: the IR LED blinks for 1200 usec followed by being off for 600 usec.
/               etc., till all 26 of the POWER-Code's pairs are transmitted.
/                  If the resultant light from the IR LED hit a Sony TV, that Sony TV would be Off!  (Yay!)
/
****************************************/

// xmitPair( carFreq, onTime, offTime )
//
// This function:
//    Pulses the IR LED at the Carrier Frequency (given by carFreq) for the number of msec given by onTime,
//    followed by the IR LED being off for the number of msec given by offTime
//       (NOTE: if the Carrier Frequency is "0", then don't pulse the IR LED during "onTime", but merely turn the IR LED on for "onTime")
//
// This function uses the ESP32 LEDC library, which makes use of a PWM timer
//
// global values needed for the LEDC library:
//    IRLED           --  the pin the IR LED is connected to
//    DUTY_RESOLUTION --  PWM duty cycle resolution (which determines the range of values of DUTY_CYCLE)
//    DUTY_CYCLE      --  PWM duty cycle (if DUTY_RESOLUTION=8, dutyCyle can be between 0 and 255)
//    PWM_CHANNEL     --  PWM channel to use (for this sketch, which channel we use isn't important)
//
// arguments:
//    carFreq  --  Carrier Frequency
//    onTime   --  the amount of time (in usec) to pulse the IR LED at IRLED (if carFreq = 0, then the IR LED is simply on, and not pulsed)
//    offTime  --  the cmount of time (in usec) for the IR LED to be off (after the onTime)
//

void xmitPair(uint32_t carFreq, uint32_t onTime, uint32_t offTime) {
  DEBUGP( Serial.print(onTime) );
  DEBUGP( Serial.print(", ") );
  DEBUGP( Serial.print(offTime) );
  DEBUGP( Serial.print("     ") );

  // if the Carrier Frequency is 0, then there is no Carrier Frequency, so don't pulse the IR LED (it will be solidly On)
  if (carFreq != 0) {
    // start PWM at carrier frequency to pulse the IR LED
    ledcAttachChannel(IRLED, carFreq, DUTY_RESOLUTION, PWM_CHANNEL);
    ledcWrite(IRLED, DUTY_CYCLE);
  } else {
  // IR LED "On"
  digitalWrite(IRLED, HIGH);
  }
  // "On" for the number of usec given in onTime
  ets_delay_us(onTime);

  // stop PWM, so IR LED is off (if carFreq isn't 0)
  if (carFreq != 0) {
    ledcDetach(IRLED);
  }
  // IR LED "Off" for the number of usec given in offTime
  digitalWrite(IRLED, LOW);
  // keep the IR LED off for the number of usec given in offTime
  ets_delay_us(offTime);

}



/****************************************
/   blinkLEDnTimes( numBlinks )
/
/ Quickly blink the visible LED the number of times given by "numBlinks"
/
/ global values needed:
/    VISLED -- gives the GPIO pin that the visible LED is connected to
/
/ arguments:
/     numBlinks -- the number of times to quickly blink the visible LED
/
****************************************/

void blinkLEDnTimes(int numBlinks) {
  for (int i=0; i<numBlinks; i++) {
    digitalWrite(VISLED, LOW);   // the built-in LED on the ESP32-C3 Super Mini is active-LOW (LOW turns ON the LED)
    delay(VIS_LED_BLIP_TIME);
    digitalWrite(VISLED, HIGH);
    delay(BLIP_TIME);
  }
}
