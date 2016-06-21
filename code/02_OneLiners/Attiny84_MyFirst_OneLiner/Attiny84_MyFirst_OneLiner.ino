/* PCrazy shit 8-bit symphony generator                   
 
   inspired by:
   http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html
   more on the 8 bit MixTape
   http://wiki.sgmk-ssam.ch/index.php/8bit_Mix_Tape
   
 modified 18 Feb 2016
 By dusjagr for use with Attiny84
 
 This example code is in the public domain.

 */
 
// ATMEL ATTINY84 / ARDUINO
//
// Download the 8Bit MixTape (Attiny) cores for compatibility to the Arduino IDE from:
// https://github.com/8BitMixtape/8Step-MixTape-Berliner-Schule
// or the general Attiny cores:
// http://highlowtech.org/?p=1695
//
//                       +-\/-+
//                  Vcc 1|    |14 GND
//        LED2 (10) PB0 2|    |13 PA0 (0) LED5
//        LED1 (9)  PB1 3|    |12 PA1 (1) LED6
//                Reset 4|    |11 PA2 (2) LED7
//        LED3 (8)  PB2 5|    |10 PA3 (3) A3 left Poti & right Button
//        LED4 (7)  PA7 6|    |9  PA4 (4) LED1 SCK
//  empty MOSI (6)  PA6 7|    |8  PA5 (5) MISO PWM-Output -> CV
//                       +----+
//
// https://github.com/8BitMixtape/

int speakerPin = 5;
int potiPin = A3;
const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4};

long t = 0; 
int v = 0; 
 
void setup () { 
// = TCCR0B & 0b11111001; //no timer pre-scaler, fast PWM
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000001; 
  
//Set all the pins as inputs and outputs
  pinMode (speakerPin, OUTPUT);
}

void loop () {
// read the potentiometers into a local variable:
  int P1 = ((analogRead(potiPin)>>4) + 1); // bit-shifting >> 4 leads to values 0 - 63

// A selection of OneLiners, uncomment a single one to test:
  v = t * ((t>>12|t>>8)&63&t>>1);                      // a classic, no variable
  //v = t * ((t>>12|t>>8)&63&t>>(P1>>3));                // add the variable P1
  //v = t * ((t>>12|t>>8)&P1&t>>(13>>3));                // add P1 at different positions
  //v = t * ((t>>15|t>>P1)&83&t>>(P1>>3));               // use the variable 2 times
  //v = t*t/P1;                                          // (only sounds with analogWrite)
  //v = (t|P1) * ((t>>15|t>>11)&47&t>>3);                // some other example
  //v = t*(((t>>(12+(P1/2)))|(t>>8))&((15-7)&(t>>4)));   // not so intersting
  //v = (t|3) * ((t>>9|t>>6)&P1&t>>3);                   // 4onTheFloor Techno Mucke
  //v = t>>4&1?t>>5:-t>>P1 ;                             // Dub-Keule (only sounds with analogWrite) 

// put the result of the OneLiner on the speakerPin (analog or digital):
  digitalWrite (speakerPin, v);
  //analogWrite (speakerPin, v);

// delay some microseconds (1-1000) to choose the speed:
  delayMicroseconds(100);
  
// incremental increase of the variable t:
  t++;
}

