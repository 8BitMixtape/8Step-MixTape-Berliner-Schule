/*
 Fading Heartbeat and Read Analog Sensor

 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe
 
 modified 30 April 2016
 By dusjagr for use with Attiny84 
 
 http://arduino.cc/en/Tutorial/Fading
 
 This example code is in the public domain.
 
 */
 
// ATMEL ATTINY84 / ARDUINO
//
// Download the Attiny cores for compatibility to the Arduino IDE from:
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


// define the pins
int ledPin = 5;    // LED connected to digital pin 5
int potiPin = A3;    // Potentiometer connected to AnalogIn pin 3

// The 8 LEDs on the Berliner Schule are controlled through a transistor gated from the ledPin
const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4};

// define some other variables
int sensor = 0;      // define variable to store the sensor value
int fadeSpeed = 0;   // define variable to define the speed of the fading

// LUT (Look Up Table) to have smoother fading using a logarithmic table
const char ledFadeTableFull[256] = {
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,
  3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,
  5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,9,9,
  9,9,10,10,10,10,10,11,11,11,11,12,12,12,13,13,13,13,
  14,14,14,15,15,15,16,16,16,17,17,18,18,18,19,19,20,
  20,20,21,21,22,22,23,23,24,24,25,26,26,27,27,28,29,
  29,30,31,31,32,33,33,34,35,36,36,37,38,39,40,41,42,
  42,43,44,45,46,47,48,50,51,52,53,54,55,57,58,59,60,
  62,63,64,66,67,69,70,72,74,75,77,79,80,82,84,86,88,
  90,91,94,96,98,100,102,104,107,109,111,114,116,119,
  122,124,127,130,133,136,139,142,145,148,151,155,158,
  161,165,169,172,176,180,184,188,192,196,201,205,210,
  214,219,224,229,234,239,244,250,255
};

void setup()  { 
// Set the pre-scalers of timers to fast mode, for non-audible PWM (messes up the delay)
TCCR1B = (TCCR1B & 0b11111000) | 0b00000011; 

// set ledPin as output
pinMode (ledPin, OUTPUT); 

// turn on ledPin, also gates the transistor to be able to use the 8 LEDs
analogWrite(ledPin, 100);

// set the 8 LED pins as outputs
  for (int i = 0; i < 8; i++){ 
    pinMode(ledPinMapping[i], OUTPUT);
  };

// turn the 8 LED pins on, creates a little startup visual
  for (int i = 0; i <= 8; i++){ 
    digitalWrite(ledPinMapping[i], HIGH);
    delay(80);
  };
} 

void loop()  { 
  
// fade in from min to max in increments of 1 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, ledFadeTableFull[fadeValue]);         
    // wait for 30 milliseconds to see the dimming effect    
    sensor = analogRead(potiPin);
    fadeSpeed = (sensor * 4 + 0);
    delayMicroseconds(fadeSpeed);                            
  } 

// fade out from max to min in increments of 1 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=1) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, ledFadeTableFull[fadeValue]);         
    // wait for 30 milliseconds to see the dimming effect    
    sensor = analogRead(potiPin);
    fadeSpeed = (sensor * 4 + 0);
    delayMicroseconds(fadeSpeed);                            
  } 
}


