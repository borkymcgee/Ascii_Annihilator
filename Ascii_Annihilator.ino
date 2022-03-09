void displayChar(byte digit, char dChar, bool pretty = true);
void showAscii(bool nonPrinting = false);
void setButtons(byte buttonStates, bool ignoreSwitch = false);
byte getButtons(bool ignoreSwitch = false);

//pins that the buttons are connected to
const int button_input_pin_bit0 = A3;
const int button_input_pin_bit1 = A2;
const int button_input_pin_bit2 = A1;
const int button_input_pin_bit3 = A0;

//pin that the nibble select switch is connected to
const int nibbleSwitchPin = 8;

//how long in ms to wait to debounce button presses
const int debounceTime = 200;

//game mode, set by holding buttons on startup
//none: main game mode, all ascii
//bit 3: printing characters only - NOT IMPLEMENTED
//bit 0: attract mode, cycle through all ascii
//bit 3 and 2: look mode, tell you the char corresponding to whatever binary you put in
//bit 1 and 0: table mode, let you browse
//2 = cycle through ascii with the buttons
byte gameMode = 0;

//variables used different ways for different modes
byte gameByteA = 0;
byte gameByteB = 0;

//9 frames, 2 for each digit and one for button leds
byte frameCube[10][6];

//tracks the current frame
int currentFrame = 0;

void setup() {
  randomSeed(analogRead(A3));
  //set up timers - for display and for dimming LEDs (planned)
  timerSetup();
  //initialize frameCube to be empty
  clearCube();
  //set game mode
  delay(100);
  gameMode = getButtons(true);

//set up a random number for if a game mode is picked
  gameByteB = random(0,128);

  //display the mode selected, sometimes do extra setup
  switch(gameMode){
    case 0b00001000:
      //alphanumeric characters only
      displayString("WIMP");
      gameByteB = wimpRandom();
      break;
    case 0b00000100:
      //decimal only
      displayString("dec ");
      break;
    case 0b00000010:
      //hexidecimal only
      displayString("hex ");
      break;
    case 0b00001100:
      //displays the char input on the buttons, lookup mode
      displayString("look");
      break;
    case 0b00000011:
      //browse the ascii table
      displayString("tabl");
      break;
    case 0b00000001:
      //cycle through all chars, attract mode
      displayString("attc");
      break;
    default:
      //type the correct byte for the given char
      displayString("anL8");
      break;
  }
  delay(500);
  displayString("mode");
  delay(500);
  clearCube();
}

void loop(){
  //run the selected mode, this probably doesn't need to go in loop();
  switch(gameMode){
    case 0b00001100:
      //displays the char input on the buttons
      lookMode();
      break;
    case 0b00000011:
      //cycle through chars with buttons
      tableMode();
      break;
    case 0b00000001:
      //cycle through all ascii characters, attract mode
      showAscii(true);
      break;
    default:
      //main game mode
      annihilateMode();
      break;
  }
}

//main game
//goal is stored in gameByteB
//buttons being pressed are stored in gameByteA
void annihilateMode(){
  
  //place to store output of itoa
  char itoa_result[4];
  
  //display the char to guess
  //print a leading char to indicate the base requested,
  //then convert the guess to the appropriate string to display it
  if(bitRead(gameMode,2)){  //decimal
    displayChar(0,'d');
    itoa(gameByteB,itoa_result,10);
  }else if(bitRead(gameMode,1)){  //hex
    displayChar(0,'x');
    itoa(gameByteB,itoa_result,16);
  }else{  //ascii
    displayChar(0,'a');
    //no conversion neccesary, just plop the guess in
    itoa_result[0] = gameByteB;

    //manually terminate the string early
    itoa_result[1] = 0;
  }

  //itoa has no leading spaces, and the length depends on the number of digits it outputs, so we need to manually right-justify it within the display
  if(itoa_result[1] == 0){ //1 digit number
    displayChar(3,itoa_result[0],false);
  }else if(itoa_result[2] == 0){ //2 digit number
    displayChar(2,itoa_result[0],false);
    displayChar(3,itoa_result[1],false);
  }else{  //3 digit number
    displayChar(1,itoa_result[0],false);
    displayChar(2,itoa_result[1],false);
    displayChar(3,itoa_result[2],false);
  }
  
  //track if the buttons change
  byte oldState = gameByteA;
  gameByteA = gameByteA ^= getButtons();
  //if they do, wait to debounce
  if(gameByteA != oldState){
    delay(debounceTime);
  }
  //display the gurrent state of the player's guess
  setButtons(gameByteA);

  //bit 7 is the 'give up' button
  if(bitRead(gameByteA,7)){
    //show the player what the answer is until they press a button
    while(!getButtons()){
      setButtons(gameByteB);
    }
    displayString("Good");
    delay(500);
    displayString("Try!");
    delay(500);
    clearCube();
    gameByteA = 0;
    if(gameMode == 0b00001000){
      gameByteB = wimpRandom();
    }else{
      gameByteB = random(0,128);
    }
  }

  //is the guess correct?
  if(gameByteA == gameByteB){
    if(gameMode == 0b00001000) displayString("Okay");
    else displayString("Good");
    delay(500);
    displayString("Job!");
    delay(500);
    clearCube();
    gameByteA = 0;
    if(gameMode == 0b00001000){
      gameByteB = wimpRandom();
    }else{
      gameByteB = random(0,128);
    }
  }
}

