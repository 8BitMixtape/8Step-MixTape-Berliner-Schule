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

const byte pot = A3, tonePin = 6, clock = 6, gatePin = 6, controlVoltagePin = 5;
//const int ledPinMapping[16] = {1, 2, 0, 4, 6, 8, 9, 10, 1, 2, 0, 4, 6, 8, 9, 10};
const int ledPinMapping[16] = {9, 10, 8, 7, 0, 1, 2, 4};
const int MAX_NOTE_LENGTH = 32000, MAX_FREQ = 255, NUMBER_OF_STEPS = 8, POT_THRESHOLD = 35;
//int stepFreqs[] = {255,128,230,30,0,240,50,0,200,100,200,0,160,20,130,60};
int stepFreqs[] = {100,100,100,100,100,100,100,100};
//int stepSustains[] = {8200,1000,3420,0,3000,1620,8400,7000,8200,882,842,860,820,862,840,700};
int stepSustains[] = {16,16,16,16,16,16,16,16};
//int stepSustains[] = {15,3,10,1,15,2,12,0};
int tempo = 1000;

unsigned long previousMillis, functionMillis;

void setup(){
  //enable inpustepFreqsts
  pinMode(pot, INPUT);
  digitalWrite(pot, HIGH);
  //enable outputs
  //pinMode(tonePin, OUTPUT);
  pinMode(gatePin, OUTPUT);
  digitalWrite(gatePin, LOW);
  
  analogWrite(controlVoltagePin, 255);
   
  for (int i = 0; i < NUMBER_OF_STEPS; i++){
    pinMode(ledPinMapping[i], OUTPUT);
  };
  
  for (int i = 0; i <= NUMBER_OF_STEPS; i++){
    digitalWrite(ledPinMapping[i], HIGH);
    delay(30);
  };
  for (int i = 0; i <= NUMBER_OF_STEPS; i++){
    digitalWrite(ledPinMapping[i], LOW);
    delay(30);
  };
  
  analogWrite(controlVoltagePin, 0);

  delay(400);
  //setFrequencies();
  //functionMillis=millis();
  //setFrequencies();
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000001;
  TCCR0B = (TCCR0B & 0b11111000) | 0b00000001;
  setFrequencies();

}

void loop(){
  //for each step
  
  for(byte a=0; a<NUMBER_OF_STEPS; a++){
    //turn LED on
    //digitalWrite(clock, LOW);
    digitalWrite(ledPinMapping[a], HIGH);
    
    //tempo logic/update tempo variable
    previousMillis=millis();
    
    //output Trigger / Gate signal HIGH
    digitalWrite(gatePin, HIGH);
    //output PWM / CV signal
    analogWrite(controlVoltagePin, (stepFreqs[a]));
    //output Trigger / Gate signal LOW
    digitalWrite(gatePin, LOW);
     
    while(millis()-previousMillis<tempo){      
      tempo = map(analogRead(pot),POT_THRESHOLD ,1023,MAX_NOTE_LENGTH,80);

      if(millis()-previousMillis>(stepSustains[a]*(tempo/16))){
        analogWrite(controlVoltagePin, 0);
        //pinMode(gatePin, OUTPUT);
        //digitalWrite(gatePin, LOW);
      }
      
    }  
  
  digitalWrite(ledPinMapping[a], LOW);
  
  }
}

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
     //tone(tonePin, stepFreqs[a], stepSustains[a]);
     
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

