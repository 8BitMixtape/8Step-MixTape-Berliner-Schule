// *************************************************************
// Simple POV on the 8LEDs of the 8Step Mixtape Berliner Schule
// still under development.
//
// modified 30 April 2016 from something i found on the web..
// dusjagr for use with Attiny84 
// *************************************************************
//
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

const int ledPinMapping[8] = {9, 10, 8, 7, 0, 1, 2, 4};

int ledPin = 5;    // LED connected to digital pin8 -> PA5 or just 5
int potiPin = A3;    // Potentiometer connected to AnalogIn A3

//int a1,a2,a3,a4,a5,   b1,b2,b3,b4,b5,  c1,c2,c3,c4,c5,   d1,d2,d3,d4,d5,  e1,e2,e3,e4,e5,   f1,f2,f3,f4,f5,  g1,g2,g3,g4,g5,  h1,h2,h3,h4,h5 = 0; 
//int shape [] = {a1,b1,c1,d1,e1,f1,g1,h1,  a2,b2,c2,d2,e2,f2,g2,h2,  a3,b3,c3,d3,e3,f3,g3,h3,  a4,b4,c4,d4,e4,f4,g4,h4,  a5,b5,c5,d5,e5,f5,g5,h5};

unsigned char NUMBER9[]={1,1,1,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,1,1,1,1,1,1,1};
unsigned char NUMBER8[]={0,1,1,0,1,1,1,0,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  0,1,1,0,1,1,1,0};
unsigned char NUMBER7[]={1,0,0,0,0,0,0,0,  1,0,0,0,1,0,0,0,  1,0,0,0,1,0,0,0,  1,0,0,1,1,1,1,1,  1,1,1,0,1,0,0,0};
unsigned char NUMBER6[]={1,1,1,1,1,1,1,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,1,1,1};
unsigned char NUMBER5[]={1,1,1,1,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,1,1,1};
unsigned char NUMBER2[]= {1,0,0,0,0,0,1,1,  1,0,0,0,0,1,0,1,  1,0,0,0,1,0,0,1,  1,0,0,1,0,0,0,1,  0,1,1,0,0,0,0,1};
unsigned char NUMBER1[]= {0,0,1,0,0,0,0,0,  0,1,0,0,0,0,0,0,  1,1,1,1,1,1,1,1,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};
unsigned char NUMBER0[]= {1,1,1,1,1,1,1,1,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  1,1,1,1,1,1,1,1};

unsigned char _[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};
unsigned char A[] = {1,1,1,1,1,1,1,1,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0,  1,1,1,1,1,1,1,1};
unsigned char B[] = {1,1,1,1,1,1,1,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  0,1,1,0,1,1,1,0};
unsigned char C[] = {0,0,1,1,1,1,0,0,  0,1,0,0,0,0,1,0,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1};
unsigned char D[] = {1,1,1,1,1,1,1,1,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  0,1,0,0,0,0,1,0,  0,0,1,1,1,1,0,0};
unsigned char E[] = {1,1,1,1,1,1,1,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1,  1,0,0,1,0,0,0,1};
unsigned char F[] = {1,1,1,1,1,1,1,1,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0};
unsigned char G[] = {0,1,1,1,1,1,1,1,  1,0,0,0,0,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,1,1,0};
unsigned char H[] = {1,1,1,1,1,1,1,1,  0,0,0,0,1,0,0,0,  0,0,0,0,1,0,0,0,  0,0,0,0,1,0,0,0,  1,1,1,1,1,1,1,1};
unsigned char I[] = {1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  1,1,1,1,1,1,1,1,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1};
unsigned char J[] = {0,0,0,0,0,1,1,0,  0,0,0,0,1,0,0,1,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,0,1,  1,1,1,1,1,1,1,0};
unsigned char K[] = {1,1,1,1,1,1,1,1,  0,0,0,1,1,0,0,0,  0,0,1,0,0,1,0,0,  0,1,0,0,0,0,1,0,  1,0,0,0,0,0,0,1};
unsigned char L[] = {1,1,1,1,1,1,1,1,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,0,1};
unsigned char M[] = {1,1,1,1,1,1,1,1,  0,1,0,0,0,0,0,0,  0,0,1,0,0,0,0,0,  0,1,0,0,0,0,0,0,  1,1,1,1,1,1,1,1};
unsigned char N[] = {1,1,1,1,1,1,1,1,  0,0,1,0,0,0,0,0,  0,0,0,1,1,0,0,0,  0,0,0,0,0,1,0,0,  1,1,1,1,1,1,1,1};
unsigned char O[] = {0,1,1,1,1,1,1,0,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  1,0,0,0,0,0,0,1,  0,1,1,1,1,1,1,0};
unsigned char P[] = {1,1,1,1,1,1,1,1,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0,  1,0,0,1,0,0,0,0,  0,1,1,0,0,0,0,0};
unsigned char Q[] = {0,1,1,1,1,1,1,0,  1,0,0,0,0,0,0,1,  1,0,0,0,0,1,0,1,  0,1,1,1,1,1,1,0,  0,0,0,0,0,0,0,1};
unsigned char R[] = {1,1,1,1,1,1,1,1,  1,0,0,1,1,0,0,0,  1,0,0,1,0,1,0,0,  1,0,0,1,0,0,1,0,  0,1,1,0,0,0,0,1};
unsigned char S[] = {0,1,1,1,0,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,0,0,1,  1,0,0,0,1,1,1,0};
unsigned char T[] = {1,0,0,0,0,0,0,0,  1,0,0,0,0,0,0,0,  1,1,1,1,1,1,1,1,  1,0,0,0,0,0,0,0,  1,0,0,0,0,0,0,0};
unsigned char U[] = {1,1,1,1,1,1,1,0,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,0,1,  1,1,1,1,1,1,1,0};
unsigned char V[] = {1,1,1,1,1,1,0,0,  0,0,0,0,0,0,1,0,  0,0,0,0,0,0,0,1,  0,0,0,0,0,0,1,0,  1,1,1,1,1,1,0,0};
unsigned char W[] = {1,1,1,1,1,1,1,1,  0,0,0,0,0,0,1,0,  0,0,0,0,0,1,0,0,  0,0,0,0,0,0,1,0,  1,1,1,1,1,1,1,1};
unsigned char X[] = {1,1,0,0,0,0,1,1,  0,0,1,0,0,1,0,0,  0,0,0,1,1,0,0,0,  0,0,1,0,0,1,0,0,  1,1,0,0,0,0,1,1};
unsigned char Y[] = {1,1,0,0,0,0,0,0,  0,0,1,0,0,0,0,0,  0,0,0,1,1,1,1,1,  0,0,1,0,0,0,0,0,  1,1,0,0,0,0,0,0};
unsigned char Z[] = {1,0,0,0,0,1,1,1,  1,0,0,0,1,0,0,1,  1,0,0,1,0,0,0,1,  1,0,1,0,0,0,0,1,  1,1,0,0,0,0,0,1};