//return a random alphanumeric (not symbol) character
char wimpRandom(){
  switch(random(0,3)){
    case 0:
      return random(48,58);
      break;
    case 1:
      return random(65,91);
      break;
    case 2:
      return random(97,123);
      break;
  }
}

//displays whatever character is input on the buttons
void lookMode(){
  //only update when needed (might be unnecessary)
  byte oldState = gameByteA;
  gameByteA = gameByteA ^= getButtons();
  if(gameByteA != oldState){
    delay(debounceTime);
    clearCube();
  }
  setButtons(gameByteA);
  //display the character on the buttons
  displayChar(3,gameByteA,false);
}

//browse a virtual ascii table, the inner buttons go back and forth slowly, the outer buttons go back and forth fast
void tableMode(){
  if(getButtons(true) == 0b00000010){
    gameByteA++;
    delay(debounceTime);
    clearCube();
  }else if(getButtons(true) == 0b00000001){
    gameByteA++;
    delay(debounceTime/4);
    clearCube();
  }else if(getButtons(true) == 0b00000100){
    gameByteA--;
    delay(debounceTime);
    clearCube();
  }else if(getButtons(true) == 0b00001000){
    gameByteA--;
    delay(debounceTime/4);
    clearCube();
  }
  if(gameByteA > 127 && gameByteA < 192) gameByteA = 0;
  if(gameByteA > 192) gameByteA = 127;
  displayChar(3,gameByteA,false);
  setButtons(gameByteA);
}

//sets up timers used for display and button LED driving (Latter is planned)
void timerSetup(){
  cli();
  TCCR1A = 0;
  TCCR1B = 0b00000100;
  TIMSK1 = 0b00000010;
  OCR1A = 64;
  TCNT1 = 0;
  sei();
}

//update display asynchronously
ISR(TIMER1_COMPA_vect){
  //reset timer
  TCNT1 = 0;

  //overflow at 8
  if(currentFrame > 8) currentFrame = 0;
  else currentFrame++;

  //set all the ports according to the state on the current frame in the framecube
  PORTB = frameCube[currentFrame][0];
  PORTC = frameCube[currentFrame][1];
  PORTD = frameCube[currentFrame][2];
  DDRB = frameCube[currentFrame][3];
  DDRC = frameCube[currentFrame][4];
  DDRD = frameCube[currentFrame][5];
}

//code to test display, shows all ascii characters on the display one after another
void showAscii(bool nonPrinting){
  int start = 33;
  if(nonPrinting) start = 0;
  for(byte c = start; c<125; c++){
    //clearCube();
    if(nonPrinting) displayChar(3,c,false);
    else{
      displayChar(0,c);
      displayChar(1,c);
      displayChar(2,c);
      displayChar(3,c);
    }
    setButtons(c);
    delay(200);
  }
}

