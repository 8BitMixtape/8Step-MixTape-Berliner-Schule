/*
*
* Modified by dusjagr and stahl to run on the attiny84 for the 8Step MixTape | Berliner Schule
* CC 2015
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

const byte pot = A3;
const byte buttonLeft = 6; 
const byte controlVoltagePin = 5;
const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4}; // change this if you insiston using more than 8 steps
const int POT_THRESHOLD = 10;
int potValue = 0;
int mappedValue = 0;

int buttonState1 = 0;         // current state of the button
int lastButtonState1 = 1;     // previous state of the button
int count = 0;                // count how many times button is pressed


void setup(){
  //Set all the pins as inputs and outputs
  pinMode(pot, INPUT);
  pinMode (buttonLeft, INPUT_PULLUP);
  pinMode(controlVoltagePin, OUTPUT);
  
  // Set all the LED pins as Output using a for loop
  for (int i = 0; i < 8; i++){
    pinMode(ledPinMapping[i], OUTPUT);
  }
  
  //Turn on the CV pin, which controls the transistor, thus all LEDs
  digitalWrite(controlVoltagePin, HIGH);

  // Turn the LEDs on, slooooowly
  for (int i = 0; i < 8; i++){
    digitalWrite(ledPinMapping[i], HIGH);
    delay(100);
  }
  
  delay(100); //wait a bit
  // Turn the LEDs off
  for (int i = 0; i < 8; i++){
    digitalWrite(ledPinMapping[i], LOW);
    delay(1);
  }
  digitalWrite(ledPinMapping[count], HIGH);
}

void loop(){
  // read the state of the switch into a local variable:
  buttonState1 = digitalRead(buttonLeft);
  // read the potentiometers into a local variable:
  potValue = analogRead(pot); // this also reads th right button
  // read the potentiometers into a local variable:
  mappedValue = map(potValue,POT_THRESHOLD,1023,0,255);
  // Make sure the mapped value doesnt get negativ
  if (mappedValue <= 0) {
    mappedValue = 0;
  }
  
  analogWrite(controlVoltagePin, mappedValue);
  
  if (buttonState1 != lastButtonState1 && buttonState1 == LOW) {
    digitalWrite(ledPinMapping[count], LOW);
    count++;
    if (count >= 8) {
        count = 0;
      } 
    digitalWrite(ledPinMapping[count], HIGH); 
  }
  lastButtonState1 = buttonState1;
}



