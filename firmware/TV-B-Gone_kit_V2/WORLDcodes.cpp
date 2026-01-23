/**************************************************

TV-B-Gone WORLDcodes for Arduino version 0.002

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
       struct IrCode *EUpowerCodes[] = {
  and
       struct IrCode *NApowerCodes[] = {
  so that the sketch will compile with Arduino 1.6.5 software.
  The new definitions are:
       extern "C" struct IrCode * const EUpowerCodes[] = {
  and
       extern "C" struct IrCode * const EUpowerCodes[] = {

  20-Jun-2025
  BenBE and Mitch Altman
  The POWER-Codes here are the same as the original compressed codes
       except the indices to the On-Time/Off-Time pairs table for each Off-Code are no longer data-compressed.
  The "convertWorldCodes.py" script converts from the original to the new version of the Off-Codes in the "extractedWorldCodeData.json" file

  25-Jun-2025
  This "json2cppConvert.py" script to create the new "WORLDcodes.txt" file from the "extractedWorldCodesData.json" file
       was written by Mitch Altman

  17-Jul-2025
  Mitch Altman
  I'm having trouble getting the ESP32-C3 compiler for Arduino to accept variable pointers to constants 
     (even through the same code compiles fine, and works fine, when compiled for Arduino Uno boards),
  The code compiles and works fine if the entire database here in WORLDcodes.cpp is variables (and not constants).
  Updated comments.

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
#include "main.h"

/**************************************************

   Table of POWER codes
   Codes captured from Generation 3 TV-B-Gone by Limor Fried & Mitch Altman

   This version of WORLDcodes does not compress the indices to the On-Time/Off-Time pair tables for each POWER-Code.

   Each POWER-Code consists of three tables:
      * 'Pairs' table -- a table of unique On-Time/Off-Time pairs for the POWER-Code -- e.g.: code_na000Pairs[]
      * 'Sequence' table -- table of indices to the On-Time/Off-Time Pairs table     -- e.g.: code_na000Sequence[]
      * 'Code'  table                                                                -- e.g.: code_na000Code[]
           that gives:
              ** the POWER-Code's Carrier Frequency
              ** number of On-Time/Off-Time pairs in the transmission sequence that comprise the POWER-Code
              ** address of the Pairs table (which contains the POWER-Code's unique On-Time/Off-Time pairs)
              ** address of the sequence table (which contains the sequence of indices to the On-Time/Off-Time pairs in the pairs table)
   To transmit a code:
      - Grab the info for this POWER-Code from the 'Code' table
      - Set up hardware timers on the microcontroller for this POWER-Code's Carrier Frequency
      - Go through the 'Sequence' table, one index at a time,
           to transmit each On-Time/Off-Time pair in the 'Pairs' table in the appropriate order


   Example POWER-Code transmission (using code_na000):
      - From the 'Code' table we can see that the Carrier Frequency is 38,400Hz and that there are 26 On-Time/Off-Time pairs.
      - After setting up the microcontroller's timers for the Carrier Frequency, we can start transmitting the POWER-Code.
      - Every POWER-Code is merely a sequence of On-Time/Off-Time pairs.
      - Let's just look at the first three indices from the 'Codes' table
         -- The first index is: 3
               This points to the 4th entry in the 'Pairs' table: 2400, 600
               This means: pulse IR at the Carrier Frequency of 38,400Hz for 2400uS, and then IR off for 600uS.
         -- The second index is: 2
               This points to the 3rd entry in the 'Pairs' table: 1200, 600
               This means: pulse IR at the Carrier Frequency of 38,400Hz for 1200uS, and then IR off for 600uS.
         -- The third index is: 0
               This points to the 1st entry in the 'Pairs' table: 600, 600
               This means: pulse IR at the Carrier Frequency of 38,400Hz for 600uS, and then IR off for 600uS.
      - After going through all 26 On-Time/Off-Time pairs, any TV near you with this POWER-Code will be (pleasantly) OFF.

**************************************************/

