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
// Download the Attiny cores for compatibility to the Arduino IDE from:
// https://github.com/8BitMixtape
//

 
int speakerPin = 5;
int potiPin = A3;
const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4};

long t = 0; 
int v = 0; 
 
void setup () {
 
  // = TCCR0B & 0b11111001; //no timer pre-scaler, fast PWM
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000001; 

  pinMode (speakerPin, OUTPUT);
  
  analogWrite(speakerPin, 255);
  for (int i = 0; i < 8; i++){
    pinMode(ledPinMapping[i], OUTPUT);
  };
  
  for (int i = 0; i <= 8; i++){
    digitalWrite(ledPinMapping[i], HIGH);
    delay(80);
  };
 
}
 
 
void loop () {
  int P1 = ((analogRead(potiPin)>>5) + 2);
  int speedSiech = analogRead(potiPin);
  
  //v = t*((t>>12|t>>8)&63&t>>(P1>>3));
  //v = t * ((t>>12|t>>8)&63&t>>8);
  //v = t * ((t>>12|t>>8)&P1&t>>(13>>3));
  //v = t * ((t>>15|t>>P1)&83&t>>(P1>>3));
  
  v = (t|3) * ((t>>9|t>>6)&P1&t>>3); // 4onTheFloor  // 4onTheFloor Techno Mucke
  
  //v = t*t/P1;
  //v = (t|P1) * ((t>>15|t>>11)&47&t>>3);
  //v = t*(((t>>(12+(P1/2)))|(t>>8))&((15-7)&(t>>4)));
  //v = t>>4&1?t>>5:-t>>P1 ; // Dub-Keule
  
  if (t % 17 == 0) {
    digitalWrite(ledPinMapping[t % 8], v % 3);
        
        }
  
    
  //digitalWrite (speakerPin, v);
  //
  
  analogWrite (speakerPin, v);
  if (speedSiech < 400) {
    delayMicroseconds(10);
  }
  else {
    delayMicroseconds(350);
  }
  //delayMicroseconds(100 + (P1*20));
  t++;
}

