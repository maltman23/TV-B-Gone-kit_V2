/**************************************************

TV-B-Gone main.h for Arduino version 0.002

  The original TV-B-Gone kit firmware is:
  TV-B-Gone Firmware version 1.2
       for use with ATtiny85v and TV-B-Gone kit v1.2 hardware
       (c) Mitch Altman + Limor Fried 2009

  3-December-2009
  Ported to Arduino by Ken Shirriff
  http://arcfn.com

  2-September-2015
  Mitch Altman
  Updated definitions for
       const struct IrCode *EUpowerCodes[] = {
  and
       const struct IrCode *NApowerCodes[] = {
  so that the sketch will compile with Arduino 1.6.5 software.
  The new definitions are:
       extern "C" const struct IrCode * const EUpowerCodes[] = {
  and
       extern "C" const struct IrCode * const EUpowerCodes[] = {

  20-Jun-2025
  BenBE and Mitch Altman
  The Off-Codes here are the same as the original compressed codes
       except the indices to the On-Time/Off-Time pairs table for each Off-Code are no longer data-compressed.
  The "convertWorldCodes.py" script converts from the original to the new version of the Off-Codes in the "extractedWorldCodeData.json" file

  17-Jul-2025
  Mitch Altman
  Updated for ESP32-C3 and new TV-B-Gone kit format of POWER-Code database.
  I'm having trouble getting the ESP32-C3 compiler for Arduino to accept variable pointers to constants 
     (even through the same code compiles fine, and works fine, when compiled for Arduino Uno boards),
  The code compiles and works fine if the entire database here ( struct IrCode {} ) and in WORLDcodes.cpp is variables (and not constants).

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

**************************************************/


#include <cstdint>      // needed for ESP32 on the Arduino IDE (must be declared before any other #include)


/*
This sketch was originally written for an Arduino board, and Arduino boards, using AVR chips,
for which the compiler transfers constants to RAM.  To keep the constants in program memory, PROGMEM was used, along with
   pgm_read_byte()
      and
   pgm_read_word()
to access the constants.
For ESP chips, there is no need for PROGMEM, since constants are kept in program memory.
To replace all occurances of
   pgm_read_byte()
	  and
   pgm_read_word()
we make use of the following macros
*/
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))


// The TV-B-Gone for Arduino can use
//   either the EU or the NA database of POWER CODES
// EU is for Europe, Middle East, Australia, New Zealand, and some countries in Africa and South America
// NA is for North America, Asia, and the rest of the world not covered by EU

// Two regions!
#define NA 1
#define EU 0

// boolean values
#define FALSE 0
#define TRUE 1

// What pins do what
#define VISLED 8             // (On Mitch's HOPE badge dev board (Rel 0.8.14), this is Arduino pin 13, which is D28, Green LED)
#define IRLED 2              // (On Mitch's HOPE badge dev board (Rel 0.8.14), this is Arduino pin 2, which is D23, IR_TX)
#define BUTTON_NA 10         // (On Mitch's HOPE badge dev board (Rel 0.8.14), this is Arduino pin 10, which is connected to SW1, Green push-button, TACT_A)
#define BUTTON_EU 9          // (On Mitch's HOPE badge dev board (Rel 0.8.14), this is Arduino pin 9, which is connected to SW2, Red push-button, TACT_B)

// Lets us calculate the size of the NA/EU databases
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)));

// set define to 0 to turn off debug output
#define DEBUG 0
#define DEBUGP(x) if (DEBUG == 1) { x ; }

// The structure of compressed code entries
struct IrCode {
  uint32_t carrier_freq;      // Carrier Frequency for the POWER-Code
  uint8_t num_pairs;          // number of On-Time/Off-Time pairs the POWER-Code has
  uint32_t * pairs;           // pointer to 'Pairs' table for the POWER-Code (which contains the unique On-Time/Off-Time pairs)
  uint8_t * sequence;         // pointer to 'Sequence' table for the POWER-Code (which contains the sequence of On-Time/Off-Time pairs to transmit)
};

// function prototypes
void xmitPair(uint32_t carFreq, uint32_t onTime, uint32_t offTime);
void blinkLEDnTimes(int numBlinks);