// code_na000
//
// table of On-Time/Off-Time pairs
uint32_t code_na000Pairs[] = {
  600, 600,
  600, 27000,
  1200, 600,
  2400, 600,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na000Sequence[] = {
  3, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 1, 3, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na000Code  = {
  38400,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na000Pairs,
  code_na000Sequence
};


// code_na001
//
// table of On-Time/Off-Time pairs
uint32_t code_na001Pairs[] = {
  500, 1000,
  500, 2000,
  500, 8000,
  4000, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na001Sequence[] = {
  3, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 2, 3, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1
};
// POWER-Code table
struct IrCode code_na001Code  = {
  57143,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na001Pairs,
  code_na001Sequence
};


// code_na002
//
// table of On-Time/Off-Time pairs
uint32_t code_na002Pairs[] = {
  420, 460,
  420, 1330,
  420, 75190,
  3470, 1760,
  3470, 1770,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na002Sequence[] = {
  3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 2, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na002Code  = {
  37037,     // carrier frequency
  100,       // number of On-Time/Off-Time pairs in this Off-Code
  code_na002Pairs,
  code_na002Sequence
};


// code_na003
//
// table of On-Time/Off-Time pairs
uint32_t code_na003Pairs[] = {
  260, 1850,
  270, 800,
  270, 1850,
  270, 45490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na003Sequence[] = {
  0, 1, 1, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 3, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 3, 2, 1, 1, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 3, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 1
};
// POWER-Code table
struct IrCode code_na003Code  = {
  38610,     // carrier frequency
  64,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na003Pairs,
  code_na003Sequence
};


// code_na004
//
// table of On-Time/Off-Time pairs
uint32_t code_na004Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na004Sequence[] = {
  5, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na004Code  = {
  38610,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na004Pairs,
  code_na004Sequence
};


// code_na005
//
// table of On-Time/Off-Time pairs
uint32_t code_na005Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  880, 89760,
  1770, 910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na005Sequence[] = {
  0, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 3, 1, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_na005Code  = {
  35714,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na005Pairs,
  code_na005Sequence
};


// code_na006
//
// table of On-Time/Off-Time pairs
uint32_t code_na006Pairs[] = {
  500, 620,
  500, 1720,
  500, 45410,
  4480, 4660,
  4500, 4650,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na006Sequence[] = {
  3, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 4, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_na006Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na006Pairs,
  code_na006Sequence
};


// code_na007
//
// table of On-Time/Off-Time pairs
uint32_t code_na007Pairs[] = {
  490, 490,
  490, 500,
  490, 4100,
  490, 5100,
  490, 121070,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na007Sequence[] = {
  0, 2, 3, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 3, 1, 4, 1, 2, 3, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 3, 1, 0
};
// POWER-Code table
struct IrCode code_na007Code  = {
  39216,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na007Pairs,
  code_na007Sequence
};


// code_na008
//
// table of On-Time/Off-Time pairs
uint32_t code_na008Pairs[] = {
  560, 580,
  560, 1700,
  560, 40110,
  8980, 4500,
  9000, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na008Sequence[] = {
  3, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 2, 4, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1
};
// POWER-Code table
struct IrCode code_na008Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na008Pairs,
  code_na008Sequence
};


// code_na009
//
// table of On-Time/Off-Time pairs
uint32_t code_na009Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na009Sequence[] = {
  4, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_na009Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na009Pairs,
  code_na009Sequence
};


// code_na010
//
// table of On-Time/Off-Time pairs
uint32_t code_na010Pairs[] = {
  510, 550,
  510, 1580,
  510, 22860,
  8410, 4190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na010Sequence[] = {
  3, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na010Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na010Pairs,
  code_na010Sequence
};


// code_na011
//
// table of On-Time/Off-Time pairs
uint32_t code_na011Pairs[] = {
  550, 550,
  550, 1720,
  550, 40390,
  550, 93480,
  560, 0,
  8840, 4420,
  8850, 2250,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na011Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na011Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na011Pairs,
  code_na011Sequence
};


// code_na012
//
// table of On-Time/Off-Time pairs
uint32_t code_na012Pairs[] = {
  810, 870,
  810, 2540,
  810, 32800,
  3310, 3360,
  3310, 3370,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na012Sequence[] = {
  3, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 2, 4, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_na012Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na012Pairs,
  code_na012Sequence
};


// code_na013
//
// table of On-Time/Off-Time pairs
uint32_t code_na013Pairs[] = {
  530, 550,
  530, 1670,
  530, 23040,
  530, 93690,
  8930, 4480,
  8950, 4470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na013Sequence[] = {
  4, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na013Code  = {
  38462,     // carrier frequency
  48,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na013Pairs,
  code_na013Sequence
};


// code_na014
//
// table of On-Time/Off-Time pairs
uint32_t code_na014Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na014Sequence[] = {
  5, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na014Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na014Pairs,
  code_na014Sequence
};


// code_na015
//
// table of On-Time/Off-Time pairs
uint32_t code_na015Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na015Sequence[] = {
  5, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na015Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na015Pairs,
  code_na015Sequence
};


// code_na016
//
// table of On-Time/Off-Time pairs
uint32_t code_na016Pairs[] = {
  280, 900,
  280, 2110,
  280, 25070,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na016Sequence[] = {
  1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na016Code  = {
  34483,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na016Pairs,
  code_na016Sequence
};


// code_na017
//
// table of On-Time/Off-Time pairs
uint32_t code_na017Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na017Sequence[] = {
  5, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na017Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na017Pairs,
  code_na017Sequence
};


// code_na018
//
// table of On-Time/Off-Time pairs
uint32_t code_na018Pairs[] = {
  510, 550,
  510, 1610,
  510, 25660,
  8490, 4290,
  8490, 4300,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na018Sequence[] = {
  3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na018Code  = {
  38462,     // carrier frequency
  136,       // number of On-Time/Off-Time pairs in this Off-Code
  code_na018Pairs,
  code_na018Sequence
};


// code_na019
//
// table of On-Time/Off-Time pairs
uint32_t code_na019Pairs[] = {
  400, 420,
  400, 1240,
  400, 46010,
  3250, 1630,
  3260, 1630,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na019Sequence[] = {
  3, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na019Code  = {
  38462,     // carrier frequency
  100,       // number of On-Time/Off-Time pairs in this Off-Code
  code_na019Pairs,
  code_na019Sequence
};


// code_na020
//
// table of On-Time/Off-Time pairs
uint32_t code_na020Pairs[] = {
  600, 550,
  600, 1630,
  600, 40990,
  600, 96980,
  610, 0,
  8980, 4610,
  9000, 2300,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na020Sequence[] = {
  5, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na020Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na020Pairs,
  code_na020Sequence
};


// code_na021
//
// table of On-Time/Off-Time pairs
uint32_t code_na021Pairs[] = {
  480, 520,
  480, 1600,
  480, 4000,
  480, 23350,
  7990, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na021Sequence[] = {
  4, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 3, 4, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na021Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na021Pairs,
  code_na021Sequence
};


// code_na022
//
// table of On-Time/Off-Time pairs
uint32_t code_na022Pairs[] = {
  530, 600,
  530, 1750,
  530, 44630,
  530, 94530,
  8920, 4500,
  8950, 2250,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na022Sequence[] = {
  4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na022Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na022Pairs,
  code_na022Sequence
};


// code_na023
//
// table of On-Time/Off-Time pairs
uint32_t code_na023Pairs[] = {
  480, 520,
  480, 4090,
  480, 5040,
  480, 104610,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na023Sequence[] = {
  2, 2, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 2, 0, 1, 0, 1, 2, 0, 1, 3, 2, 2, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 2, 0, 1, 0, 1, 2, 0, 1, 2
};
// POWER-Code table
struct IrCode code_na023Code  = {
  40000,     // carrier frequency
  44,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na023Pairs,
  code_na023Sequence
};


// code_na024
//
// table of On-Time/Off-Time pairs
uint32_t code_na024Pairs[] = {
  580, 600,
  580, 25690,
  1180, 600,
  2370, 600,
  2380, 600,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na024Sequence[] = {
  3, 2, 2, 2, 2, 0, 2, 0, 2, 0, 0, 0, 1, 4, 2, 2, 2, 2, 0, 2, 0, 2, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na024Code  = {
  38462,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na024Pairs,
  code_na024Sequence
};


// code_na025
//
// table of On-Time/Off-Time pairs
uint32_t code_na025Pairs[] = {
  840, 900,
  840, 2640,
  840, 34700,
  3460, 3500,
  3470, 3500,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na025Sequence[] = {
  3, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 4, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_na025Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na025Pairs,
  code_na025Sequence
};


// code_na026
//
// table of On-Time/Off-Time pairs
uint32_t code_na026Pairs[] = {
  490, 490,
  490, 500,
  490, 4100,
  490, 5100,
  490, 125820,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na026Sequence[] = {
  0, 2, 3, 1, 2, 1, 2, 3, 3, 1, 2, 3, 1, 2, 3, 1, 4, 1, 2, 3, 1, 2, 1, 2, 3, 3, 1, 2, 3, 1, 2, 3, 1, 0
};
// POWER-Code table
struct IrCode code_na026Code  = {
  39216,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na026Pairs,
  code_na026Sequence
};


// code_na027
//
// table of On-Time/Off-Time pairs
uint32_t code_na027Pairs[] = {
  500, 1000,
  500, 2000,
  500, 8000,
  4000, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na027Sequence[] = {
  3, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na027Code  = {
  57143,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na027Pairs,
  code_na027Sequence
};


// code_na028
//
// table of On-Time/Off-Time pairs
uint32_t code_na028Pairs[] = {
  1180, 1210,
  1180, 2710,
  1180, 47500,
  2580, 2710,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na028Sequence[] = {
  3, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 2, 3, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na028Code  = {
  38610,     // carrier frequency
  36,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na028Pairs,
  code_na028Sequence
};


// code_na029
//
// table of On-Time/Off-Time pairs
uint32_t code_na029Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  1770, 910,
  1770, 89760,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na029Sequence[] = {
  0, 3, 1, 1, 1, 1, 2, 3, 2, 1, 4, 1, 3, 1, 1, 1, 1, 2, 3, 2, 1, 3
};
// POWER-Code table
struct IrCode code_na029Code  = {
  35842,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na029Pairs,
  code_na029Sequence
};


// code_na030
//
// table of On-Time/Off-Time pairs
uint32_t code_na030Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na030Sequence[] = {
  4, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na030Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na030Pairs,
  code_na030Sequence
};


// code_na031
//
// table of On-Time/Off-Time pairs
uint32_t code_na031Pairs[] = {
  880, 890,
  880, 900,
  880, 1790,
  880, 89770,
  1770, 900,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na031Sequence[] = {
  0, 1, 4, 1, 1, 1, 1, 1, 2, 1, 4, 3, 1, 1, 4, 1, 1, 1, 1, 1, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_na031Code  = {
  35842,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na031Pairs,
  code_na031Sequence
};


// code_na032
//
// table of On-Time/Off-Time pairs
uint32_t code_na032Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na032Sequence[] = {
  4, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na032Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na032Pairs,
  code_na032Sequence
};


// code_na033
//
// table of On-Time/Off-Time pairs
uint32_t code_na033Pairs[] = {
  400, 430,
  400, 1220,
  400, 52970,
  3340, 1560,
  3360, 1550,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na033Sequence[] = {
  3, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na033Code  = {
  38462,     // carrier frequency
  100,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na033Pairs,
  code_na033Sequence
};


// code_na034
//
// table of On-Time/Off-Time pairs
uint32_t code_na034Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na034Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na034Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na034Pairs,
  code_na034Sequence
};


// code_na035
//
// table of On-Time/Off-Time pairs
uint32_t code_na035Pairs[] = {
  960, 930,
  970, 930,
  970, 2870,
  970, 34310,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na035Sequence[] = {
  0, 1, 1, 2, 1, 2, 1, 2, 1, 1, 3, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1
};
// POWER-Code table
struct IrCode code_na035Code  = {
  41667,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na035Pairs,
  code_na035Sequence
};


// code_na036
//
// table of On-Time/Off-Time pairs
uint32_t code_na036Pairs[] = {
  820, 5810,
  840, 2500,
  840, 5800,
  850, 0,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na036Sequence[] = {
  0, 1, 1, 1, 2, 1, 2, 2, 2, 1, 3
};
// POWER-Code table
struct IrCode code_na036Code  = {
  37037,     // carrier frequency
  11,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na036Pairs,
  code_na036Sequence
};


// code_na037
//
// table of On-Time/Off-Time pairs
uint32_t code_na037Pairs[] = {
  390, 2630,
  1640, 1630,
  5140, 1640,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na037Sequence[] = {
  2, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na037Code  = {
  41667,     // carrier frequency
  11,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na037Pairs,
  code_na037Sequence
};


// code_na038
//
// table of On-Time/Off-Time pairs
uint32_t code_na038Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na038Sequence[] = {
  5, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na038Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na038Pairs,
  code_na038Sequence
};


// code_na039
//
// table of On-Time/Off-Time pairs
uint32_t code_na039Pairs[] = {
  1130, 1010,
  6880, 27070,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na039Sequence[] = {
  0, 1, 0, 1
};
// POWER-Code table
struct IrCode code_na039Code  = {
  40000,     // carrier frequency
  4,         // number of On-Time/Off-Time pairs in this Off-Code
  code_na039Pairs,
  code_na039Sequence
};


// code_na040
//
// table of On-Time/Off-Time pairs
uint32_t code_na040Pairs[] = {
  1130, 1010,
  1130, 2010,
  1130, 27070,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na040Sequence[] = {
  0, 0, 1, 2, 0, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na040Code  = {
  40000,     // carrier frequency
  8,         // number of On-Time/Off-Time pairs in this Off-Code
  code_na040Pairs,
  code_na040Sequence
};


// code_na041
//
// table of On-Time/Off-Time pairs
uint32_t code_na041Pairs[] = {
  580, 620,
  580, 27460,
  1170, 620,
  2420, 620,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na041Sequence[] = {
  3, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 1, 3, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na041Code  = {
  76923,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na041Pairs,
  code_na041Sequence
};


// code_na042
//
// table of On-Time/Off-Time pairs
uint32_t code_na042Pairs[] = {
  540, 650,
  540, 1700,
  540, 40990,
  540, 86680,
  8990, 2260,
  8990, 4210,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na042Sequence[] = {
  5, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na042Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na042Pairs,
  code_na042Sequence
};


// code_na043
//
// table of On-Time/Off-Time pairs
uint32_t code_na043Pairs[] = {
  430, 1200,
  430, 1210,
  430, 34910,
  1310, 450,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na043Sequence[] = {
  0, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na043Code  = {
  40000,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na043Pairs,
  code_na043Sequence
};


// code_na044
//
// table of On-Time/Off-Time pairs
uint32_t code_na044Pairs[] = {
  510, 510,
  510, 1600,
  510, 40960,
  510, 95130,
  4310, 4360,
  8830, 2190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na044Sequence[] = {
  4, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_na044Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na044Pairs,
  code_na044Sequence
};


// code_na045
//
// table of On-Time/Off-Time pairs
uint32_t code_na045Pairs[] = {
  580, 530,
  580, 1670,
  580, 44940,
  580, 96790,
  4550, 4490,
  4560, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na045Sequence[] = {
  4, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 1, 3, 5, 1, 0
};
// POWER-Code table
struct IrCode code_na045Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na045Pairs,
  code_na045Sequence
};


// code_na046
//
// table of On-Time/Off-Time pairs
uint32_t code_na046Pairs[] = {
  510, 2770,
  520, 530,
  520, 1050,
  520, 2770,
  520, 25270,
  520, 128090,
  1030, 540,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na046Sequence[] = {
  0, 2, 6, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 1, 2
};
// POWER-Code table
struct IrCode code_na046Code  = {
  29412,     // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na046Pairs,
  code_na046Sequence
};


// code_na047
//
// table of On-Time/Off-Time pairs
uint32_t code_na047Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na047Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na047Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na047Pairs,
  code_na047Sequence
};


// code_na048
//
// table of On-Time/Off-Time pairs
uint32_t code_na048Pairs[] = {
  510, 510,
  510, 1600,
  510, 40960,
  510, 95130,
  4310, 4360,
  8830, 2190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na048Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na048Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na048Pairs,
  code_na048Sequence
};


// code_na049
//
// table of On-Time/Off-Time pairs
uint32_t code_na049Pairs[] = {
  2740, 8540,
  2740, 19860,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na049Sequence[] = {
  0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0
};
// POWER-Code table
struct IrCode code_na049Code  = {
  45455,     // carrier frequency
  11,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na049Pairs,
  code_na049Sequence
};


// code_na050
//
// table of On-Time/Off-Time pairs
uint32_t code_na050Pairs[] = {
  800, 880,
  800, 2540,
  800, 37500,
  3590, 3310,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na050Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0
};
// POWER-Code table
struct IrCode code_na050Code  = {
  55556,     // carrier frequency
  48,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na050Pairs,
  code_na050Sequence
};


// code_na051
//
// table of On-Time/Off-Time pairs
uint32_t code_na051Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na051Sequence[] = {
  5, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na051Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na051Pairs,
  code_na051Sequence
};


// code_na052
//
// table of On-Time/Off-Time pairs
uint32_t code_na052Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na052Sequence[] = {
  5, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na052Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na052Pairs,
  code_na052Sequence
};


// code_na053
//
// table of On-Time/Off-Time pairs
uint32_t code_na053Pairs[] = {
  510, 2320,
  510, 5120,
  510, 7920,
  510, 28830,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na053Sequence[] = {
  0, 2, 0, 2, 0, 2, 0, 1, 1, 0, 0, 0, 0, 1, 3, 0, 2, 0, 2, 0, 2, 0, 1, 1, 0, 0, 0, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na053Code  = {
  55556,     // carrier frequency
  30,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na053Pairs,
  code_na053Sequence
};


// code_na054
//
// table of On-Time/Off-Time pairs
uint32_t code_na054Pairs[] = {
  510, 2320,
  510, 5120,
  510, 7920,
  510, 28830,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na054Sequence[] = {
  0, 2, 0, 2, 0, 2, 0, 0, 0, 1, 1, 1, 1, 3, 0, 2, 0, 2, 0, 2, 0, 0, 0, 1, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na054Code  = {
  55556,     // carrier frequency
  28,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na054Pairs,
  code_na054Sequence
};


// code_na055
//
// table of On-Time/Off-Time pairs
uint32_t code_na055Pairs[] = {
  30, 100,
  30, 200,
  30, 300,
  30, 127780,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na055Sequence[] = {
  2, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 2, 3, 2, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1
};
// POWER-Code table
struct IrCode code_na055Code  = {
  0,         // carrier frequency
  27,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na055Pairs,
  code_na055Sequence
};


// code_na056
//
// table of On-Time/Off-Time pairs
uint32_t code_na056Pairs[] = {
  550, 1930,
  570, 1920,
  570, 3840,
  580, 0,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na056Sequence[] = {
  0, 2, 2, 2, 1, 1, 1, 3
};
// POWER-Code table
struct IrCode code_na056Code  = {
  37175,     // carrier frequency
  8,         // number of On-Time/Off-Time pairs in this Off-Code
  code_na056Pairs,
  code_na056Sequence
};


// code_na057
//
// table of On-Time/Off-Time pairs
uint32_t code_na057Pairs[] = {
  450, 1480,
  460, 1480,
  460, 3510,
  460, 27810,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na057Sequence[] = {
  0, 2, 2, 2, 1, 1, 3, 1, 2, 2, 2, 1, 1, 2
};
// POWER-Code table
struct IrCode code_na057Code  = {
  40000,     // carrier frequency
  14,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na057Pairs,
  code_na057Sequence
};


// code_na058
//
// table of On-Time/Off-Time pairs
uint32_t code_na058Pairs[] = {
  220, 1010,
  220, 2190,
  230, 1010,
  230, 2190,
  310, 2180,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na058Sequence[] = {
  4, 3, 3, 2, 2, 0, 1, 0, 0, 1, 0, 0, 2, 2, 2, 2, 2, 3
};
// POWER-Code table
struct IrCode code_na058Code  = {
  33333,     // carrier frequency
  18,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na058Pairs,
  code_na058Sequence
};


// code_na059
//
// table of On-Time/Off-Time pairs
uint32_t code_na059Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na059Sequence[] = {
  5, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na059Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na059Pairs,
  code_na059Sequence
};


// code_na060
//
// table of On-Time/Off-Time pairs
uint32_t code_na060Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na060Sequence[] = {
  5, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na060Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na060Pairs,
  code_na060Sequence
};


// code_na061
//
// table of On-Time/Off-Time pairs
uint32_t code_na061Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na061Sequence[] = {
  5, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na061Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na061Pairs,
  code_na061Sequence
};


// code_na062
//
// table of On-Time/Off-Time pairs
uint32_t code_na062Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na062Sequence[] = {
  5, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na062Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na062Pairs,
  code_na062Sequence
};


// code_na063
//
// table of On-Time/Off-Time pairs
uint32_t code_na063Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na063Sequence[] = {
  5, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na063Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na063Pairs,
  code_na063Sequence
};


// code_na064
//
// table of On-Time/Off-Time pairs
uint32_t code_na064Pairs[] = {
  500, 1000,
  500, 2000,
  500, 8000,
  4000, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na064Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na064Code  = {
  57143,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na064Pairs,
  code_na064Sequence
};


// code_na065
//
// table of On-Time/Off-Time pairs
uint32_t code_na065Pairs[] = {
  480, 980,
  480, 1970,
  980, 8460,
  3950, 3920,
  19530, 3920,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na065Sequence[] = {
  4, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2, 3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2, 3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2
};
// POWER-Code table
struct IrCode code_na065Code  = {
  59172,     // carrier frequency
  78,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na065Pairs,
  code_na065Sequence
};


// code_na066
//
// table of On-Time/Off-Time pairs
uint32_t code_na066Pairs[] = {
  380, 2760,
  1650, 1540,
  4150, 1550,
  7420, 1540,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na066Sequence[] = {
  3, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na066Code  = {
  38462,     // carrier frequency
  33,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na066Pairs,
  code_na066Sequence
};


// code_na067
//
// table of On-Time/Off-Time pairs
uint32_t code_na067Pairs[] = {
  510, 510,
  510, 1600,
  510, 40960,
  510, 95130,
  4310, 4360,
  8830, 2190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na067Sequence[] = {
  4, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na067Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na067Pairs,
  code_na067Sequence
};


// code_na068
//
// table of On-Time/Off-Time pairs
uint32_t code_na068Pairs[] = {
  430, 1210,
  430, 94370,
  1300, 450,
  1310, 450,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na068Sequence[] = {
  2, 0, 3, 0, 0, 3, 0, 0, 0, 0, 3, 1, 3, 0, 3, 0, 0, 3, 0, 0, 0, 0, 3, 0
};
// POWER-Code table
struct IrCode code_na068Code  = {
  40000,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na068Pairs,
  code_na068Sequence
};


// code_na069
//
// table of On-Time/Off-Time pairs
uint32_t code_na069Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na069Sequence[] = {
  5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na069Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na069Pairs,
  code_na069Sequence
};


// code_na070
//
// table of On-Time/Off-Time pairs
uint32_t code_na070Pairs[] = {
  270, 760,
  270, 1820,
  270, 1830,
  270, 31990,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na070Sequence[] = {
  1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2, 3, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 1
};
// POWER-Code table
struct IrCode code_na070Code  = {
  38462,     // carrier frequency
  33,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na070Pairs,
  code_na070Sequence
};


// code_na071
//
// table of On-Time/Off-Time pairs
uint32_t code_na071Pairs[] = {
  370, 1810,
  370, 2720,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na071Sequence[] = {
  0, 1, 0, 1, 1, 0, 0, 0
};
// POWER-Code table
struct IrCode code_na071Code  = {
  55556,     // carrier frequency
  8,         // number of On-Time/Off-Time pairs in this Off-Code
  code_na071Pairs,
  code_na071Sequence
};


// code_na072
//
// table of On-Time/Off-Time pairs
uint32_t code_na072Pairs[] = {
  540, 650,
  540, 1700,
  540, 40990,
  540, 86680,
  8990, 2260,
  8990, 4210,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na072Sequence[] = {
  5, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na072Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na072Pairs,
  code_na072Sequence
};


// code_na073
//
// table of On-Time/Off-Time pairs
uint32_t code_na073Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na073Sequence[] = {
  5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na073Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na073Pairs,
  code_na073Sequence
};


// code_na074
//
// table of On-Time/Off-Time pairs
uint32_t code_na074Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na074Sequence[] = {
  5, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na074Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na074Pairs,
  code_na074Sequence
};


// code_na075
//
// table of On-Time/Off-Time pairs
uint32_t code_na075Pairs[] = {
  510, 980,
  510, 1940,
  1020, 9310,
  3900, 3900,
  3900, 3910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na075Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 2
};
// POWER-Code table
struct IrCode code_na075Code  = {
  41667,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na075Pairs,
  code_na075Sequence
};


// code_na076
//
// table of On-Time/Off-Time pairs
uint32_t code_na076Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na076Sequence[] = {
  5, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na076Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na076Pairs,
  code_na076Sequence
};


// code_na077
//
// table of On-Time/Off-Time pairs
uint32_t code_na077Pairs[] = {
  880, 890,
  880, 900,
  880, 1790,
  880, 89770,
  1770, 900,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na077Sequence[] = {
  0, 4, 1, 2, 1, 1, 4, 2, 1, 4, 3, 1, 4, 1, 2, 1, 1, 4, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_na077Code  = {
  35714,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na077Pairs,
  code_na077Sequence
};


// code_na078
//
// table of On-Time/Off-Time pairs
uint32_t code_na078Pairs[] = {
  400, 2750,
  1600, 1540,
  4800, 1550,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na078Sequence[] = {
  2, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na078Code  = {
  38462,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na078Pairs,
  code_na078Sequence
};


// code_na079
//
// table of On-Time/Off-Time pairs
uint32_t code_na079Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na079Sequence[] = {
  5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na079Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na079Pairs,
  code_na079Sequence
};


// code_na080
//
// table of On-Time/Off-Time pairs
uint32_t code_na080Pairs[] = {
  30, 100,
  30, 200,
  30, 300,
  30, 127780,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na080Sequence[] = {
  2, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 2, 3, 2, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_na080Code  = {
  0,         // carrier frequency
  27,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na080Pairs,
  code_na080Sequence
};


// code_na081
//
// table of On-Time/Off-Time pairs
uint32_t code_na081Pairs[] = {
  480, 520,
  480, 4090,
  480, 5040,
  480, 99780,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na081Sequence[] = {
  0, 1, 2, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 2, 0, 1, 0, 1, 3, 0, 1, 2, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 2, 0, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na081Code  = {
  40000,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na081Pairs,
  code_na081Sequence
};


// code_na082
//
// table of On-Time/Off-Time pairs
uint32_t code_na082Pairs[] = {
  880, 890,
  880, 900,
  880, 1790,
  880, 88880,
  1770, 900,
  1770, 1790,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na082Sequence[] = {
  0, 2, 4, 1, 1, 1, 1, 1, 1, 2, 5, 3, 1, 2, 4, 1, 1, 1, 1, 1, 1, 2, 5, 0
};
// POWER-Code table
struct IrCode code_na082Code  = {
  35714,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na082Pairs,
  code_na082Sequence
};


// code_na083
//
// table of On-Time/Off-Time pairs
uint32_t code_na083Pairs[] = {
  880, 890,
  880, 900,
  880, 1790,
  880, 89770,
  1770, 900,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na083Sequence[] = {
  0, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 3, 1, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_na083Code  = {
  35714,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na083Pairs,
  code_na083Sequence
};


// code_na084
//
// table of On-Time/Off-Time pairs
uint32_t code_na084Pairs[] = {
  410, 430,
  410, 1280,
  410, 74760,
  3360, 1710,
  3380, 1690,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na084Sequence[] = {
  3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 2, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na084Code  = {
  37037,     // carrier frequency
  100,       // number of On-Time/Off-Time pairs in this Off-Code
  code_na084Pairs,
  code_na084Sequence
};


// code_na085
//
// table of On-Time/Off-Time pairs
uint32_t code_na085Pairs[] = {
  550, 600,
  550, 1650,
  550, 22840,
  4450, 4370,
  4480, 4360,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na085Sequence[] = {
  3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na085Code  = {
  38462,     // carrier frequency
  44,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na085Pairs,
  code_na085Sequence
};


// code_na086
//
// table of On-Time/Off-Time pairs
uint32_t code_na086Pairs[] = {
  420, 460,
  420, 1260,
  420, 69890,
  3470, 1760,
  3470, 1770,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na086Sequence[] = {
  3, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na086Code  = {
  37175,     // carrier frequency
  100,       // number of On-Time/Off-Time pairs in this Off-Code
  code_na086Pairs,
  code_na086Sequence
};


// code_na087
//
// table of On-Time/Off-Time pairs
uint32_t code_na087Pairs[] = {
  560, 690,
  560, 1740,
  560, 41650,
  560, 95850,
  8800, 2220,
  8800, 4350,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na087Sequence[] = {
  5, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na087Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na087Pairs,
  code_na087Sequence
};


// code_na088
//
// table of On-Time/Off-Time pairs
uint32_t code_na088Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na088Sequence[] = {
  4, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na088Code  = {
  38610,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na088Pairs,
  code_na088Sequence
};


// code_na089
//
// table of On-Time/Off-Time pairs
uint32_t code_na089Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na089Sequence[] = {
  5, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na089Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na089Pairs,
  code_na089Sequence
};


// code_na090
//
// table of On-Time/Off-Time pairs
uint32_t code_na090Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  880, 89760,
  1770, 910,
  1770, 1810,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na090Sequence[] = {
  0, 4, 1, 2, 5, 4, 2, 1, 4, 3, 1, 4, 1, 2, 5, 4, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_na090Code  = {
  35714,     // carrier frequency
  20,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na090Pairs,
  code_na090Sequence
};


// code_na091
//
// table of On-Time/Off-Time pairs
uint32_t code_na091Pairs[] = {
  480, 1000,
  480, 2000,
  480, 10500,
  4000, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na091Sequence[] = {
  3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2, 3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na091Code  = {
  58824,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na091Pairs,
  code_na091Sequence
};


// code_na092
//
// table of On-Time/Off-Time pairs
uint32_t code_na092Pairs[] = {
  540, 560,
  540, 1700,
  540, 49270,
  4510, 4470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na092Sequence[] = {
  3, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 3, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_na092Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na092Pairs,
  code_na092Sequence
};


// code_na093
//
// table of On-Time/Off-Time pairs
uint32_t code_na093Pairs[] = {
  550, 570,
  550, 1670,
  550, 44000,
  8950, 4480,
  8970, 4470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na093Sequence[] = {
  3, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 2, 4, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na093Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na093Pairs,
  code_na093Sequence
};


// code_na094
//
// table of On-Time/Off-Time pairs
uint32_t code_na094Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  880, 89760,
  1770, 910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na094Sequence[] = {
  0, 4, 1, 1, 2, 1, 4, 2, 1, 4, 3, 1, 4, 1, 1, 2, 1, 4, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_na094Code  = {
  35714,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na094Pairs,
  code_na094Sequence
};


// code_na095
//
// table of On-Time/Off-Time pairs
uint32_t code_na095Pairs[] = {
  560, 580,
  560, 1740,
  560, 45490,
  560, 94480,
  4400, 4460,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na095Sequence[] = {
  4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 2, 4, 1, 3, 4, 1, 0
};
// POWER-Code table
struct IrCode code_na095Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na095Pairs,
  code_na095Sequence
};


// code_na096
//
// table of On-Time/Off-Time pairs
uint32_t code_na096Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na096Sequence[] = {
  4, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na096Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na096Pairs,
  code_na096Sequence
};


// code_na097
//
// table of On-Time/Off-Time pairs
uint32_t code_na097Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na097Sequence[] = {
  4, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_na097Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na097Pairs,
  code_na097Sequence
};


// code_na098
//
// table of On-Time/Off-Time pairs
uint32_t code_na098Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na098Sequence[] = {
  5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na098Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na098Pairs,
  code_na098Sequence
};


// code_na099
//
// table of On-Time/Off-Time pairs
uint32_t code_na099Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na099Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na099Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na099Pairs,
  code_na099Sequence
};


// code_na100
//
// table of On-Time/Off-Time pairs
uint32_t code_na100Pairs[] = {
  430, 1710,
  450, 600,
  450, 1700,
  540, 23010,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na100Sequence[] = {
  0, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 3, 2, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 3
};
// POWER-Code table
struct IrCode code_na100Code  = {
  35842,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na100Pairs,
  code_na100Sequence
};


// code_na101
//
// table of On-Time/Off-Time pairs
uint32_t code_na101Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na101Sequence[] = {
  5, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na101Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na101Pairs,
  code_na101Sequence
};


// code_na102
//
// table of On-Time/Off-Time pairs
uint32_t code_na102Pairs[] = {
  860, 870,
  860, 2580,
  860, 33380,
  3460, 3480,
  3480, 3470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na102Sequence[] = {
  3, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 2, 4, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1
};
// POWER-Code table
struct IrCode code_na102Code  = {
  40000,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na102Pairs,
  code_na102Sequence
};


// code_na103
//
// table of On-Time/Off-Time pairs
uint32_t code_na103Pairs[] = {
  580, 530,
  580, 1670,
  580, 44940,
  580, 96790,
  4550, 4490,
  4560, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na103Sequence[] = {
  4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 1, 3, 5, 1, 0
};
// POWER-Code table
struct IrCode code_na103Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na103Pairs,
  code_na103Sequence
};


// code_na104
//
// table of On-Time/Off-Time pairs
uint32_t code_na104Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na104Sequence[] = {
  5, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na104Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na104Pairs,
  code_na104Sequence
};


// code_na105
//
// table of On-Time/Off-Time pairs
uint32_t code_na105Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na105Sequence[] = {
  5, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na105Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na105Pairs,
  code_na105Sequence
};


// code_na106
//
// table of On-Time/Off-Time pairs
uint32_t code_na106Pairs[] = {
  510, 510,
  510, 1600,
  510, 40960,
  510, 95130,
  4310, 4360,
  8830, 2190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na106Sequence[] = {
  4, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na106Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na106Pairs,
  code_na106Sequence
};


// code_na107
//
// table of On-Time/Off-Time pairs
uint32_t code_na107Pairs[] = {
  580, 530,
  580, 1670,
  580, 44940,
  580, 96790,
  4550, 4490,
  4560, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na107Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 1, 3, 5, 1, 0
};
// POWER-Code table
struct IrCode code_na107Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na107Pairs,
  code_na107Sequence
};


// code_na108
//
// table of On-Time/Off-Time pairs
uint32_t code_na108Pairs[] = {
  580, 530,
  580, 1670,
  580, 44940,
  580, 96790,
  4550, 4490,
  4560, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na108Sequence[] = {
  4, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 1, 3, 5, 1, 0
};
// POWER-Code table
struct IrCode code_na108Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na108Pairs,
  code_na108Sequence
};


// code_na109
//
// table of On-Time/Off-Time pairs
uint32_t code_na109Pairs[] = {
  580, 610,
  580, 2110,
  580, 95820,
  730, 41640,
  8830, 2110,
  10500, 4940,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na109Sequence[] = {
  5, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 3, 4, 2, 4, 0
};
// POWER-Code table
struct IrCode code_na109Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na109Pairs,
  code_na109Sequence
};


// code_na110
//
// table of On-Time/Off-Time pairs
uint32_t code_na110Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na110Sequence[] = {
  5, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na110Code  = {
  40161,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na110Pairs,
  code_na110Sequence
};


// code_na111
//
// table of On-Time/Off-Time pairs
uint32_t code_na111Pairs[] = {
  510, 510,
  510, 1600,
  510, 40960,
  510, 95130,
  4310, 4360,
  8830, 2190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na111Sequence[] = {
  4, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_na111Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na111Pairs,
  code_na111Sequence
};


// code_na112
//
// table of On-Time/Off-Time pairs
uint32_t code_na112Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na112Sequence[] = {
  5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_na112Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na112Pairs,
  code_na112Sequence
};


// code_na113
//
// table of On-Time/Off-Time pairs
uint32_t code_na113Pairs[] = {
  560, 540,
  560, 1660,
  560, 39450,
  8960, 4420,
  8960, 4430,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na113Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_na113Code  = {
  40000,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na113Pairs,
  code_na113Sequence
};


// code_na114
//
// table of On-Time/Off-Time pairs
uint32_t code_na114Pairs[] = {
  440, 500,
  440, 1470,
  440, 4470,
  440, 22360,
  7910, 3980,
  7930, 3970,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na114Sequence[] = {
  4, 1, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 3, 5, 1, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na114Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na114Pairs,
  code_na114Sequence
};


// code_na115
//
// table of On-Time/Off-Time pairs
uint32_t code_na115Pairs[] = {
  810, 860,
  810, 2960,
  810, 33490,
  3280, 3310,
  3290, 3310,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na115Sequence[] = {
  3, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 2, 4, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na115Code  = {
  40000,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na115Pairs,
  code_na115Sequence
};


// code_na116
//
// table of On-Time/Off-Time pairs
uint32_t code_na116Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na116Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na116Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na116Pairs,
  code_na116Sequence
};


// code_na117
//
// table of On-Time/Off-Time pairs
uint32_t code_na117Pairs[] = {
  490, 540,
  490, 1580,
  490, 4200,
  490, 24460,
  8190, 4200,
  8210, 4190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na117Sequence[] = {
  4, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0, 3, 5, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_na117Code  = {
  41667,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na117Pairs,
  code_na117Sequence
};


// code_na118
//
// table of On-Time/Off-Time pairs
uint32_t code_na118Pairs[] = {
  510, 510,
  510, 1600,
  510, 40960,
  510, 95130,
  4310, 4360,
  8830, 2190,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na118Sequence[] = {
  4, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_na118Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na118Pairs,
  code_na118Sequence
};


// code_na119
//
// table of On-Time/Off-Time pairs
uint32_t code_na119Pairs[] = {
  550, 630,
  550, 1710,
  550, 40940,
  550, 95080,
  8810, 2190,
  8810, 4380,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na119Sequence[] = {
  5, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na119Code  = {
  55556,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na119Pairs,
  code_na119Sequence
};


// code_na120
//
// table of On-Time/Off-Time pairs
uint32_t code_na120Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na120Sequence[] = {
  5, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na120Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na120Pairs,
  code_na120Sequence
};


// code_na121
//
// table of On-Time/Off-Time pairs
uint32_t code_na121Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na121Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na121Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na121Pairs,
  code_na121Sequence
};


// code_na122
//
// table of On-Time/Off-Time pairs
uint32_t code_na122Pairs[] = {
  800, 950,
  800, 2490,
  800, 38670,
  810, 0,
  3290, 3220,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na122Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 3
};
// POWER-Code table
struct IrCode code_na122Code  = {
  52632,     // carrier frequency
  48,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na122Pairs,
  code_na122Sequence
};


// code_na123
//
// table of On-Time/Off-Time pairs
uint32_t code_na123Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na123Sequence[] = {
  5, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na123Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na123Pairs,
  code_na123Sequence
};


// code_na124
//
// table of On-Time/Off-Time pairs
uint32_t code_na124Pairs[] = {
  540, 560,
  540, 1510,
  540, 40920,
  540, 86770,
  9000, 4210,
  9010, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na124Sequence[] = {
  4, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_na124Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na124Pairs,
  code_na124Sequence
};


// code_na125
//
// table of On-Time/Off-Time pairs
uint32_t code_na125Pairs[] = {
  550, 630,
  550, 1710,
  550, 40940,
  550, 95080,
  8810, 2190,
  8810, 4380,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na125Sequence[] = {
  5, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na125Code  = {
  55556,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na125Pairs,
  code_na125Sequence
};


// code_na126
//
// table of On-Time/Off-Time pairs
uint32_t code_na126Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na126Sequence[] = {
  5, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na126Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na126Pairs,
  code_na126Sequence
};


// code_na127
//
// table of On-Time/Off-Time pairs
uint32_t code_na127Pairs[] = {
  1140, 1000,
  1150, 1000,
  1150, 2000,
  1150, 27060,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na127Sequence[] = {
  0, 1, 2, 3, 1, 1, 2, 1
};
// POWER-Code table
struct IrCode code_na127Code  = {
  25641,     // carrier frequency
  8,         // number of On-Time/Off-Time pairs in this Off-Code
  code_na127Pairs,
  code_na127Sequence
};


// code_na128
//
// table of On-Time/Off-Time pairs
uint32_t code_na128Pairs[] = {
  860, 870,
  860, 2580,
  860, 33380,
  3460, 3480,
  3480, 3470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na128Sequence[] = {
  3, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 2, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0
};
// POWER-Code table
struct IrCode code_na128Code  = {
  40000,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na128Pairs,
  code_na128Sequence
};


// code_na129
//
// table of On-Time/Off-Time pairs
uint32_t code_na129Pairs[] = {
  560, 570,
  560, 1750,
  560, 41500,
  560, 94990,
  8980, 2270,
  8980, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na129Sequence[] = {
  5, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 4, 3, 4, 1
};
// POWER-Code table
struct IrCode code_na129Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na129Pairs,
  code_na129Sequence
};


// code_na130
//
// table of On-Time/Off-Time pairs
uint32_t code_na130Pairs[] = {
  880, 900,
  880, 2580,
  880, 22470,
  3580, 3490,
  3580, 3500,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na130Sequence[] = {
  3, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 2, 4, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1
};
// POWER-Code table
struct IrCode code_na130Code  = {
  37037,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na130Pairs,
  code_na130Sequence
};


// code_na131
//
// table of On-Time/Off-Time pairs
uint32_t code_na131Pairs[] = {
  540, 650,
  540, 1700,
  540, 40990,
  540, 86680,
  8990, 2260,
  8990, 4210,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na131Sequence[] = {
  5, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 2, 4, 3, 4, 0
};
// POWER-Code table
struct IrCode code_na131Code  = {
  40000,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na131Pairs,
  code_na131Sequence
};


// code_na132
//
// table of On-Time/Off-Time pairs
uint32_t code_na132Pairs[] = {
  280, 1060,
  280, 2380,
  280, 3700,
  280, 11730,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na132Sequence[] = {
  0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na132Code  = {
  83333,     // carrier frequency
  32,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na132Pairs,
  code_na132Sequence
};


// code_na133
//
// table of On-Time/Off-Time pairs
uint32_t code_na133Pairs[] = {
  130, 7410,
  150, 4890,
  150, 7400,
  170, 46410,
  180, 0,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na133Sequence[] = {
  0, 2, 2, 2, 2, 1, 1, 1, 2, 2, 1, 3, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 1, 4
};
// POWER-Code table
struct IrCode code_na133Code  = {
  41667,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na133Pairs,
  code_na133Sequence
};


// code_na134
//
// table of On-Time/Off-Time pairs
uint32_t code_na134Pairs[] = {
  560, 540,
  560, 1660,
  560, 39450,
  8960, 4420,
  8960, 4430,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na134Sequence[] = {
  3, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 4, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na134Code  = {
  40000,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na134Pairs,
  code_na134Sequence
};


// code_na135
//
// table of On-Time/Off-Time pairs
uint32_t code_na135Pairs[] = {
  530, 590,
  530, 1710,
  530, 23010,
  8920, 4500,
  8950, 4480,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na135Sequence[] = {
  3, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_na135Code  = {
  38462,     // carrier frequency
  88,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na135Pairs,
  code_na135Sequence
};


// code_na136
//
// table of On-Time/Off-Time pairs
uint32_t code_na136Pairs[] = {
  530, 590,
  530, 1710,
  530, 23010,
  550, 0,
  8920, 4500,
  8950, 4480,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_na136Sequence[] = {
  4, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3
};
// POWER-Code table
struct IrCode code_na136Code  = {
  38610,     // carrier frequency
  88,        // number of On-Time/Off-Time pairs in this Off-Code
  code_na136Pairs,
  code_na136Sequence
};


// code_eu000
//
// table of On-Time/Off-Time pairs
uint32_t code_eu000Pairs[] = {
  430, 470,
  430, 910,
  430, 83240,
  880, 470,
  1330, 1330,
  2640, 900,
  2640, 910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu000Sequence[] = {
  5, 1, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 2, 6, 1, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 1
};
// POWER-Code table
struct IrCode code_eu000Code  = {
  35714,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu000Pairs,
  code_eu000Sequence
};


// code_eu001
//
// table of On-Time/Off-Time pairs
uint32_t code_eu001Pairs[] = {
  470, 2650,
  510, 540,
  510, 1080,
  510, 2630,
  510, 20530,
  510, 116470,
  1000, 1090,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu001Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 6, 1, 1, 1, 1, 1, 1, 5, 3, 2, 6, 1, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu001Code  = {
  30303,     // carrier frequency
  31,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu001Pairs,
  code_eu001Sequence
};


// code_eu002
//
// table of On-Time/Off-Time pairs
uint32_t code_eu002Pairs[] = {
  430, 2060,
  460, 2040,
  460, 4560,
  460, 34880,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu002Sequence[] = {
  0, 1, 2, 2, 1, 1, 1, 2, 2, 2, 1, 2, 3, 1, 1, 2, 2, 1, 1, 1, 2, 2, 2, 1, 2, 1
};
// POWER-Code table
struct IrCode code_eu002Code  = {
  33333,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu002Pairs,
  code_eu002Sequence
};


// code_eu003
//
// table of On-Time/Off-Time pairs
uint32_t code_eu003Pairs[] = {
  580, 600,
  580, 26870,
  1180, 600,
  2370, 600,
  2380, 600,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu003Sequence[] = {
  3, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 1, 4, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_eu003Code  = {
  38462,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu003Pairs,
  code_eu003Sequence
};


// code_eu004
//
// table of On-Time/Off-Time pairs
uint32_t code_eu004Pairs[] = {
  440, 450,
  440, 1310,
  440, 74620,
  3460, 1760,
  3460, 1780,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu004Sequence[] = {
  3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 2, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_eu004Code  = {
  37037,     // carrier frequency
  100,       // number of On-Time/Off-Time pairs in this Off-Code
  code_eu004Pairs,
  code_eu004Sequence
};


// code_eu005
//
// table of On-Time/Off-Time pairs
uint32_t code_eu005Pairs[] = {
  240, 1900,
  250, 800,
  250, 1900,
  250, 41990,
  250, 47990,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu005Sequence[] = {
  0, 1, 1, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 4, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 3, 2, 1, 1, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 4, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 1
};
// POWER-Code table
struct IrCode code_eu005Code  = {
  38610,     // carrier frequency
  64,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu005Pairs,
  code_eu005Sequence
};


// code_eu006
//
// table of On-Time/Off-Time pairs
uint32_t code_eu006Pairs[] = {
  530, 630,
  530, 1720,
  530, 44720,
  540, 0,
  4550, 4680,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu006Sequence[] = {
  4, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 4, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 3
};
// POWER-Code table
struct IrCode code_eu006Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu006Pairs,
  code_eu006Sequence
};


// code_eu007
//
// table of On-Time/Off-Time pairs
uint32_t code_eu007Pairs[] = {
  500, 540,
  500, 1590,
  500, 23070,
  8380, 4220,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu007Sequence[] = {
  3, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu007Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu007Pairs,
  code_eu007Sequence
};


// code_eu008
//
// table of On-Time/Off-Time pairs
uint32_t code_eu008Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu008Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu008Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu008Pairs,
  code_eu008Sequence
};


// code_eu009
//
// table of On-Time/Off-Time pairs
uint32_t code_eu009Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  880, 89760,
  1770, 910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu009Sequence[] = {
  0, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 3, 1, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_eu009Code  = {
  35714,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu009Pairs,
  code_eu009Sequence
};


// code_eu010
//
// table of On-Time/Off-Time pairs
uint32_t code_eu010Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu010Sequence[] = {
  5, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu010Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu010Pairs,
  code_eu010Sequence
};


// code_eu011
//
// table of On-Time/Off-Time pairs
uint32_t code_eu011Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu011Sequence[] = {
  4, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_eu011Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu011Pairs,
  code_eu011Sequence
};


// code_eu012
//
// table of On-Time/Off-Time pairs
uint32_t code_eu012Pairs[] = {
  460, 2060,
  460, 4590,
  460, 34470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu012Sequence[] = {
  0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 2, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_eu012Code  = {
  33445,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu012Pairs,
  code_eu012Sequence
};


// code_eu013
//
// table of On-Time/Off-Time pairs
uint32_t code_eu013Pairs[] = {
  530, 590,
  530, 1710,
  530, 23020,
  8950, 4490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu013Sequence[] = {
  3, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu013Code  = {
  38462,     // carrier frequency
  88,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu013Pairs,
  code_eu013Sequence
};


// code_eu014
//
// table of On-Time/Off-Time pairs
uint32_t code_eu014Pairs[] = {
  480, 520,
  480, 1600,
  480, 4000,
  480, 23350,
  7990, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu014Sequence[] = {
  4, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 3, 4, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_eu014Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu014Pairs,
  code_eu014Sequence
};


// code_eu015
//
// table of On-Time/Off-Time pairs
uint32_t code_eu015Pairs[] = {
  530, 540,
  530, 1560,
  530, 25420,
  8510, 4250,
  8530, 4240,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu015Sequence[] = {
  3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu015Code  = {
  38462,     // carrier frequency
  136,       // number of On-Time/Off-Time pairs in this Off-Code
  code_eu015Pairs,
  code_eu015Sequence
};


// code_eu016
//
// table of On-Time/Off-Time pairs
uint32_t code_eu016Pairs[] = {
  280, 920,
  280, 2130,
  280, 2140,
  280, 27710,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu016Sequence[] = {
  1, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu016Code  = {
  33333,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu016Pairs,
  code_eu016Sequence
};


// code_eu017
//
// table of On-Time/Off-Time pairs
uint32_t code_eu017Pairs[] = {
  150, 8440,
  160, 5570,
  160, 8440,
  160, 52240,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu017Sequence[] = {
  0, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 3, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1
};
// POWER-Code table
struct IrCode code_eu017Code  = {
  33333,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu017Pairs,
  code_eu017Sequence
};


// code_eu018
//
// table of On-Time/Off-Time pairs
uint32_t code_eu018Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu018Sequence[] = {
  5, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu018Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu018Pairs,
  code_eu018Sequence
};


// code_eu019
//
// table of On-Time/Off-Time pairs
uint32_t code_eu019Pairs[] = {
  500, 540,
  500, 1580,
  500, 4180,
  500, 24430,
  8430, 4180,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu019Sequence[] = {
  4, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0, 3, 4, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_eu019Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu019Pairs,
  code_eu019Sequence
};


// code_eu020
//
// table of On-Time/Off-Time pairs
uint32_t code_eu020Pairs[] = {
  480, 3010,
  480, 6510,
  480, 10010,
  480, 30010,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu020Sequence[] = {
  0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
};
// POWER-Code table
struct IrCode code_eu020Code  = {
  35714,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu020Pairs,
  code_eu020Sequence
};


// code_eu021
//
// table of On-Time/Off-Time pairs
uint32_t code_eu021Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu021Sequence[] = {
  4, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_eu021Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu021Pairs,
  code_eu021Sequence
};


// code_eu022
//
// table of On-Time/Off-Time pairs
uint32_t code_eu022Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu022Sequence[] = {
  5, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu022Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu022Pairs,
  code_eu022Sequence
};


// code_eu023
//
// table of On-Time/Off-Time pairs
uint32_t code_eu023Pairs[] = {
  530, 600,
  530, 1750,
  530, 44630,
  530, 94530,
  8920, 4500,
  8950, 2250,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu023Sequence[] = {
  4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_eu023Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu023Pairs,
  code_eu023Sequence
};


// code_eu024
//
// table of On-Time/Off-Time pairs
uint32_t code_eu024Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu024Sequence[] = {
  5, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu024Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu024Pairs,
  code_eu024Sequence
};


// code_eu025
//
// table of On-Time/Off-Time pairs
uint32_t code_eu025Pairs[] = {
  490, 520,
  490, 1020,
  490, 2500,
  490, 2520,
  490, 23770,
  490, 120090,
  1000, 520,
  1000, 1020,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu025Sequence[] = {
  2, 1, 6, 0, 0, 0, 4, 3, 1, 7, 0, 0, 0, 5, 3, 1, 7, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_eu025Code  = {
  31250,     // carrier frequency
  21,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu025Pairs,
  code_eu025Sequence
};


// code_eu026
//
// table of On-Time/Off-Time pairs
uint32_t code_eu026Pairs[] = {
  140, 4910,
  140, 7430,
  140, 49260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu026Sequence[] = {
  1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu026Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu026Pairs,
  code_eu026Sequence
};


// code_eu027
//
// table of On-Time/Off-Time pairs
uint32_t code_eu027Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu027Sequence[] = {
  5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu027Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu027Pairs,
  code_eu027Sequence
};


// code_eu028
//
// table of On-Time/Off-Time pairs
uint32_t code_eu028Pairs[] = {
  470, 2670,
  500, 550,
  500, 1100,
  500, 2650,
  500, 20550,
  500, 121170,
  1000, 570,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu028Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 6, 2, 1, 1, 1, 1, 5, 3, 2, 1, 6, 2, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu028Code  = {
  30303,     // carrier frequency
  31,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu028Pairs,
  code_eu028Sequence
};


// code_eu029
//
// table of On-Time/Off-Time pairs
uint32_t code_eu029Pairs[] = {
  500, 500,
  500, 990,
  500, 2510,
  500, 2520,
  500, 14450,
  500, 110140,
  1020, 490,
  1020, 980,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu029Sequence[] = {
  2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 0, 6, 1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 5, 3, 1, 0, 6, 1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_eu029Code  = {
  34483,     // carrier frequency
  46,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu029Pairs,
  code_eu029Sequence
};


// code_eu030
//
// table of On-Time/Off-Time pairs
uint32_t code_eu030Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu030Sequence[] = {
  5, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu030Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu030Pairs,
  code_eu030Sequence
};


// code_eu031
//
// table of On-Time/Off-Time pairs
uint32_t code_eu031Pairs[] = {
  530, 530,
  530, 1600,
  530, 16970,
  8380, 4220,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu031Sequence[] = {
  3, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu031Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu031Pairs,
  code_eu031Sequence
};


// code_eu032
//
// table of On-Time/Off-Time pairs
uint32_t code_eu032Pairs[] = {
  490, 2050,
  490, 2060,
  490, 4560,
  490, 36900,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu032Sequence[] = {
  0, 1, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 3, 1, 1, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu032Code  = {
  33333,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu032Pairs,
  code_eu032Sequence
};


// code_eu033
//
// table of On-Time/Off-Time pairs
uint32_t code_eu033Pairs[] = {
  480, 1500,
  500, 1490,
  500, 3470,
  500, 29360,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu033Sequence[] = {
  0, 2, 2, 2, 1, 1, 3, 1, 2, 2, 2, 1, 1, 2
};
// POWER-Code table
struct IrCode code_eu033Code  = {
  38462,     // carrier frequency
  14,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu033Pairs,
  code_eu033Sequence
};


// code_eu034
//
// table of On-Time/Off-Time pairs
uint32_t code_eu034Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu034Sequence[] = {
  5, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu034Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu034Pairs,
  code_eu034Sequence
};


// code_eu035
//
// table of On-Time/Off-Time pairs
uint32_t code_eu035Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  880, 89760,
  1770, 910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu035Sequence[] = {
  0, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 3, 1, 4, 1, 1, 1, 1, 1, 1, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_eu035Code  = {
  35714,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu035Pairs,
  code_eu035Sequence
};


// code_eu036
//
// table of On-Time/Off-Time pairs
uint32_t code_eu036Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu036Sequence[] = {
  5, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu036Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu036Pairs,
  code_eu036Sequence
};


// code_eu037
//
// table of On-Time/Off-Time pairs
uint32_t code_eu037Pairs[] = {
  140, 4910,
  140, 7430,
  140, 51780,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu037Sequence[] = {
  1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu037Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu037Pairs,
  code_eu037Sequence
};


// code_eu038
//
// table of On-Time/Off-Time pairs
uint32_t code_eu038Pairs[] = {
  30, 10020,
  30, 14950,
  30, 30590,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu038Sequence[] = {
  0, 0, 1, 1, 1, 2, 0, 0, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu038Code  = {
  0,         // carrier frequency
  11,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu038Pairs,
  code_eu038Sequence
};


// code_eu039
//
// table of On-Time/Off-Time pairs
uint32_t code_eu039Pairs[] = {
  130, 4450,
  130, 6740,
  130, 6750,
  130, 45830,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu039Sequence[] = {
  1, 2, 2, 2, 2, 0, 0, 2, 2, 0, 0, 3, 2, 2, 2, 2, 2, 0, 0, 2, 2, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu039Code  = {
  40161,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu039Pairs,
  code_eu039Sequence
};


// code_eu040
//
// table of On-Time/Off-Time pairs
uint32_t code_eu040Pairs[] = {
  850, 890,
  850, 2640,
  850, 34020,
  3470, 3500,
  3480, 3500,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu040Sequence[] = {
  3, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 2, 4, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu040Code  = {
  35714,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu040Pairs,
  code_eu040Sequence
};


// code_eu041
//
// table of On-Time/Off-Time pairs
uint32_t code_eu041Pairs[] = {
  460, 3000,
  490, 2980,
  490, 6480,
  490, 9970,
  490, 30560,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu041Sequence[] = {
  0, 3, 1, 3, 1, 3, 1, 2, 2, 2, 2, 1, 1, 4, 1, 3, 1, 3, 1, 3, 1, 2, 2, 2, 2, 1, 1, 3
};
// POWER-Code table
struct IrCode code_eu041Code  = {
  33333,     // carrier frequency
  28,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu041Pairs,
  code_eu041Sequence
};


// code_eu042
//
// table of On-Time/Off-Time pairs
uint32_t code_eu042Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu042Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_eu042Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu042Pairs,
  code_eu042Sequence
};


// code_eu043
//
// table of On-Time/Off-Time pairs
uint32_t code_eu043Pairs[] = {
  10370, 42160,
  10400, 0,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu043Sequence[] = {
  0, 1
};
// POWER-Code table
struct IrCode code_eu043Code  = {
  41667,     // carrier frequency
  2,         // number of On-Time/Off-Time pairs in this Off-Code
  code_eu043Pairs,
  code_eu043Sequence
};


// code_eu044
//
// table of On-Time/Off-Time pairs
uint32_t code_eu044Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu044Sequence[] = {
  5, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu044Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu044Pairs,
  code_eu044Sequence
};


// code_eu045
//
// table of On-Time/Off-Time pairs
uint32_t code_eu045Pairs[] = {
  1520, 4710,
  1540, 1560,
  1540, 4690,
  1540, 29470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu045Sequence[] = {
  0, 1, 1, 2, 3, 2, 1, 1, 2, 1
};
// POWER-Code table
struct IrCode code_eu045Code  = {
  41667,     // carrier frequency
  10,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu045Pairs,
  code_eu045Sequence
};


// code_eu046
//
// table of On-Time/Off-Time pairs
uint32_t code_eu046Pairs[] = {
  150, 4930,
  160, 4930,
  160, 6980,
  160, 14140,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu046Sequence[] = {
  0, 1, 1, 2, 2, 2, 2, 3, 1, 1, 1, 2, 2, 2, 2, 1
};
// POWER-Code table
struct IrCode code_eu046Code  = {
  34602,     // carrier frequency
  16,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu046Pairs,
  code_eu046Sequence
};


// code_eu047
//
// table of On-Time/Off-Time pairs
uint32_t code_eu047Pairs[] = {
  30, 4960,
  30, 7450,
  30, 14880,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu047Sequence[] = {
  1, 0, 0, 1, 0, 2, 1, 0, 0, 1, 0, 2, 1, 0, 0, 1, 0
};
// POWER-Code table
struct IrCode code_eu047Code  = {
  0,         // carrier frequency
  17,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu047Pairs,
  code_eu047Sequence
};


// code_eu048
//
// table of On-Time/Off-Time pairs
uint32_t code_eu048Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu048Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_eu048Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu048Pairs,
  code_eu048Sequence
};


// code_eu049
//
// table of On-Time/Off-Time pairs
uint32_t code_eu049Pairs[] = {
  550, 550,
  550, 1670,
  550, 45770,
  550, 95060,
  4480, 4450,
  4500, 4440,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu049Sequence[] = {
  4, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 2, 5, 1, 3, 5, 1, 0
};
// POWER-Code table
struct IrCode code_eu049Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu049Pairs,
  code_eu049Sequence
};


// code_eu050
//
// table of On-Time/Off-Time pairs
uint32_t code_eu050Pairs[] = {
  910, 880,
  910, 2670,
  910, 36210,
  3610, 3580,
  3610, 3590,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu050Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0
};
// POWER-Code table
struct IrCode code_eu050Code  = {
  33333,     // carrier frequency
  48,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu050Pairs,
  code_eu050Sequence
};


// code_eu051
//
// table of On-Time/Off-Time pairs
uint32_t code_eu051Pairs[] = {
  840, 880,
  840, 2610,
  840, 33600,
  3470, 3470,
  3470, 3480,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu051Sequence[] = {
  3, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 2, 4, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu051Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu051Pairs,
  code_eu051Sequence
};


// code_eu052
//
// table of On-Time/Off-Time pairs
uint32_t code_eu052Pairs[] = {
  160, 8380,
  170, 5580,
  170, 8390,
  170, 63280,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu052Sequence[] = {
  0, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 3, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1
};
// POWER-Code table
struct IrCode code_eu052Code  = {
  31250,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu052Pairs,
  code_eu052Sequence
};


// code_eu053
//
// table of On-Time/Off-Time pairs
uint32_t code_eu053Pairs[] = {
  150, 4930,
  160, 4930,
  160, 6980,
  160, 14140,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu053Sequence[] = {
  0, 2, 1, 2, 2, 2, 2, 3, 1, 2, 1, 2, 2, 2, 2, 2
};
// POWER-Code table
struct IrCode code_eu053Code  = {
  34483,     // carrier frequency
  16,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu053Pairs,
  code_eu053Sequence
};


// code_eu054
//
// table of On-Time/Off-Time pairs
uint32_t code_eu054Pairs[] = {
  490, 530,
  490, 1040,
  490, 2620,
  490, 2640,
  490, 80300,
  1000, 1030,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu054Sequence[] = {
  2, 0, 0, 1, 5, 0, 4, 3, 0, 0, 1, 5, 0, 2
};
// POWER-Code table
struct IrCode code_eu054Code  = {
  31250,     // carrier frequency
  14,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu054Pairs,
  code_eu054Sequence
};


// code_eu055
//
// table of On-Time/Off-Time pairs
uint32_t code_eu055Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu055Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_eu055Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu055Pairs,
  code_eu055Sequence
};


// code_eu056
//
// table of On-Time/Off-Time pairs
uint32_t code_eu056Pairs[] = {
  1120, 1070,
  1130, 1070,
  6770, 27660,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu056Sequence[] = {
  0, 2, 1, 2
};
// POWER-Code table
struct IrCode code_eu056Code  = {
  38462,     // carrier frequency
  4,         // number of On-Time/Off-Time pairs in this Off-Code
  code_eu056Pairs,
  code_eu056Sequence
};


// code_eu057
//
// table of On-Time/Off-Time pairs
uint32_t code_eu057Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu057Sequence[] = {
  5, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu057Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu057Pairs,
  code_eu057Sequence
};


// code_eu058
//
// table of On-Time/Off-Time pairs
uint32_t code_eu058Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu058Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_eu058Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu058Pairs,
  code_eu058Sequence
};


// code_eu059
//
// table of On-Time/Off-Time pairs
uint32_t code_eu059Pairs[] = {
  3100, 6130,
  3100, 6140,
  6220, 83120,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu059Sequence[] = {
  0, 2, 1, 2
};
// POWER-Code table
struct IrCode code_eu059Code  = {
  41667,     // carrier frequency
  4,         // number of On-Time/Off-Time pairs in this Off-Code
  code_eu059Pairs,
  code_eu059Sequence
};


// code_eu060
//
// table of On-Time/Off-Time pairs
uint32_t code_eu060Pairs[] = {
  500, 1580,
  530, 510,
  530, 1560,
  530, 21800,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu060Sequence[] = {
  0, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2, 3, 2, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2, 2
};
// POWER-Code table
struct IrCode code_eu060Code  = {
  38462,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu060Pairs,
  code_eu060Sequence
};


// code_eu061
//
// table of On-Time/Off-Time pairs
uint32_t code_eu061Pairs[] = {
  880, 900,
  880, 910,
  880, 1810,
  880, 89760,
  1770, 910,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu061Sequence[] = {
  0, 4, 1, 1, 1, 1, 2, 4, 1, 1, 1, 3, 1, 4, 1, 1, 1, 1, 2, 4, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu061Code  = {
  35714,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu061Pairs,
  code_eu061Sequence
};


// code_eu062
//
// table of On-Time/Off-Time pairs
uint32_t code_eu062Pairs[] = {
  500, 1580,
  530, 510,
  530, 1560,
  530, 21800,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu062Sequence[] = {
  0, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2, 3, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 1, 2, 2, 2
};
// POWER-Code table
struct IrCode code_eu062Code  = {
  38462,     // carrier frequency
  34,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu062Pairs,
  code_eu062Sequence
};


// code_eu063
//
// table of On-Time/Off-Time pairs
uint32_t code_eu063Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu063Sequence[] = {
  4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 2, 5, 3, 5, 0
};
// POWER-Code table
struct IrCode code_eu063Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu063Pairs,
  code_eu063Sequence
};


// code_eu064
//
// table of On-Time/Off-Time pairs
uint32_t code_eu064Pairs[] = {
  470, 2670,
  500, 550,
  500, 1100,
  500, 2650,
  500, 20550,
  500, 121170,
  1000, 570,
  1000, 1120,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu064Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 1, 1, 2, 1, 6, 2, 7, 5, 3, 1, 1, 2, 1, 6, 2, 7, 1
};
// POWER-Code table
struct IrCode code_eu064Code  = {
  30395,     // carrier frequency
  29,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu064Pairs,
  code_eu064Sequence
};


// code_eu065
//
// table of On-Time/Off-Time pairs
uint32_t code_eu065Pairs[] = {
  470, 2670,
  500, 550,
  500, 1100,
  500, 2650,
  500, 20550,
  500, 121170,
  1000, 1120,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu065Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 1, 1, 1, 2, 1, 6, 1, 1, 5, 3, 1, 1, 1, 2, 1, 6, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu065Code  = {
  30303,     // carrier frequency
  31,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu065Pairs,
  code_eu065Sequence
};


// code_eu066
//
// table of On-Time/Off-Time pairs
uint32_t code_eu066Pairs[] = {
  550, 550,
  550, 1670,
  550, 45770,
  550, 95060,
  4480, 4450,
  4500, 4440,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu066Sequence[] = {
  4, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 2, 5, 0, 3, 5, 0, 1
};
// POWER-Code table
struct IrCode code_eu066Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu066Pairs,
  code_eu066Sequence
};


// code_eu067
//
// table of On-Time/Off-Time pairs
uint32_t code_eu067Pairs[] = {
  940, 4730,
  940, 7280,
  1020, 16370,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu067Sequence[] = {
  1, 0, 0, 1, 0, 2, 1, 0, 0, 1, 0, 2
};
// POWER-Code table
struct IrCode code_eu067Code  = {
  38462,     // carrier frequency
  12,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu067Pairs,
  code_eu067Sequence
};


// code_eu068
//
// table of On-Time/Off-Time pairs
uint32_t code_eu068Pairs[] = {
  490, 2630,
  500, 540,
  500, 1080,
  500, 2630,
  500, 20290,
  500, 101990,
  1000, 1100,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu068Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 6, 1, 1, 5, 3, 2, 1, 1, 1, 1, 6, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu068Code  = {
  38610,     // carrier frequency
  31,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu068Pairs,
  code_eu068Sequence
};


// code_eu069
//
// table of On-Time/Off-Time pairs
uint32_t code_eu069Pairs[] = {
  40, 4990,
  40, 7500,
  40, 49990,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu069Sequence[] = {
  0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 2, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu069Code  = {
  0,         // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu069Pairs,
  code_eu069Sequence
};


// code_eu070
//
// table of On-Time/Off-Time pairs
uint32_t code_eu070Pairs[] = {
  40, 4990,
  40, 7500,
  40, 49990,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu070Sequence[] = {
  0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 2, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu070Code  = {
  0,         // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu070Pairs,
  code_eu070Sequence
};


// code_eu071
//
// table of On-Time/Off-Time pairs
uint32_t code_eu071Pairs[] = {
  140, 4910,
  140, 7430,
  140, 44220,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu071Sequence[] = {
  1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu071Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu071Pairs,
  code_eu071Sequence
};


// code_eu072
//
// table of On-Time/Off-Time pairs
uint32_t code_eu072Pairs[] = {
  50, 5680,
  50, 8540,
  50, 49990,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu072Sequence[] = {
  1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1
};
// POWER-Code table
struct IrCode code_eu072Code  = {
  0,         // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu072Pairs,
  code_eu072Sequence
};


// code_eu073
//
// table of On-Time/Off-Time pairs
uint32_t code_eu073Pairs[] = {
  150, 4930,
  160, 4930,
  160, 6980,
  160, 14140,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu073Sequence[] = {
  0, 1, 2, 1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu073Code  = {
  34483,     // carrier frequency
  16,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu073Pairs,
  code_eu073Sequence
};


// code_eu074
//
// table of On-Time/Off-Time pairs
uint32_t code_eu074Pairs[] = {
  880, 890,
  880, 900,
  880, 1790,
  880, 89770,
  1770, 900,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu074Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 4, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 4, 0
};
// POWER-Code table
struct IrCode code_eu074Code  = {
  35714,     // carrier frequency
  26,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu074Pairs,
  code_eu074Sequence
};


// code_eu075
//
// table of On-Time/Off-Time pairs
uint32_t code_eu075Pairs[] = {
  60, 5660,
  60, 8510,
  60, 54740,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu075Sequence[] = {
  0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 2, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1
};
// POWER-Code table
struct IrCode code_eu075Code  = {
  0,         // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu075Pairs,
  code_eu075Sequence
};


// code_eu076
//
// table of On-Time/Off-Time pairs
uint32_t code_eu076Pairs[] = {
  140, 8430,
  160, 5550,
  160, 8410,
  160, 49110,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu076Sequence[] = {
  0, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 3, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2
};
// POWER-Code table
struct IrCode code_eu076Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu076Pairs,
  code_eu076Sequence
};


// code_eu077
//
// table of On-Time/Off-Time pairs
uint32_t code_eu077Pairs[] = {
  470, 2670,
  500, 550,
  500, 1100,
  500, 2650,
  500, 20550,
  500, 121170,
  1000, 570,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu077Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 1, 1, 2, 1, 6, 2, 1, 1, 5, 3, 1, 1, 2, 1, 6, 2, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu077Code  = {
  30303,     // carrier frequency
  31,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu077Pairs,
  code_eu077Sequence
};


// code_eu078
//
// table of On-Time/Off-Time pairs
uint32_t code_eu078Pairs[] = {
  60, 9250,
  60, 13390,
  60, 20980,
  60, 27870,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu078Sequence[] = {
  2, 1, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_eu078Code  = {
  0,         // carrier frequency
  12,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu078Pairs,
  code_eu078Sequence
};


// code_eu079
//
// table of On-Time/Off-Time pairs
uint32_t code_eu079Pairs[] = {
  530, 590,
  530, 1700,
  530, 43590,
  8920, 4480,
  8930, 4480,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu079Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu079Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu079Pairs,
  code_eu079Sequence
};


// code_eu080
//
// table of On-Time/Off-Time pairs
uint32_t code_eu080Pairs[] = {
  550, 570,
  550, 1670,
  550, 44160,
  8950, 4480,
  8970, 4470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu080Sequence[] = {
  3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0
};
// POWER-Code table
struct IrCode code_eu080Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu080Pairs,
  code_eu080Sequence
};


// code_eu081
//
// table of On-Time/Off-Time pairs
uint32_t code_eu081Pairs[] = {
  260, 1850,
  270, 800,
  270, 1850,
  270, 42490,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu081Sequence[] = {
  0, 1, 2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 3, 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 3, 2, 1, 2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 3, 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 3, 2, 1, 2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 1, 2, 1, 1
};
// POWER-Code table
struct IrCode code_eu081Code  = {
  38462,     // carrier frequency
  80,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu081Pairs,
  code_eu081Sequence
};


// code_eu082
//
// table of On-Time/Off-Time pairs
uint32_t code_eu082Pairs[] = {
  510, 560,
  510, 1620,
  510, 28420,
  8480, 4300,
  8500, 4290,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu082Sequence[] = {
  3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 2, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0
};
// POWER-Code table
struct IrCode code_eu082Code  = {
  40000,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu082Pairs,
  code_eu082Sequence
};


// code_eu083
//
// table of On-Time/Off-Time pairs
uint32_t code_eu083Pairs[] = {
  160, 5590,
  160, 8470,
  160, 59000,
  170, 5590,
  170, 8470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu083Sequence[] = {
  0, 3, 4, 3, 4, 0, 4, 1, 4, 0, 4, 2, 3, 0, 4, 0, 4, 0, 4, 4, 4, 0, 4, 3
};
// POWER-Code table
struct IrCode code_eu083Code  = {
  33333,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu083Pairs,
  code_eu083Sequence
};


// code_eu084
//
// table of On-Time/Off-Time pairs
uint32_t code_eu084Pairs[] = {
  160, 4840,
  160, 7380,
  160, 7390,
  160, 47950,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu084Sequence[] = {
  1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu084Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu084Pairs,
  code_eu084Sequence
};


// code_eu085
//
// table of On-Time/Off-Time pairs
uint32_t code_eu085Pairs[] = {
  480, 520,
  480, 1600,
  480, 4000,
  480, 21200,
  7990, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu085Sequence[] = {
  4, 1, 1, 0, 1, 1, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 3, 4, 1, 1, 0, 1, 1, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu085Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu085Pairs,
  code_eu085Sequence
};


// code_eu086
//
// table of On-Time/Off-Time pairs
uint32_t code_eu086Pairs[] = {
  160, 8510,
  170, 5540,
  170, 8500,
  170, 8510,
  170, 48470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu086Sequence[] = {
  2, 1, 3, 0, 3, 1, 3, 3, 0, 1, 3, 4, 3, 1, 3, 3, 0, 1, 3, 3, 0, 1, 0, 2
};
// POWER-Code table
struct IrCode code_eu086Code  = {
  33333,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu086Pairs,
  code_eu086Sequence
};


// code_eu087
//
// table of On-Time/Off-Time pairs
uint32_t code_eu087Pairs[] = {
  140, 4910,
  140, 7430,
  140, 51260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu087Sequence[] = {
  1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu087Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu087Pairs,
  code_eu087Sequence
};


// code_eu088
//
// table of On-Time/Off-Time pairs
uint32_t code_eu088Pairs[] = {
  140, 4910,
  140, 7430,
  140, 48740,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu088Sequence[] = {
  1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu088Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu088Pairs,
  code_eu088Sequence
};


// code_eu089
//
// table of On-Time/Off-Time pairs
uint32_t code_eu089Pairs[] = {
  480, 520,
  480, 1600,
  480, 4000,
  480, 23350,
  7990, 4000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu089Sequence[] = {
  4, 1, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 3, 4, 1, 0, 1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu089Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu089Pairs,
  code_eu089Sequence
};


// code_eu090
//
// table of On-Time/Off-Time pairs
uint32_t code_eu090Pairs[] = {
  30, 90,
  30, 190,
  30, 290,
  30, 390,
  30, 99680,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu090Sequence[] = {
  3, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 1, 3, 4, 3, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 1, 3
};
// POWER-Code table
struct IrCode code_eu090Code  = {
  0,         // carrier frequency
  29,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu090Pairs,
  code_eu090Sequence
};


// code_eu091
//
// table of On-Time/Off-Time pairs
uint32_t code_eu091Pairs[] = {
  150, 1380,
  150, 4460,
  150, 6050,
  150, 65650,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu091Sequence[] = {
  2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 3, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 2
};
// POWER-Code table
struct IrCode code_eu091Code  = {
  38462,     // carrier frequency
  30,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu091Pairs,
  code_eu091Sequence
};


// code_eu092
//
// table of On-Time/Off-Time pairs
uint32_t code_eu092Pairs[] = {
  480, 500,
  480, 1480,
  480, 1490,
  480, 14240,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu092Sequence[] = {
  1, 0, 2, 0, 2, 0, 0, 0, 0, 0, 3, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu092Code  = {
  40000,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu092Pairs,
  code_eu092Sequence
};


// code_eu093
//
// table of On-Time/Off-Time pairs
uint32_t code_eu093Pairs[] = {
  870, 6390,
  880, 2750,
  880, 6390,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu093Sequence[] = {
  0, 1, 1, 1, 2, 1, 2, 2, 2, 1, 1
};
// POWER-Code table
struct IrCode code_eu093Code  = {
  35714,     // carrier frequency
  11,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu093Pairs,
  code_eu093Sequence
};


// code_eu094
//
// table of On-Time/Off-Time pairs
uint32_t code_eu094Pairs[] = {
  30, 80,
  30, 180,
  30, 240,
  30, 380,
  30, 99690,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu094Sequence[] = {
  3, 0, 1, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 3, 4, 3, 0, 1, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 3
};
// POWER-Code table
struct IrCode code_eu094Code  = {
  0,         // carrier frequency
  29,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu094Pairs,
  code_eu094Sequence
};


// code_eu095
//
// table of On-Time/Off-Time pairs
uint32_t code_eu095Pairs[] = {
  150, 4930,
  160, 4930,
  160, 6980,
  160, 14140,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu095Sequence[] = {
  0, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 2, 2, 2, 2, 2
};
// POWER-Code table
struct IrCode code_eu095Code  = {
  34483,     // carrier frequency
  16,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu095Pairs,
  code_eu095Sequence
};


// code_eu096
//
// table of On-Time/Off-Time pairs
uint32_t code_eu096Pairs[] = {
  130, 6080,
  140, 1410,
  140, 2960,
  140, 4510,
  140, 6060,
  140, 6080,
  140, 62070,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu096Sequence[] = {
  0, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 2, 4, 6, 5, 1, 1, 1, 2, 1, 1, 3, 1, 1, 1, 1, 2, 4, 1
};
// POWER-Code table
struct IrCode code_eu096Code  = {
  38462,     // carrier frequency
  30,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu096Pairs,
  code_eu096Sequence
};


// code_eu097
//
// table of On-Time/Off-Time pairs
uint32_t code_eu097Pairs[] = {
  150, 4930,
  160, 4930,
  160, 6980,
  160, 14140,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu097Sequence[] = {
  0, 1, 2, 1, 2, 2, 2, 3, 1, 1, 2, 1, 2, 2, 2, 1
};
// POWER-Code table
struct IrCode code_eu097Code  = {
  34483,     // carrier frequency
  16,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu097Pairs,
  code_eu097Sequence
};


// code_eu098
//
// table of On-Time/Off-Time pairs
uint32_t code_eu098Pairs[] = {
  30, 80,
  30, 180,
  30, 280,
  30, 127310,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu098Sequence[] = {
  2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 3, 2, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_eu098Code  = {
  0,         // carrier frequency
  27,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu098Pairs,
  code_eu098Sequence
};


// code_eu099
//
// table of On-Time/Off-Time pairs
uint32_t code_eu099Pairs[] = {
  460, 530,
  460, 1060,
  460, 2600,
  460, 15020,
  460, 109620,
  930, 530,
  930, 1060,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu099Sequence[] = {
  2, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 4, 2, 1, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 2
};
// POWER-Code table
struct IrCode code_eu099Code  = {
  35714,     // carrier frequency
  46,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu099Pairs,
  code_eu099Sequence
};


// code_eu100
//
// table of On-Time/Off-Time pairs
uint32_t code_eu100Pairs[] = {
  30, 80,
  30, 180,
  30, 280,
  30, 127310,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu100Sequence[] = {
  2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 3, 2, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_eu100Code  = {
  0,         // carrier frequency
  27,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu100Pairs,
  code_eu100Sequence
};


// code_eu101
//
// table of On-Time/Off-Time pairs
uint32_t code_eu101Pairs[] = {
  140, 4910,
  140, 7430,
  140, 46740,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu101Sequence[] = {
  1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1
};
// POWER-Code table
struct IrCode code_eu101Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu101Pairs,
  code_eu101Sequence
};


// code_eu102
//
// table of On-Time/Off-Time pairs
uint32_t code_eu102Pairs[] = {
  140, 4910,
  140, 7430,
  140, 51260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu102Sequence[] = {
  1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu102Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu102Pairs,
  code_eu102Sequence
};


// code_eu103
//
// table of On-Time/Off-Time pairs
uint32_t code_eu103Pairs[] = {
  440, 8150,
  450, 5280,
  450, 8150,
  450, 50000,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu103Sequence[] = {
  0, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 3, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2
};
// POWER-Code table
struct IrCode code_eu103Code  = {
  34483,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu103Pairs,
  code_eu103Sequence
};


// code_eu104
//
// table of On-Time/Off-Time pairs
uint32_t code_eu104Pairs[] = {
  140, 4910,
  140, 7430,
  140, 58810,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu104Sequence[] = {
  1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu104Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu104Pairs,
  code_eu104Sequence
};


// code_eu105
//
// table of On-Time/Off-Time pairs
uint32_t code_eu105Pairs[] = {
  530, 560,
  530, 1710,
  530, 39500,
  530, 95990,
  8980, 4510,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu105Sequence[] = {
  4, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 2, 5, 3, 5, 1
};
// POWER-Code table
struct IrCode code_eu105Code  = {
  38610,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu105Pairs,
  code_eu105Sequence
};


// code_eu106
//
// table of On-Time/Off-Time pairs
uint32_t code_eu106Pairs[] = {
  480, 2460,
  500, 470,
  500, 940,
  500, 2450,
  500, 14880,
  500, 109700,
  1000, 470,
  1000, 940,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu106Sequence[] = {
  0, 2, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 6, 2, 1, 1, 1, 1, 7, 7, 6, 2, 5, 3, 2, 1, 6, 2, 1, 1, 1, 1, 7, 7, 6, 2, 5, 3, 2, 1, 6, 2, 1, 1, 1, 1, 7, 7, 6, 2, 2
};
// POWER-Code table
struct IrCode code_eu106Code  = {
  38462,     // carrier frequency
  59,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu106Pairs,
  code_eu106Sequence
};


// code_eu107
//
// table of On-Time/Off-Time pairs
uint32_t code_eu107Pairs[] = {
  160, 8470,
  160, 59000,
  170, 5590,
  170, 8460,
  170, 8470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu107Sequence[] = {
  3, 0, 4, 0, 4, 2, 4, 0, 4, 2, 4, 1, 4, 4, 0, 4, 0, 2, 0, 4, 0, 2, 0, 3
};
// POWER-Code table
struct IrCode code_eu107Code  = {
  33333,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu107Pairs,
  code_eu107Sequence
};


// code_eu108
//
// table of On-Time/Off-Time pairs
uint32_t code_eu108Pairs[] = {
  140, 4910,
  140, 7430,
  140, 46220,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu108Sequence[] = {
  1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu108Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu108Pairs,
  code_eu108Sequence
};


// code_eu109
//
// table of On-Time/Off-Time pairs
uint32_t code_eu109Pairs[] = {
  240, 1850,
  270, 780,
  270, 1830,
  270, 15420,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu109Sequence[] = {
  0, 1, 2, 1, 2, 1, 1, 1, 1, 1, 3, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu109Code  = {
  38462,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu109Pairs,
  code_eu109Sequence
};


// code_eu110
//
// table of On-Time/Off-Time pairs
uint32_t code_eu110Pairs[] = {
  560, 550,
  560, 1680,
  560, 48500,
  4470, 4530,
  4480, 4530,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu110Sequence[] = {
  3, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 4, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu110Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu110Pairs,
  code_eu110Sequence
};


// code_eu111
//
// table of On-Time/Off-Time pairs
uint32_t code_eu111Pairs[] = {
  490, 520,
  490, 2500,
  490, 2520,
  490, 23770,
  490, 120090,
  1000, 520,
  1000, 1020,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu111Sequence[] = {
  1, 0, 5, 0, 0, 0, 3, 2, 0, 6, 0, 0, 0, 4, 2, 0, 6, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu111Code  = {
  31250,     // carrier frequency
  21,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu111Pairs,
  code_eu111Sequence
};


// code_eu112
//
// table of On-Time/Off-Time pairs
uint32_t code_eu112Pairs[] = {
  550, 550,
  550, 1670,
  550, 50230,
  550, 95060,
  4480, 4450,
  4500, 4440,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu112Sequence[] = {
  4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 5, 1, 3, 5, 1, 0
};
// POWER-Code table
struct IrCode code_eu112Code  = {
  38462,     // carrier frequency
  40,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu112Pairs,
  code_eu112Sequence
};


// code_eu113
//
// table of On-Time/Off-Time pairs
uint32_t code_eu113Pairs[] = {
  490, 530,
  490, 1040,
  490, 2620,
  490, 2640,
  490, 80300,
  1000, 1030,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu113Sequence[] = {
  2, 1, 5, 0, 0, 0, 4, 3, 1, 5, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_eu113Code  = {
  31250,     // carrier frequency
  14,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu113Pairs,
  code_eu113Sequence
};


// code_eu114
//
// table of On-Time/Off-Time pairs
uint32_t code_eu114Pairs[] = {
  470, 2670,
  500, 550,
  500, 1100,
  500, 2650,
  500, 20550,
  500, 121170,
  1000, 570,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu114Sequence[] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 6, 1, 2, 1, 1, 1, 5, 3, 2, 1, 6, 1, 2, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu114Code  = {
  30303,     // carrier frequency
  31,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu114Pairs,
  code_eu114Sequence
};


// code_eu115
//
// table of On-Time/Off-Time pairs
uint32_t code_eu115Pairs[] = {
  480, 980,
  480, 1960,
  970, 8360,
  3950, 3880,
  19310, 3890,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu115Sequence[] = {
  4, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2, 3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2, 3, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1
};
// POWER-Code table
struct IrCode code_eu115Code  = {
  58824,     // carrier frequency
  77,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu115Pairs,
  code_eu115Sequence
};


// code_eu116
//
// table of On-Time/Off-Time pairs
uint32_t code_eu116Pairs[] = {
  30, 90,
  30, 310,
  30, 420,
  30, 109570,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu116Sequence[] = {
  2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 3, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2
};
// POWER-Code table
struct IrCode code_eu116Code  = {
  0,         // carrier frequency
  29,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu116Pairs,
  code_eu116Sequence
};


// code_eu117
//
// table of On-Time/Off-Time pairs
uint32_t code_eu117Pairs[] = {
  490, 530,
  490, 2620,
  490, 2640,
  490, 80300,
  1000, 1030,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu117Sequence[] = {
  1, 0, 4, 0, 0, 0, 3, 2, 0, 4, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu117Code  = {
  31250,     // carrier frequency
  14,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu117Pairs,
  code_eu117Sequence
};


// code_eu118
//
// table of On-Time/Off-Time pairs
uint32_t code_eu118Pairs[] = {
  440, 8150,
  450, 5280,
  450, 8150,
  450, 47130,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu118Sequence[] = {
  0, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 3, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2
};
// POWER-Code table
struct IrCode code_eu118Code  = {
  34483,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu118Pairs,
  code_eu118Sequence
};


// code_eu119
//
// table of On-Time/Off-Time pairs
uint32_t code_eu119Pairs[] = {
  140, 4910,
  140, 7430,
  140, 54300,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu119Sequence[] = {
  1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 2, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu119Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu119Pairs,
  code_eu119Sequence
};


// code_eu120
//
// table of On-Time/Off-Time pairs
uint32_t code_eu120Pairs[] = {
  190, 780,
  210, 270,
  210, 770,
  210, 37850,
  220, 0,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu120Sequence[] = {
  0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 4
};
// POWER-Code table
struct IrCode code_eu120Code  = {
  38462,     // carrier frequency
  82,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu120Pairs,
  code_eu120Sequence
};


// code_eu121
//
// table of On-Time/Off-Time pairs
uint32_t code_eu121Pairs[] = {
  840, 880,
  840, 2610,
  840, 33600,
  3470, 3470,
  3470, 3480,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu121Sequence[] = {
  3, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 2, 4, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu121Code  = {
  38462,     // carrier frequency
  52,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu121Pairs,
  code_eu121Sequence
};


// code_eu122
//
// table of On-Time/Off-Time pairs
uint32_t code_eu122Pairs[] = {
  190, 780,
  210, 270,
  210, 770,
  210, 37850,
  220, 0,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu122Sequence[] = {
  0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 3, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 4
};
// POWER-Code table
struct IrCode code_eu122Code  = {
  38462,     // carrier frequency
  82,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu122Pairs,
  code_eu122Sequence
};


// code_eu123
//
// table of On-Time/Off-Time pairs
uint32_t code_eu123Pairs[] = {
  130, 4900,
  130, 7410,
  130, 7420,
  130, 54430,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu123Sequence[] = {
  1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 3, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 1
};
// POWER-Code table
struct IrCode code_eu123Code  = {
  40000,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu123Pairs,
  code_eu123Sequence
};


// code_eu124
//
// table of On-Time/Off-Time pairs
uint32_t code_eu124Pairs[] = {
  500, 540,
  500, 1580,
  500, 4070,
  500, 21530,
  8430, 4070,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu124Sequence[] = {
  4, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 3, 4, 0, 0, 1, 0, 1, 0, 0, 0, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0
};
// POWER-Code table
struct IrCode code_eu124Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu124Pairs,
  code_eu124Sequence
};


// code_eu125
//
// table of On-Time/Off-Time pairs
uint32_t code_eu125Pairs[] = {
  550, 560,
  550, 1680,
  550, 39290,
  560, 0,
  8820, 4540,
  8840, 4520,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu125Sequence[] = {
  4, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 2, 5, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 3
};
// POWER-Code table
struct IrCode code_eu125Code  = {
  38462,     // carrier frequency
  68,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu125Pairs,
  code_eu125Sequence
};


// code_eu126
//
// table of On-Time/Off-Time pairs
uint32_t code_eu126Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu126Sequence[] = {
  5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu126Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu126Pairs,
  code_eu126Sequence
};


// code_eu127
//
// table of On-Time/Off-Time pairs
uint32_t code_eu127Pairs[] = {
  140, 4910,
  140, 7430,
  140, 51260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu127Sequence[] = {
  1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 2, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu127Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu127Pairs,
  code_eu127Sequence
};


// code_eu128
//
// table of On-Time/Off-Time pairs
uint32_t code_eu128Pairs[] = {
  1520, 4710,
  1540, 1560,
  1540, 4690,
  1540, 7820,
  1540, 29470,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu128Sequence[] = {
  0, 1, 3, 4, 2, 1, 3, 1
};
// POWER-Code table
struct IrCode code_eu128Code  = {
  41667,     // carrier frequency
  8,         // number of On-Time/Off-Time pairs in this Off-Code
  code_eu128Pairs,
  code_eu128Sequence
};


// code_eu129
//
// table of On-Time/Off-Time pairs
uint32_t code_eu129Pairs[] = {
  500, 500,
  500, 990,
  500, 2510,
  500, 2520,
  500, 14490,
  500, 110140,
  1020, 490,
  1020, 980,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu129Sequence[] = {
  2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 0, 6, 1, 0, 0, 0, 0, 7, 7, 0, 7, 5, 3, 1, 0, 6, 1, 0, 0, 0, 0, 7, 7, 0, 7, 2
};
// POWER-Code table
struct IrCode code_eu129Code  = {
  38462,     // carrier frequency
  45,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu129Pairs,
  code_eu129Sequence
};


// code_eu130
//
// table of On-Time/Off-Time pairs
uint32_t code_eu130Pairs[] = {
  500, 500,
  500, 990,
  500, 2510,
  500, 2520,
  500, 14490,
  500, 110140,
  1020, 490,
  1020, 980,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu130Sequence[] = {
  2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 0, 6, 1, 0, 0, 0, 0, 7, 6, 1, 7, 5, 3, 1, 0, 6, 1, 0, 0, 0, 0, 7, 6, 1, 7, 2
};
// POWER-Code table
struct IrCode code_eu130Code  = {
  38462,     // carrier frequency
  45,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu130Pairs,
  code_eu130Sequence
};


// code_eu131
//
// table of On-Time/Off-Time pairs
uint32_t code_eu131Pairs[] = {
  140, 4910,
  140, 7430,
  140, 41700,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu131Sequence[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu131Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu131Pairs,
  code_eu131Sequence
};


// code_eu132
//
// table of On-Time/Off-Time pairs
uint32_t code_eu132Pairs[] = {
  40, 4990,
  40, 7500,
  40, 49990,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu132Sequence[] = {
  0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 2, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu132Code  = {
  0,         // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu132Pairs,
  code_eu132Sequence
};


// code_eu133
//
// table of On-Time/Off-Time pairs
uint32_t code_eu133Pairs[] = {
  140, 4910,
  140, 7430,
  140, 44220,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu133Sequence[] = {
  1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1
};
// POWER-Code table
struct IrCode code_eu133Code  = {
  38462,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu133Pairs,
  code_eu133Sequence
};


// code_eu134
//
// table of On-Time/Off-Time pairs
uint32_t code_eu134Pairs[] = {
  130, 4900,
  130, 7410,
  130, 7420,
  130, 59390,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu134Sequence[] = {
  1, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 3, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 1
};
// POWER-Code table
struct IrCode code_eu134Code  = {
  40000,     // carrier frequency
  24,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu134Pairs,
  code_eu134Sequence
};


// code_eu135
//
// table of On-Time/Off-Time pairs
uint32_t code_eu135Pairs[] = {
  60, 5660,
  60, 8510,
  60, 51880,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu135Sequence[] = {
  1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 2, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1
};
// POWER-Code table
struct IrCode code_eu135Code  = {
  0,         // carrier frequency
  23,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu135Pairs,
  code_eu135Sequence
};


// code_eu136
//
// table of On-Time/Off-Time pairs
uint32_t code_eu136Pairs[] = {
  550, 570,
  550, 1700,
  550, 39490,
  550, 96230,
  560, 0,
  8980, 4530,
  9000, 2260,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu136Sequence[] = {
  5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 6, 3, 6, 4
};
// POWER-Code table
struct IrCode code_eu136Code  = {
  38462,     // carrier frequency
  38,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu136Pairs,
  code_eu136Sequence
};


// code_eu137
//
// table of On-Time/Off-Time pairs
uint32_t code_eu137Pairs[] = {
  860, 910,
  870, 900,
  870, 1800,
  870, 88680,
  880, 0,
  1740, 900,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu137Sequence[] = {
  0, 5, 1, 1, 2, 5, 1, 2, 1, 5, 3, 1, 5, 1, 1, 2, 5, 1, 2, 1, 5, 4
};
// POWER-Code table
struct IrCode code_eu137Code  = {
  35714,     // carrier frequency
  22,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu137Pairs,
  code_eu137Sequence
};


// code_eu138
//
// table of On-Time/Off-Time pairs
uint32_t code_eu138Pairs[] = {
  40, 10360,
  40, 15070,
  40, 30050,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu138Sequence[] = {
  0, 0, 1, 1, 1, 2, 0, 0, 1, 1, 1
};
// POWER-Code table
struct IrCode code_eu138Code  = {
  0,         // carrier frequency
  11,        // number of On-Time/Off-Time pairs in this Off-Code
  code_eu138Pairs,
  code_eu138Sequence
};


// code_eu139
//
// table of On-Time/Off-Time pairs
uint32_t code_eu139Pairs[] = {
  0, 0,
  140, 1410,
  140, 4520,
  140, 6070,
  140, 63100,
};
// table of indices to On-Time/Off-Time pairs
uint8_t code_eu139Sequence[] = {
  3, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 3, 4, 3, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 3, 0
};
// POWER-Code table
struct IrCode code_eu139Code  = {
  0,     // carrier frequency
  30,    // number of On-Time/Off-Time pairs in this Off-Code
  code_eu139Pairs,
  code_eu139Sequence
};




////////////////////////////////////////////////////////////////




extern "C" struct IrCode * NApowerCodes[]  = {
  &code_na000Code,
  &code_na001Code,
  &code_na002Code,
  &code_na003Code,
  &code_na004Code,
  &code_na005Code,
  &code_na006Code,
  &code_na007Code,
  &code_na008Code,
  &code_na009Code,
  &code_na010Code,
  &code_na011Code,
  &code_na012Code,
  &code_na013Code,
  &code_na014Code,
  &code_na015Code,
  &code_na016Code,
  &code_na017Code,
  &code_na018Code,
  &code_na019Code,
  &code_na020Code,
  &code_na021Code,
  &code_na022Code,
  &code_na023Code,
  &code_na024Code,
  &code_na025Code,
  &code_na026Code,
  &code_na027Code,
  &code_na028Code,
  &code_na029Code,
  &code_na030Code,
  &code_na031Code,
  &code_na032Code,
  &code_na033Code,
  &code_na034Code,
  &code_na035Code,
  &code_na036Code,
  &code_na037Code,
  &code_na038Code,
  &code_na039Code,
  &code_na040Code,
  &code_na041Code,
  &code_na042Code,
  &code_na043Code,
  &code_na044Code,
  &code_na045Code,
  &code_na046Code,
  &code_na047Code,
  &code_na048Code,
  &code_na049Code,
  &code_na050Code,
  &code_na051Code,
  &code_na052Code,
  &code_na053Code,
  &code_na054Code,
  &code_na055Code,
  &code_na056Code,
  &code_na057Code,
  &code_na058Code,
  &code_na059Code,
  &code_na060Code,
  &code_na061Code,
  &code_na062Code,
  &code_na063Code,
  &code_na064Code,
  &code_na065Code,
  &code_na066Code,
  &code_na067Code,
  &code_na068Code,
  &code_na069Code,
  &code_na070Code,
  &code_na071Code,
  &code_na072Code,
  &code_na073Code,
  &code_na074Code,
  &code_na075Code,
  &code_na076Code,
  &code_na077Code,
  &code_na078Code,
  &code_na079Code,
  &code_na080Code,
  &code_na081Code,
  &code_na082Code,
  &code_na083Code,
  &code_na084Code,
  &code_na085Code,
  &code_na086Code,
  &code_na087Code,
  &code_na088Code,
  &code_na089Code,
  &code_na090Code,
  &code_na091Code,
  &code_na092Code,
  &code_na093Code,
  &code_na094Code,
  &code_na095Code,
  &code_na096Code,
  &code_na097Code,
  &code_na098Code,
  &code_na099Code,
  &code_na100Code,
  &code_na101Code,
  &code_na102Code,
  &code_na103Code,
  &code_na104Code,
  &code_na105Code,
  &code_na106Code,
  &code_na107Code,
  &code_na108Code,
  &code_na109Code,
  &code_na110Code,
  &code_na111Code,
  &code_na112Code,
  &code_na113Code,
  &code_na114Code,
  &code_na115Code,
  &code_na116Code,
  &code_na117Code,
  &code_na118Code,
  &code_na119Code,
  &code_na120Code,
  &code_na121Code,
  &code_na122Code,
  &code_na123Code,
  &code_na124Code,
  &code_na125Code,
  &code_na126Code,
  &code_na127Code,
  &code_na128Code,
  &code_na129Code,
  &code_na130Code,
  &code_na131Code,
  &code_na132Code,
  &code_na133Code,
  &code_na134Code,
  &code_na135Code,
  &code_na136Code,
};


extern "C" struct IrCode * EUpowerCodes[]  = {
    &code_eu000Code,
    &code_eu001Code,
    &code_eu002Code,
    &code_eu003Code,
    &code_eu004Code,
    &code_eu005Code,
    &code_eu006Code,
    &code_eu007Code,
    &code_eu008Code,
    &code_eu009Code,
    &code_eu010Code,
    &code_eu011Code,
    &code_eu012Code,
    &code_eu013Code,
    &code_eu014Code,
    &code_eu015Code,
    &code_eu016Code,
    &code_eu017Code,
    &code_eu018Code,
    &code_eu019Code,
    &code_eu020Code,
    &code_eu021Code,
    &code_eu022Code,
    &code_eu023Code,
    &code_eu024Code,
    &code_eu025Code,
    &code_eu026Code,
    &code_eu027Code,
    &code_eu028Code,
    &code_eu029Code,
    &code_eu030Code,
    &code_eu031Code,
    &code_eu032Code,
    &code_eu033Code,
    &code_eu034Code,
    //&code_eu035Code, same as eu009
    &code_eu036Code,
    &code_eu037Code,
    &code_eu038Code,
    &code_eu039Code,
    &code_eu040Code,
    &code_eu041Code,
    &code_eu042Code,
    &code_eu043Code,
    &code_eu044Code,
    &code_eu045Code,
    &code_eu046Code,
    &code_eu047Code,
    &code_eu048Code,
    &code_eu049Code,
    &code_eu050Code,
    &code_eu051Code,
    &code_eu052Code,
    &code_eu053Code,
    &code_eu054Code,
    &code_eu055Code,
    &code_eu056Code,
    //&code_eu057Code, same as eu008
    &code_eu058Code,
    &code_eu059Code,
    &code_eu060Code,
    &code_eu061Code,
    &code_eu062Code,
    &code_eu063Code,
    &code_eu064Code,
    &code_eu065Code,
    &code_eu066Code,
    &code_eu067Code,
    &code_eu068Code,
    &code_eu069Code,
    &code_eu070Code,
    &code_eu071Code,
    &code_eu072Code,
    &code_eu073Code,
    &code_eu074Code,
    &code_eu075Code,
    &code_eu076Code,
    &code_eu077Code,
    &code_eu078Code,
    &code_eu079Code,
    &code_eu080Code,
    &code_eu081Code,
    &code_eu082Code,
    &code_eu083Code,
    &code_eu084Code,
    &code_eu085Code,
    &code_eu086Code,
    &code_eu087Code,
    &code_eu088Code,
    &code_eu089Code,
    &code_eu090Code,
    &code_eu091Code,
    &code_eu092Code,
    &code_eu093Code,
    &code_eu094Code,
    &code_eu095Code,
    &code_eu096Code,
    &code_eu097Code,
    &code_eu098Code,
    &code_eu099Code,
    &code_eu100Code,
    &code_eu101Code,
    &code_eu102Code,
    &code_eu103Code,
    &code_eu104Code,
    &code_eu105Code,
    &code_eu106Code,
    &code_eu107Code,
    &code_eu108Code,
    &code_eu109Code,
    &code_eu110Code,
    &code_eu111Code,
    &code_eu112Code,
    &code_eu113Code,
    &code_eu114Code,
    &code_eu115Code,
    &code_eu116Code,
    &code_eu117Code,
    &code_eu118Code,
    &code_eu119Code,
    &code_eu120Code,
    &code_eu121Code,
    &code_eu122Code,
    &code_eu123Code,
    &code_eu124Code,
    &code_eu125Code,
    &code_eu126Code,
    &code_eu127Code,
    &code_eu128Code,
    &code_eu129Code,
    &code_eu130Code,
    &code_eu131Code,
    &code_eu132Code,
    &code_eu133Code,
    &code_eu134Code,
    &code_eu135Code,
    &code_eu136Code,
    &code_eu137Code,
    &code_eu138Code,
    &code_eu139Code,
};


uint8_t num_NAcodes = NUM_ELEM(NApowerCodes);
uint8_t num_EUcodes = NUM_ELEM(EUpowerCodes);
