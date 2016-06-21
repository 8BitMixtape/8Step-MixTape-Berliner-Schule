/*
*This code was for an attiny85 powered pocket sequencer and is part of an
*instructable at http://www.instructables.com/id/Attiny-Pocket-Sequencer/ . 
*The following code was written by: Adam Berger
*You are free to use my code in anyway you'd like, as long as
*you give credit where it is due. Thank you for your interest!
*
* Modified by dusjagr and stahl to run on the attiny84 for the 8Step MixTape | Berliner Schule
* CC 2015
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

const byte pot = A3;
const byte gatePin = 6; 
const byte controlVoltagePin = 5;
const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4}; // change this if you insiston using more than 8 steps
const int MAX_NOTE_LENGTH = 32000;
const int MAX_FREQ = 255; 
const int POT_THRESHOLD = 35;
const int NUMBER_OF_STEPS = 8;

//int stepFreqs[] = {255,128,230,30,0,240,50,0,200,100,200,0,160,20,130,60};
int stepFreqs[] = {100,110,120,130,140,150,160,170};
//int stepFreqs[] = {255,128,230,130,180,240,150,90};

// Step Lenthg is a proportion of the full step between 0-16
int stepLengths[] = {14,14,14,14,14,14,14,14};
//int stepLengths[] = {16,16,16,16,16,16,16,16};
//int stepLengths[] = {15,3,2,1,15,2,12,0};
int tempo = 12000;
int progStep = -1;

int buttonState = 0;         // current state of the button
int lastButtonState = 1;     // previous state of the button

unsigned long previousMillis, functionMillis, longpressMillis;

// different modes
const int TEMPO = 2;
const int PITCH = 0;
const int GATE = 1;
int mode = TEMPO;    // current mode

int oldValue = -1; // value for pot

void setup(){
  //enable inpustepFreqsts
  pinMode(pot, INPUT);
  digitalWrite(pot, HIGH);
  //enable outputs
  //pinMode(tonePin, OUTPUT);
  pinMode(gatePin, OUTPUT);
  digitalWrite(gatePin, LOW);
   
  for (int i = 0; i < NUMBER_OF_STEPS; i++){
    pinMode(ledPinMapping[i], OUTPUT);
  };
  
  // start-up Sequence
  // turns CV on, so there will be a peeeeeep!!
  analogWrite(controlVoltagePin, 255);
  
  for (int i = 0; i <= NUMBER_OF_STEPS; i++){
    digitalWrite(ledPinMapping[i], HIGH);
    delay(30);
  };
  for (int i = 0; i <= NUMBER_OF_STEPS; i++){
    digitalWrite(ledPinMapping[i], LOW);
    delay(30);
  };
  
  analogWrite(controlVoltagePin, 0);
  
  // End of the start-up Sequence and wait a bit
  delay(400);
  // Set the pre-scalers of timers to fast mode, for non-audible PWM (messes up the delay)
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000001;
  TCCR0B = (TCCR0B & 0b11111000) | 0b00000001;
  
  // Go into edit mode of each step
  setFrequencies();
  mode = TEMPO; 

}

void loop(){
  
  //Do this for each step
    for(byte a=0; a<NUMBER_OF_STEPS; a++){
    //turn LED on
    digitalWrite(ledPinMapping[a], HIGH);
    //output Trigger / Gate signal HIGH
    digitalWrite(gatePin, HIGH);
    
    //tempo logic/update tempo variable
    previousMillis=millis();
    
    //output PWM / CV signal
    analogWrite(controlVoltagePin, (stepFreqs[a]));
    
    //output Trigger / Gate signal LOW
    //digitalWrite(gatePin, LOW);
     
    while(millis()-previousMillis<tempo){
      if (mode == TEMPO) {
        digitalWrite(ledPinMapping[progStep], LOW);
        progStep = -1;
      }
      else {
        digitalWrite(ledPinMapping[progStep], HIGH);
      }
      
      int val = analogRead(pot);
      
      // if button is pressed short
      if(val<POT_THRESHOLD){
        if(millis()-longpressMillis<20000){
          buttonState = 0;
          functionMillis=millis();
        }
        else { // long pressed button
          mode = TEMPO;
          digitalWrite(ledPinMapping[progStep], LOW);
        }
      }

      else if(val>POT_THRESHOLD&&(millis()-functionMillis>500)) {
        
        buttonState = 1;
        if (val < oldValue - 50 || val > oldValue + 50) {
          oldValue = val;
          switch (mode) {
            case TEMPO:
              tempo = map(val,POT_THRESHOLD ,1023,MAX_NOTE_LENGTH,20);
              break;
            case GATE:
              stepLengths[progStep]= map(val,POT_THRESHOLD,1023,0,16);
              break;
            case PITCH:
              stepFreqs[progStep] = map(val,POT_THRESHOLD,1023,0,MAX_FREQ);
              break;
            default:
              break;
          };
        }
      longpressMillis=millis();  
      }
      
      // Check if button has just now been pressed
      if (buttonState != lastButtonState && buttonState == 0) {       
        digitalWrite(ledPinMapping[progStep], LOW);  
        progStep = progStep + 1;
        
      // Count the prog Steps and switch to other programming modes
        if (progStep % NUMBER_OF_STEPS == 0) {
          mode = mode + 1; mode = mode % 2; progStep = 0;
        }
      }
        
      lastButtonState = buttonState;
      
      // Check if step Lentgh time has passed, if so, turn off Gate and put CV to 0
      if(millis()-previousMillis>(stepLengths[a]*(tempo/16))){
        analogWrite(controlVoltagePin, 0);
        digitalWrite(gatePin, LOW);
      }
    }
    
    //turn LED off
    digitalWrite(ledPinMapping[a], LOW);
  }
}

// start programming mode. is only done once!

void setFrequencies(){
  
  for(byte a=0; a<NUMBER_OF_STEPS; a++){
    //turn LED on
    digitalWrite(ledPinMapping[a], HIGH);
    //tempo logic/update tempo variable
    while((analogRead(pot)>POT_THRESHOLD)){
     previousMillis=millis();
     int freq = map(analogRead(pot),POT_THRESHOLD,1023,0,MAX_FREQ);
     
     for (int i = 0; i <= freq>>5; i++){
       digitalWrite(ledPinMapping[i], HIGH);
       digitalWrite(ledPinMapping[a], LOW);
       delay(20);
       digitalWrite(ledPinMapping[a], HIGH);
       delay(20);
     };
     
     for (int i = (freq>>5)+1; i < NUMBER_OF_STEPS; i++){
       digitalWrite(ledPinMapping[i], LOW); 
       digitalWrite(ledPinMapping[a], HIGH);
       delay(20);
       digitalWrite(ledPinMapping[a], LOW);
       delay(20);
     };
     
     if(analogRead(pot)<POT_THRESHOLD+20) freq=0;
     stepFreqs[a] = freq;
     
     analogWrite(controlVoltagePin, stepFreqs[a]);
     //tone(tonePin, stepFreqs[a], stepLenghts[a]);
     
     functionMillis=millis();
     while(millis()-functionMillis<3000){
      if(analogRead(pot)<POT_THRESHOLD) {
        /*
        for (int i = 0; i <= NUMBER_OF_STEPS; i++){
        digitalWrite(ledPinMapping[i], LOW);
        };
        */
        break;
      }
    }
    }
  delay(5000);
  digitalWrite(ledPinMapping[a], LOW);
  delay(10000);
  

  }
    for (int i = 0; i <= NUMBER_OF_STEPS; i++){
    digitalWrite(ledPinMapping[i], LOW);
  };
  functionMillis=millis();
}