//sets the frame cube to an empty state, I.E. a totally blank display
void clearCube(){
  //all of the frames have mostly the same clear state, so set that
  //also set the button LEDs' anode pins to always be high-Z
  //also set the button's and switch's input pins to pullup
  for(byte frame=0; frame < 10; frame++){
    frameCube[frame][0] = 0b10111001; //PORTB
    frameCube[frame][1] = 0b00111111; //PORTC
    frameCube[frame][2] = 0b00111101; //PORTD
    frameCube[frame][3] = 0b11110110; //DDRB
    frameCube[frame][4] = 0b11000000; //DDRC
    frameCube[frame][5] = 0b11111110; //DDRD
  }
 
  //except the pins for the specific subdigits (1A-4B)
  //OR combine those in
  frameCube[0][2] |= 0b00000010; //1A (D1)
  frameCube[1][2] |= 0b00000001; //1B (D0)
  frameCube[2][0] |= 0b00000010; //2A (B1)
  frameCube[3][1] |= 0b00010000; //2B (C4)
  frameCube[4][0] |= 0b01000000; //3A (B6)
  frameCube[5][0] |= 0b00000100; //3B (B2)
  frameCube[6][0] |= 0b00001000; //4A (B3)
  frameCube[7][2] |= 0b10000000; //4B (D7)
  frameCube[8][1] |= 0b00100000; //DN (C5)
  
  //also the special button frame
  frameCube[9][3] |= 0b00001000; //b4 at BN, B3
  frameCube[9][4] |= 0b00110000; //b2,3 at BN, C4,5
  frameCube[9][5] |= 0b00000001; //b1 at BN, D0

  //set the buttons LEDs' anodes to not be high-Z on specific frames when they are needed for the display
  frameCube[1][5] |= 0b00000001; //b1 at 1B, D0
  frameCube[3][4] |= 0b00010000; //b2 at 2B, C4
  frameCube[8][4] |= 0b00100000; //b3 at DN, C5
  frameCube[6][3] |= 0b00001000; //b4 at 4A, B3
  
  //frameCube[9][0] ^= 0b00001000;
}

byte getButtons(bool ignoreSwitch){
  byte buttonValue = 0;
  if(!digitalRead(button_input_pin_bit0)) buttonValue |= (1<<0);
  if(!digitalRead(button_input_pin_bit1)) buttonValue |= (1<<1);
  if(!digitalRead(button_input_pin_bit2)) buttonValue |= (1<<2);
  if(!digitalRead(button_input_pin_bit3)) buttonValue |= (1<<3);
  if(digitalRead(nibbleSwitchPin) & !ignoreSwitch) buttonValue = buttonValue << 4;
  return buttonValue;
}

//sets the button lights to the value in the nibble of buttonValue-
// -that the switch is set to.
void setButtons(byte buttonStates, bool ignoreSwitch){
  
  if(digitalRead(nibbleSwitchPin) & !ignoreSwitch) buttonStates = buttonStates >> 4;
  //                to bit v                        v from bit  
  bitWrite(frameCube[9][2],0,!bitRead(buttonStates, 3));//b1, set D0 low
  bitWrite(frameCube[9][1],4,!bitRead(buttonStates, 2));//b2, set C4 low
  bitWrite(frameCube[9][1],5,!bitRead(buttonStates, 1));//b3, set C5 low
  bitWrite(frameCube[9][0],3,!bitRead(buttonStates, 0));//b4, set B3 low 
}

//sets the specified segments on for the specified subdigit
//segments: segA:A,B,D,E,F,G1,CN   segB:G2,H,J,K,L,M,N,DP
void setSegments(byte digit, byte segA, byte segB){
  
  //                      to bit v                v from bit
  bitWrite(frameCube[digit*2][2],2,!bitRead(segA, 7)); //A, SA7 to D2
  bitWrite(frameCube[digit*2][2],3,!bitRead(segA, 6)); //B, SA6 to D3
  bitWrite(frameCube[digit*2][2],4,!bitRead(segA, 5)); //C, SA5 to D4
  bitWrite(frameCube[digit*2][2],5,!bitRead(segA, 4)); //D, SA4 to D5
  bitWrite(frameCube[digit*2][0],4,!bitRead(segA, 3)); //E, SA3 to B4
  bitWrite(frameCube[digit*2][0],7,!bitRead(segA, 2)); //F, SA2 to B7
  bitWrite(frameCube[digit*2][0],5,!bitRead(segA, 1)); //G1, SA1 to B5
  //OR combine the colon input so as not to overwrite other digits that need it
  bitWrite(frameCube[8][2],2,!(bitRead(segA, 0)|!bitRead(frameCube[8][2],2))); //CN, SA0 to D2
  
  bitWrite(frameCube[digit*2+1][2],2,!bitRead(segB, 7)); //G2, SB7 to D2
  bitWrite(frameCube[digit*2+1][2],3,!bitRead(segB, 6)); //H, SA6 to D3
  bitWrite(frameCube[digit*2+1][2],4,!bitRead(segB, 5)); //J, SA5 to D4
  bitWrite(frameCube[digit*2+1][2],5,!bitRead(segB, 4)); //K, SA4 to D5
  bitWrite(frameCube[digit*2+1][0],4,!bitRead(segB, 3)); //L, SA3 to B4
  bitWrite(frameCube[digit*2+1][0],7,!bitRead(segB, 2)); //M, SA2 to B7
  bitWrite(frameCube[digit*2+1][0],5,!bitRead(segB, 1)); //N, SA1 to B5
  //OR combine the decimal input so as not to overwrite other digits that need it
  bitWrite(frameCube[8][2],3,!(bitRead(segB, 0)|!bitRead(frameCube[8][2],3))); //DP, SA0 to D3
}

