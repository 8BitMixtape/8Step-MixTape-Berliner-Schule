/* PCrazy shit 8-bit symphony generator                   
 
   inspired by:
   http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html
   http://countercomplex.blogspot.ch/2011/10/some-deep-analysis-of-one-line-music.html
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
int buttonLeft = 6; 
int potiPin = A3;
const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4};

int potiValue = 0;            // readout of the right potentiometer
int P1 = 0;                   // mapped value to put into OneLiner
int buttonState1 = 0;         // current state of the left button
int lastButtonState1 = 1;     // previous state of the left button
int buttonState2 = 0;         // current state of the right button
int lastButtonState2 = 1;     // previous state of the right button
int count = 0;                // count how many times button is pressed
int sampleSpeed = 100;        // variable for the microsecond delay -> speed/pitch
int mappedSpeed = 80;
const int POT_THRESHOLD = 100;
int sampleMode = 0;

long t = 0; 
int v = 0; 
 
void setup () { 
// = TCCR0B & 0b11111001; //no timer pre-scaler, fast PWM
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000001; 
  
//Set all the pins as inputs and outputs
  //pinMode(potiPin, INPUT);
  pinMode (speakerPin, OUTPUT);
  pinMode (buttonLeft, INPUT_PULLUP);
  digitalWrite(ledPinMapping[count], HIGH);
}

void loop () {
// read the potentiometers into a local variable:
  potiValue = analogRead(potiPin); // this also reads th right button
  // check if right button was pressed:
  if (potiValue <= 10){
    buttonState2 = 0;
  }
  else {
    buttonState2 = 1;
  }

// count the button right clicks
  if (buttonState2 != lastButtonState2 && buttonState2 == 0) {
    digitalWrite(ledPinMapping[count], LOW);
    count++; 
    if (count >= 8) {
      count = 0;
      } 
    digitalWrite(ledPinMapping[count], HIGH);
  } 

// read the state of the switch into a local variable:
  buttonState1 = digitalRead(buttonLeft); // LOW means button is pressed (connected to GND)

// map the read value into a useable smaller variable to use in the OneLiner  
  P1 = ((potiValue>>5) + 1); // bit-shifting >> 5 + 1 leads to values 1 - 32
  
// A selection of OneLiners
  if (count == 0)  v = t&48&t>>8;
  if (count == 1) v = t * ((t>>12|t>>8)&63&t>>(P1>>3));                // add the variable P1
  if (count == 2) v = t*t/P1;                                          // (only sounds with analogWrite)
  if (count == 3) v = (t|P1) * ((t>>15|t>>11)&47&t>>3);                // some other example
  if (count == 4) v = t*(((t>>(12+(P1/2)))|(t>>8))&((P1)&(t>>4)));     // intersting
  if (count == 5) v = (t|3) * ((t>>9|t>>6)&P1&t>>3);                   // 4onTheFloor Techno Mucke
  if (count == 6) v = t>>4&1?t>>5:-t>>P1 ;                             // Dub-Keule (only sounds with analogWrite) 
  if (count == 7) v = t * ((t>>12|t>>8)&P1&t>>(13>>3));  


// change the speed while button is pressed, to be done:
  if (buttonState1 == LOW) {
    sampleSpeed = analogRead(potiPin);
    mappedSpeed = map(sampleSpeed,POT_THRESHOLD,1023,0,255);
    if (mappedSpeed <= 0) {
      mappedSpeed = 0;
    }
  }
  else {
  }

// put the result of the OneLiner on the speakerPin (analog or digital):
  if (buttonState1 != lastButtonState1 && buttonState1 == LOW) {
    sampleMode++;
    if (sampleMode >= 2) {
      sampleMode = 0;
    } 
  }
  if (sampleMode == 0) {
    digitalWrite (speakerPin, v);
  }
  else {
    analogWrite (speakerPin, v);
  }

// delay some microseconds (1-1000) to choose the speed:
  delayMicroseconds(mappedSpeed);
  
// incremental increase of the variable t:
  t++;

  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
}