int y = 0;

// defining the space between the letters (ms)
int letterSpace = 2;

// defining the time dots appear (ms)
int dotTime = 1;

void setup() {
  
  //TCCR1B = (TCCR1B & 0b11111000) | 0b00000001; //pre-scaler, fast PWM

  pinMode (ledPin, OUTPUT); // set pin as output
  digitalWrite(ledPin, HIGH);
  
  //delay (100);
  
  // setting the ports of the leds to OUTPUT
  for (int i = 0; i < 8; i++){
    pinMode(ledPinMapping[i], OUTPUT);
  }

}

void printLetter(unsigned char letter[]) { 
  
  //digitalWrite(ledPin, HIGH);

  // printing the first y row of the letter
  for (y=0; y<8; y++) {digitalWrite(ledPinMapping[y], letter[y]); }
  delay(dotTime);
 
  // printing the second y row of the letter
  for (y=0; y<8; y++) {digitalWrite(ledPinMapping[y], letter[y+8]); }
  delay(dotTime);
 
  // printing the third y row of the letter
  for (y=0; y<8; y++) {digitalWrite(ledPinMapping[y], letter[y+16]); }
  delay(dotTime);
  
  // printing the forth y row of the letter
  for(y = 0; y<8; y++) {digitalWrite(ledPinMapping[y], letter[y+24]); }
  delay(dotTime);

  // printing the fifth y row of the letter
  for(y = 0; y<8; y++) {digitalWrite(ledPinMapping[y], letter[y+32]); }
  delay(dotTime);
  
  // printing the space between the letters
  for (y=0; y<8; y++) {digitalWrite(ledPinMapping[y], 0); }
  delay(letterSpace);

}

void loop() {   
  printLetter(NUMBER8);
  printLetter(B);
  printLetter(I);
  printLetter(T);
  //printLetter(A);
  //printLetter(M);
  //printLetter(I);
  //printLetter(X);
  //printLetter(T);
  //printLetter(A);
  //printLetter(P);
  //printLetter(E);
  digitalWrite(ledPin, LOW);
  delay(20);
  digitalWrite(ledPin, HIGH);
  //delay(100);

    /*
      case'A':
      shape [0] = 1;
      break;
      case'B':
      shape[1] = 1;
      break;
      case'C':
      shape[2] = 1;
      break;
      case'D':
      shape[3] = 1;
      break;
      case'E':
      shape[4] = 1;
      break;
      case'F':
      shape[5] = 1;
      break;
      case'G':
      shape[6] = 1;
      break;
      case'H':
      shape[7] = 1;
      break;
      case'I':
      shape[8] = 1;
      break;
      case'J':
      shape[9] = 1;
      break;
      case'K':
      shape[10] = 1;
      break;
      case'L':
      shape[11] = 1;
      break;
      case'M':
      shape[12] = 1;
      break;
      case'N':
      shape[13] = 1;
      break;
      case'O':
      shape[14] = 1;
      break;
      case'P':
      shape[15] = 1;
      break;
      case'Q':
      shape[16]= 1;
      break;
      case'R':
      shape[17] = 1;
      break;
      case'S':
      shape[18] = 1;
      break;
      case'U':
      shape[19] = 1;
      break;
      case'T':
      shape[20] = 1;
      break;
      case'V':
      shape[21] = 1;
      break;
      case'W':
      shape[22] = 1;
      break;
      case'X':
      shape[23] = 1;
      break;
      case'Y':
      shape[24] = 1;
      break;
      case'Z':
      shape[25] = 1;
      break;
      case'1':
      shape[26] = 1;
      break;
      case'3':
      shape[27] = 1;
      break;
      case'2':
      shape[28] = 1;
      break;
      case'4':
      shape[29] = 1;
      break;
      case'5':
      shape[30] = 1;
      break;
      case'6':
      shape[31] = 1;
      break;
      case'7':
      shape[32] = 1;
      break;
      case'8':
      shape[33] = 1;
      break;
      case'9':
      shape[34]= 1;
      break;
      case'b':
      shape[35] = 1;
      break;
      case'a':
      shape[36] = 1;
      break;
      case'c':
      shape[37] = 1;
      break;
      case'd':
      shape[38] = 1;
      break;
      case'e':
      shape[39] = 1;
      break;

    }

*/

}