//display the given 4-character ASCII string on the display
void displayString(const char* dString){
  displayChar(0,dString[0]);
  displayChar(1,dString[1]);
  displayChar(2,dString[2]);
  displayChar(3,dString[3]);
}


//display the specified char at the specified digit on the display
//pretty makes the letters look pretty, but non-unique? also it makes the space a printing character
void displayChar(byte digit, char dChar, bool pretty){
  switch(dChar){
    //if asked to display a non-printing character, print its 3-letter abbreviation
    case 0:
      displayString(" NUL");
      break;
    case 1:
      displayString(" SOH");
      break;
    case 2:
      displayString(" STX");
      break;
    case 3:
      displayString(" ETX");
      break;
    case 4:
      displayString(" EOT");
      break;
    case 5:
      displayString(" ENQ");
      break;
    case 6:
      displayString(" ACK");
      break;
    case 7:
      displayString(" BEL");
      break;
    case 8:
      displayString("  BS");
      break;
    case 9:
      displayString(" TAB");
      break;
    case 10:
      displayString("  LF");
      break;
    case 11:
      displayString("  VT");
      break;
    case 12:
      displayString("  FF");
      break;
    case 13:
      displayString("  CR");
      break;
    case 14:
      displayString("  SO");
      break;
    case 15:
      displayString("  SI");
      break;
    case 16:
      displayString(" DLE");
      break;
    case 17:
      displayString(" DC1");
      break;
    case 18:
      displayString(" DC2");
      break;
    case 19:
      displayString(" DC3");
      break;
    case 20:
      displayString(" DC4");
      break;
    case 21:
      displayString(" NAK");
      break;
    case 22:
      displayString(" SYN");
      break;
    case 23:
      displayString(" ETB");
      break;
    case 24:
      displayString(" CAN");
      break;
    case 25:
      displayString("  EM");
      break;
    case 26:
      displayString(" SUB");
      break;
    case 27:
      displayString(" ESC");
      break;
    case 28:
      displayString("  FS");
      break;
    case 29:
      displayString("  GS");
      break;
    case 30:
      displayString("  RS");
      break;
    case 31:
      displayString("  US");
      break;
    case ' ':
      if(pretty){
        //                   ABCDEFGCn   GHJKLMNDp
        setSegments(digit, 0b00000000, 0b00000000);
      }else{
        displayString("  SP");
      }
      break;
    case '!':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001100, 0b00000001);
      break;
    case '"':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01000000, 0b00100000);
      break;
    case '#':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01110010, 0b10100100);
      break;
    case '$':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10110110, 0b10100100);
      break;
    case '%':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00100110, 0b11011010);
      break;
    case '&':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10011010, 0b01011000);
      break;
    case '\'':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00010000);
      break;
    case '(':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00011000);
      break;
    case ')':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b01000010);
      break;
    case '*':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000010, 0b11111110);
      break;
    case '+':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000010, 0b10100100);
      break;
    case ',':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00000010);
      break;
    case '-':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000010, 0b10000000);
      break;
    case '.':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00000010);
      break;
    case '/':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00010010);
      break;
    case '0':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11111100, 0b00010010);
      break;
    case '1':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01100000, 0b00010000);
      break;
    case '2':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11011010, 0b10000000);
      break;
    case '3':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11110010, 0b10000000);
      break;
    case '4':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01100110, 0b10000000);
      break;
    case '5':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10010110, 0b00001000);
      break;
    case '6':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10011110, 0b00001000);
      break;
    case '7':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10000010, 0b10010100);
      break;
    case '8':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11111110, 0b10000000);
      break;
    case '9':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11110110, 0b10000000);
      break;
    case ':':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000001, 0b00000000);
      break;
    case ';':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00100010);
      break;
    case '<':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010000, 0b00010010);
      break;
    case '=':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010010, 0b10000000);
      break;
    case '>':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010000, 0b01001000);
      break;
    case '?':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11000100, 0b10000101);
      break;
    case '@':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11011100, 0b10100000);
      break;
    case 'A':
      //                   ABCDEFGCn   GHJKLMNDp
    setSegments(digit, 0b11101110, 0b10000000);
      break;
    case 'B':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10011110, 0b00011000);
      break;
    case 'C':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10011100, 0b00000000);
      break;
    case 'D':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11110000, 0b00100100);
      break;
    case 'E':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10011110, 0b10000000);
      break;
    case 'F':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10001110, 0b00000000);
      break;
    case 'G':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10111100, 0b10000000);
      break;
    case 'H':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01101110, 0b10000000);
      break;
    case 'I':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10010000, 0b00100100);
      break;
    case 'J':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01111000, 0b00000000);
      break;
    case 'K':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001110, 0b00011000);
      break;
    case 'L':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00011100, 0b00000000);
      break;
    case 'M':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01101100, 0b01010000);
      break;
    case 'N':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01101100, 0b00010010);
      break;
    case 'O':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11111100, 0b00000000);
      break;
    case 'P':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11001110, 0b10000000);
      break;
    case 'Q':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11111100, 0b00001000);
      break;
    case 'R':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11001110, 0b10001000);
      break;
    case 'S':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10110110, 0b10000000);
      break;
    case 'T':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10000000, 0b00100100);
      break;
    case 'U':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01111100, 0b00000000);
      break;
    case 'V':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001100, 0b00010010);
      break;
    case 'W':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01101100, 0b00001010);
      break;
    case 'X':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b01011010);
      break;
    case 'Y':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b01010100);
      break;
    case 'Z':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10010000, 0b00010010);
      break;
    case '[':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10010010, 0b01000010);
      break;
    case '\\':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b01001000);
      break;
    case ']':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10010000, 0b10011000);
      break;
    case '^':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01000000, 0b00010000);
      break;
    case '_':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010000, 0b00000000);
      break;
    case '`':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b01000000);
      break;
    case 'a':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00011010, 0b00000100);
      break;
    case 'b':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00111110, 0b10000000);
      break;
    case 'c':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00011010, 0b10000000);
      break;
    case 'd':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01111010, 0b10000000);
      break;
    case 'e':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00011010, 0b00000010);
      break;
    case 'f':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b10010100);
      break;
    case 'g':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11110000, 0b11000000);
      break;
    case 'h':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00101110, 0b10000000);
      break;
    case 'i':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00000100);
      break;
    case 'j':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001000, 0b00100010);
      break;
    case 'k':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00111100);
      break;
    case 'l':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010000, 0b00100100);
      break;
    case 'm':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00101010, 0b10000100);
      break;
    case 'n':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001010, 0b00000100);
      break;
    case 'o':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00111010, 0b10000000);
      break;
    case 'p':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10001110, 0b00010000);
      break;
    case 'q':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b11100000, 0b11000000);
      break;
    case 'r':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001010, 0b00000000);
      break;
    case 's':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010000, 0b10001000);
      break;
    case 't':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00011110, 0b00000000);
      break;
    case 'u':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00111000, 0b00000000);
      break;
    case 'v':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00001000, 0b00000010);
      break;
    case 'w':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00101000, 0b00001010);
      break;
    case 'x':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000010, 0b10001010);
      break;
    case 'y':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b01110000, 0b10100000);
      break;
    case 'z':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00010010, 0b00000010);
      break;
    case '{':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b10011000);
      break;
    case '|':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000000, 0b00100100);
      break;
    case '}':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b00000010, 0b01000010);
      break;
    case '~':
      //                   ABCDEFGCn   GHJKLMNDp
      setSegments(digit, 0b10000000, 0b00000000);
      break;
    case 127:
      displayString(" DEL");
      break;
  }
}
