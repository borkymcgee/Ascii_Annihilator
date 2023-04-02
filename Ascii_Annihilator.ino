/**************************************************************************
* Ascii Annihilator, Copyright (C) 2022 Juno Presken
* E-mail: juno.presken+github@protonmail.com
*      
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or (at your
* option) any later version.

* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
* Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>. 
* 
**************************************************************************/

#include <EEPROM.h>
#include <avr/power.h>
void displayChar(byte digit, char dChar, bool pretty = true);
void showAscii(bool nonPrinting = false);
void setButtons(byte currentGuesss, bool ignoreSwitch = false);
byte getButtons(bool ignoreSwitch = false);
void setupGuesses(bool describe = true);

//pins that the buttons are connected to
const int button_input_pin_bit0 = A3;
const int button_input_pin_bit1 = A2;
const int button_input_pin_bit2 = A1;
const int button_input_pin_bit3 = A0;

//arrays to hold the set of guesses and be shuffled by Knuth-Fisher-Yates
int decGuesses[256];
int hexGuesses[256];
int asciiGuesses[128];

//tracks progress through the guess array
int decGuessIndex = 0;
int hexGuessIndex = 0;
int asciiGuessIndex = 0;

//pin that the nibble select switch is connected to
const int nibbleSwitchPin = 8;

//how long in ms to wait to debounce button presses
const int debounceTime = 200;

//game mode, set by holding buttons on startup
//none: full, quiz on full ascii, hex, and dec
//bit 3: include ascii in guesses
//bit 2: include dec in guesses
//bit 1: include hex in guesses
//bit 0: wimp mode, only quiz hex up to 15, dec up to 15, and alphanumeric ascii only
//all bits held: expert mode, quiz only on ascii, and only non-printing and symbols
byte gameMode = 0;

//tracks whether wimp mode is enabled
bool wimpMode = false;

//tracks the current state of the user's guess
byte currentGuess = 0;

//holds the correct guess for the current round
byte correctGuess = 0;

//framebuffer for the display and button leads
//has 9 frames, 2 for each digit and one for button leds
byte frameCube[10][6];

//enable alternate wimp mode (in wimp mode, switch nibble switch HIGH to quiz on only the upper nibble)
bool aWimp = false;

//ascii display lookup table, stored in EEPROM to save space
const byte asciiSegs[188] PROGMEM = {
                           //abcdefg:   g2hjklmn.
                           0b00001100, 0b00000001, //!
                           0b01000000, 0b00100000, //"
                           0b01110010, 0b10100100, //#
                           0b10110110, 0b10100100, //$
                           0b00100110, 0b11011010, //%
                           0b10011010, 0b01011000, //&
                           0b00000000, 0b00010000, //'
                           0b00000000, 0b00011000, //(
                           0b00000000, 0b01000010, //)
                           0b00000010, 0b11111110, //*
                           0b00000010, 0b10100100, //+
                           0b00000000, 0b00000010, //,
                           0b00000010, 0b10000000, //-
                           0b00000000, 0b00000001, //.
                           0b00000000, 0b00010010, ///
                           0b11111100, 0b00010010, //0
                           0b01100000, 0b00010000, //1
                           0b11011010, 0b10000000, //2
                           0b11110010, 0b10000000, //3
                           0b01100110, 0b10000000, //4
                           0b10010110, 0b00001000, //5
                           0b10011110, 0b00001000, //6
                           0b10000010, 0b10010100, //7
                           0b11111110, 0b10000000, //8
                           0b11110110, 0b10000000, //9
                           0b00000001, 0b00000000, //:
                           0b00000000, 0b00100010, //;
                           0b00010000, 0b00010010, //<
                           0b00010010, 0b10000000, //=
                           0b00010000, 0b01001000, //>
                           0b11000100, 0b10000101, //?
                           0b11011100, 0b10100000, //@
                           0b11101110, 0b10000000, //A
                           0b10011110, 0b00011000, //B
                           0b10011100, 0b00000000, //C
                           0b11110000, 0b00100100, //D
                           0b10011110, 0b10000000, //E
                           0b10001110, 0b00000000, //F
                           0b10111100, 0b10000000, //G
                           0b01101110, 0b10000000, //H
                           0b10010000, 0b00100100, //I
                           0b01111000, 0b00000000, //J
                           0b00001110, 0b00011000, //K
                           0b00011100, 0b00000000, //L
                           0b01101100, 0b01010000, //M
                           0b01101100, 0b00010010, //N
                           0b11111100, 0b00000000, //O
                           0b11001110, 0b10000000, //P
                           0b11111100, 0b00001000, //Q
                           0b11001110, 0b10001000, //R
                           0b10110000, 0b11000000, //S
                           0b10000000, 0b00100100, //T
                           0b01111100, 0b00000000, //U
                           0b00001100, 0b00010010, //V
                           0b01101100, 0b00001010, //W
                           0b00000000, 0b01011010, //X
                           0b00000000, 0b01010100, //Y
                           0b10010000, 0b00010010, //Z
                           0b10010010, 0b01000010, //[
                           0b00000000, 0b01001000, //backslash. this @#!$er got escaped and it took me like a million years to find
                           0b10010000, 0b10011000, //]
                           0b01000000, 0b00010000, //^
                           0b00010000, 0b00000000, //_
                           0b00000000, 0b01000000, //`
                           0b00011010, 0b00000100, //a
                           0b00111110, 0b10000000, //b
                           0b00011010, 0b10000000, //c
                           0b01111010, 0b10000000, //d
                           0b00011010, 0b00000010, //e
                           0b00000000, 0b10010100, //f
                           0b11110000, 0b11000000, //g
                           0b00101110, 0b10000000, //h
                           0b00000000, 0b00000100, //i
                           0b00001000, 0b00100010, //j
                           0b00000000, 0b00111100, //k
                           0b00010000, 0b00100100, //l
                           0b00101010, 0b10000100, //m
                           0b00001010, 0b00000100, //n
                           0b00111010, 0b10000000, //o
                           0b10001110, 0b00010000, //p
                           0b11100000, 0b11000000, //q
                           0b00001010, 0b00000000, //r
                           0b00010000, 0b10001000, //s
                           0b00011110, 0b00000000, //t
                           0b00111000, 0b00000000, //u
                           0b00001000, 0b00000010, //v
                           0b00101000, 0b00001010, //w
                           0b00000010, 0b10001010, //x
                           0b01110000, 0b10100000, //y
                           0b00010010, 0b00000010, //z
                           0b00000000, 0b10011000, //{
                           0b00000000, 0b00100100, //|
                           0b00000010, 0b01000010, //}
                           0b10000000, 0b00000000 //~
                           };


//tracks the current frame
int currentFrame = 0;

//tracks how long it's been since each button was pressed
//element 4 is used for detecting rapid nibble toggles to display high score
int debounceArray[4] = {debounceTime,debounceTime,debounceTime,debounceTime};

//tracks when the current game was started to show the user their time
unsigned long startTime;

void setup(){

  randomSeed(analogRead(A0)+analogRead(A1)+analogRead(A2)+analogRead(A3));

  //penny pinch about power consumption. compared to the LEDs, the difference is negligible
  power_spi_disable(); // SPI
  power_usart0_disable();// Serial (USART)
  power_timer2_disable();// Timer 2
  power_twi_disable(); // TWI (I2C)
  
  //set up timers - for updating display
  timerSetup();
  
  //initialize frameCube to be empty
  clearCube();


//  testCode();
  
  //set game mode
  delay(100); //button pins seem to take a moment to get pulled low, so wait a sec
  gameMode = getButtons(true);  //set game mode, ignore nibbleswitch
  wimpMode = bitRead(gameMode,0) && !(gameMode == 15);//check if wimp mode is activated

  //set up the guess tables, display the options selected
  setupGuesses(true);
  
  //if all the buttons are held down, we can't get a random seed, so spin wheels until they release some
  while(getButtons(true) == 15){}
  
  PORTC = 0b00110000; //turn off pullup resistors
  //any buttons not pressed should be floating, sum them to gobble up all their delicious randomness  
  randomSeed(analogRead(A0)+analogRead(A1)+analogRead(A2)+analogRead(A3));
  PORTC = 0b00111111; //turn 'em on again

  //clear the screen
  clearCube();

  //start the player timer
  startTime = millis();

  //play the game!
  annihilateMode();
}


//sets up the guesses for the game
//if describe is true, also display the gameMode selected
void setupGuesses(bool describe){

  //populate the guess arrays
  fillGuesses();
  
  if(gameMode != 15){ //just display whether wimp mode is on
      if(describe && wimpMode){
        displayString("WIMP");
        delay(500);
       }
    
  clearCube();

    if(bitRead(gameMode,3)){  //ascii included
      if(describe) displayChar(0,'a');  //display that it is
      asciiGuessIndex = 127;  //initialize the guessIndex for full ascii
    }
    if(bitRead(gameMode,2)){  //dec included
      if(describe) displayChar(1,'d');  //display that it is
      if(wimpMode) decGuessIndex = 15;  //only guess up to 15 if wimpy
      else decGuessIndex = 255; //otherwise guess the whole thing
    }
    if(bitRead(gameMode,1)){  //hex included
      if(describe) displayChar(2,'x');  //display that it is
      if(wimpMode) hexGuessIndex = 15;  //only guess up to F if wimpy
      else hexGuessIndex = 255; //otherwise guess the whole thing
    }
    if(gameMode < 2){ //all included
      if(describe) displayString("full");
      if(wimpMode){ //set for wimp mode
        decGuessIndex = 15;
        hexGuessIndex = 15;
      }else{  //set for normal guess
        decGuessIndex = 255;
        hexGuessIndex = 255;
      }
      //ascii wimp mode is set by the population of the guess array, not
      //the starting guessIndex, so unless it's exprt, make it 127
      asciiGuessIndex = 127;
    }
  }else{  //expert mode set! display and set ascii for NP and symbols only
    if(describe){
      displayString("XPRT");
      delay(500);
      displayString("mode");
    }
    asciiGuessIndex = 66; //symbols only
  }
  if(describe) delay(500);  

//if any buttons are held, display high score for this gameMode
  while(getButtons(true) > 0){
    unsigned long highScore;
    EEPROM.get(32*gameMode, highScore);
    if(!displayTime(highScore)) break;  //if high score is too low to display skip it silently
  }
}


//fill the guess arrays with all options, in order, except for ascii
//ascii has all the alphanumerics moved to the back to allow for wimp and expert modes
//to be easily set by only using a portion of the array
void fillGuesses(){

  //fill dec and hex
  for(int i=0; i<256; i++){
    decGuesses[i] = i;
    hexGuesses[i] = i;
  }

  //fill unchanged portion of ascii
  for(int l=0; l<48; l++){
    asciiGuesses[l] = l;
  }

  //this is a stupid way to do it but my brain hurts, don't @ me

  //shift ascii symbols back
  for(int j=0; j<7; j++) asciiGuesses[j+48] = j+58;  //shifts back : to @
  for(int j=0; j<7; j++) asciiGuesses[j+55] = j+91;  //shifts back [ to `
  for(int j=0; j<7; j++) asciiGuesses[j+61] = j+123;  //shifts back { to DEL

  //shift ascii alphanumerics forward
  for(int k=0; k<26; k++){
    asciiGuesses[k+66] = k+65;  //shifts A-Z
    asciiGuesses[k+92] = k+97;  //shifts a-z
  }

  for(int m=0; m<10;m++){
    asciiGuesses[m+118] = m+48; //shifts 0-9
  }
}


//gets a unique guess from the respective list using Knuth-Fisher-Yates
//KFY operates by starting the index at the end, swapping the index for a
//random position, and then returning that position and decrementing the index
//if complete, return -1
//base 10 = dec
//base 16 = hex
//base 128 = ascii
byte getGuess(int base){
  byte newGuess;  //the new guess to get swapped when we return the current
  int newGuessIndex;  //the initial index of that guess
  switch(base){
    case 128:  //ascii
      //if wimp mode
      if(bitRead(gameMode,0) && gameMode != 15){
        //quit if ascii is already done
        if(asciiGuessIndex == 66) return -1;
        //start at the alphas
        newGuessIndex = random(66,asciiGuessIndex);
      }else{
        //quit if ascii is already done
        if(asciiGuessIndex == 0) return -1;
        //otherwise start at the symbols
        newGuessIndex = random(0,asciiGuessIndex);
      }

      //take out the new guess
      newGuess = asciiGuesses[newGuessIndex];

      //replace it with the one at [base]GuessIndex
      asciiGuesses[newGuessIndex] = asciiGuesses[asciiGuessIndex];

      //decrement index for next guess
      asciiGuessIndex--;

      //if nibbleswitch is high and in wimp mode and alternate wimp mode is enabled, shift all bits up by 4
      if(digitalRead(nibbleSwitchPin) && bitRead(gameMode,0) && gameMode != 15 && aWimp) newGuess = newGuess << 4;
      
      return newGuess;
      break;
    case 10:
      //quit if dec is already done
      if(decGuessIndex == 0) return -1;
      newGuessIndex = random(0,decGuessIndex); //new guess index

      //take out the new guess
      newGuess = decGuesses[newGuessIndex];

      //replace it with the one at [base]GuessIndex
      decGuesses[newGuessIndex] = decGuesses[decGuessIndex];

      decGuessIndex--;

      //if nibbleswitch is high and in wimp mode and alternate wimp mode is enabled, shift all bits up by 4
      if(digitalRead(nibbleSwitchPin) && bitRead(gameMode,0) && gameMode != 15 && aWimp) newGuess = newGuess << 4;
            
      return newGuess;
      break;
    case 16:
      //quit if hex is already done
      if(hexGuessIndex == 0) return -1;
      newGuessIndex = random(0,hexGuessIndex); //new guess index

      //take out the new guess
      newGuess = hexGuesses[newGuessIndex];

      //replace it with the one at [base]GuessIndex
      hexGuesses[newGuessIndex] = hexGuesses[hexGuessIndex];

      hexGuessIndex--;

      //if nibbleswitch is high and in wimp mode and alternate wimp mode is enabled, shift all bits up by 4
      if(digitalRead(nibbleSwitchPin) && bitRead(gameMode,0) && gameMode != 15 && aWimp) newGuess = newGuess << 4;
      
      return newGuess;
      break;
  }
}

//main game
//goal is stored in correctGuess
//buttons being pressed are stored in currentGuess
void annihilateMode(){
  //stores what base the current guess is in
  //0=ascii
  //1=dec
  //2=hex
  int base = 0;
  
  //place to store output of itoa
  char itoa_result[4];

  while(true){

  //check for win state
  if((hexGuessIndex+decGuessIndex+asciiGuessIndex == 0) || (wimpMode && asciiGuessIndex == 66)){
    displayString("YOU");
    delay(500);
    displayString("WON!");
    delay(500);

    //check for high score
    unsigned long newTime = (millis()-startTime);  //time this game took
    unsigned long currentRecord;
    int EEaddress = 32*gameMode;  //each high score is stored at a place unique to the gamemode
    EEPROM.get(EEaddress,currentRecord);
    if(newTime < currentRecord){
      EEPROM.put(EEaddress, newTime);  //store new high score
          displayString("NEW");
          delay(500);
          displayString("REC!");
          delay(500);
    }
    displayTime(newTime);  //display time in seconds
    delay(3000);  //bask in the glory
    startTime = millis(); //reset timer

    //reset guess stuff
    setupGuesses(false);
  }
  
    //pick random numbers until we get a valid one
    //if we're in expert mode don't bother, it's ascii
    while(gameMode != 15){
      base = random(0,3);
      //if full and there are ones left to guess, whatever is fine
      if(gameMode < 2 && hexGuessIndex+decGuessIndex+asciiGuessIndex > 0) break;
      //if hex and there are hex guesses left
      else if(base == 2 && hexGuessIndex > 0) break;
      //if dec and there are dec guesses left
      else if(base == 1 && decGuessIndex > 0) break;
      //if ascii and there are ascii guesses left
      else if(base == 0 && asciiGuessIndex > 0) break;
    }

    //display the char to guess
    //print a leading char to indicate the base requested,
    //then convert the guess to the appropriate string to display it
    if(base == 1){  //decimal
      correctGuess = getGuess(10);
      displayChar(0,'d');
      itoa(correctGuess,itoa_result,10);
    }else if(base == 2){  //hex
      correctGuess = getGuess(16);
      displayChar(0,'x');
      itoa(correctGuess,itoa_result,16);
      for (auto & c: itoa_result) c = toupper(c); //hex looks prettier in CAPS
    }else{  //ascii
      correctGuess = getGuess(128);
      displayChar(0,'a');
      //no conversion neccesary, just plop the guess in
      itoa_result[0] = correctGuess;
  
      //manually terminate the string early
      itoa_result[1] = 0;
    }

    //TODO this shouldn't be needed, just plop the string into displayString
    //itoa has no leading spaces, and the length depends on the number of 
    //digits it outputs, so we need to manually right-justify it within the display
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

    //hackedy hack
    if(base == 0) displayChar(0,'a');

    while(true){  // wait for the player to get the answer right
      setButtons(currentGuess); //show the current guess on the buttons

      //input 255 to give up
      if(currentGuess == 255){
        if(correctGuess != 255){  //if it IS 255, it's just a correct guess
          //show the player what the answer is until they press a button
          //TODO: this is broken, it doesn't wait rn
          while(!getButtons()){
            setButtons(correctGuess);
          }
          displayString("Good");
          delay(500);
          displayString("Try!");
          delay(500);
          //test if guess was the last one in the set
          switch(base){
            case 0: //ascii
              //if (wimp mode and 66) or not and 0, ascii done
              if((bitRead(gameMode,0) && gameMode != 15 && asciiGuessIndex == 66) || asciiGuessIndex == 0){
                displayString("asci");
                delay(500);
                displayString("DONE");
                delay(500);
              }
              break;
            case 1: //dec
              if(decGuessIndex == 0){
                displayString("dec");
                delay(500);
                displayString("DONE");
                delay(500);
              }
              break;
            case 2: //hex
              if(hexGuessIndex == 0){
                displayString("hex");
                delay(500);
                displayString("DONE");
                delay(500);
              }
              break;
          }
          clearCube();
          currentGuess = 0;
          break;
        }
      }
    
      //is the guess correct?
      if(currentGuess == correctGuess){

        //if it was 0
        if(correctGuess == 0){
          delay(3000);  //pause for comedic effect
          displayString("JK ");
          delay(500);
        }else if(correctGuess == 69){ //nice
          displayString("nice");
          delay(500);
        }
        displayString("Good");
        delay(500);
        displayString("Job!");
        delay(500);
        
        //test if guess was the last one in the set
        //TODO: why do we test twice?
        switch(base){
          case 0: //ascii
            //if beginner mode and 66 or not and 0
            if((bitRead(gameMode,0) && gameMode != 15 && asciiGuessIndex == 66) || asciiGuessIndex == 0){
              displayString("asci");
              delay(500);
              displayString("DONE");
              delay(500);
            }
            break;
          case 1: //dec
            if(decGuessIndex == 0){
              displayString("dec");
              delay(500);
              displayString("DONE");
              delay(500);
            }
            break;
          case 2: //hex
            if(hexGuessIndex == 0){
              displayString("hex");
              delay(500);
              displayString("DONE");
              delay(500);
            }
            break;
        }
        clearCube();
        currentGuess = 0;
        break;
      }
    }
    //reseed random for extra randomness
    //the result of this is determined purely by how long it took the player to
    //guess correctly, so it's VERY random
    randomSeed(micros());
  }
}

//TODO: figure this shit out again so i can describe it lol
//sets up timers used for updating the display and button LEDs
void timerSetup(){
  cli();  //turn off timers while we set 'em up
  TCCR1A = 0;
  TCCR1B = 0b00000100;
  TIMSK1 = 0b00000010;
  OCR1A = 64;
  TCNT1 = 0;
  sei();  //turn timers back on
}

//displays the given time on the display
//tim is a number of milliseconds
//return 0 if the number is 2 big
bool displayTime(unsigned long tim){
  if(tim > 5999000) return false; //number too big to display, don't try

  int minutes = (tim/1000)/60;  //calculate minutes from tim
  int seconds = (tim/1000)%60;  //calculate seconds from tim

  //convert to ascii and display
  displayChar(0,(minutes/10)+48); 
  displayChar(1,(minutes%10)+48);
  displayChar(4,':');
  displayChar(2,(seconds/10)+48);
  displayChar(3,(seconds%10)+48);
  return true;
}

//update display and buttons asynchronously
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

  //debounce buttons
  updateButtons();
  
}

//sets the frame cube to an empty state, I.E. a totally blank display
void clearCube(){
  //all of the frames have mostly the same clear state, so set that
  //also set the button LEDs' anode pins to always be high-Z
  //also set the button's and switch's input pins to pullup
  for(byte frame=0; frame < 10; frame++){
    frameCube[frame][0] = 0b10111001; //PORTB
    frameCube[frame][1] = 0b00111111; //PORTC
    frameCube[frame][2] = 0b01111101; //PORTD
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
}

//returns a byte reflecting the current state of the buttons
byte getButtons(bool ignoreSwitch){
  byte buttonValue = 0;
  //or combine each pin into the buttonvalue
  if(!digitalRead(button_input_pin_bit0)) buttonValue |= (1<<0);
  if(!digitalRead(button_input_pin_bit1)) buttonValue |= (1<<1);
  if(!digitalRead(button_input_pin_bit2)) buttonValue |= (1<<2);
  if(!digitalRead(button_input_pin_bit3)) buttonValue |= (1<<3);
  //if nibbleswitch is high, shift all bits up by 4
  if(digitalRead(nibbleSwitchPin) & !ignoreSwitch) buttonValue = buttonValue << 4;
  return buttonValue;
}

//sets currentGuess to reflect the user's input on the buttons
//waits the # of millis specified in debounceArray before being able to update a button
void updateButtons(){
  byte buttonValue = 0;
  if(!digitalRead(button_input_pin_bit0) && debounceArray[0] == debounceTime){
    buttonValue |= (1<<0);
    debounceArray[0] = 0;
  }
  if(!digitalRead(button_input_pin_bit1) && debounceArray[1] == debounceTime){
    buttonValue |= (1<<1);
    debounceArray[1] = 0;
  }
  if(!digitalRead(button_input_pin_bit2) && debounceArray[2] == debounceTime){
    buttonValue |= (1<<2);
    debounceArray[2] = 0;
  }
  if(!digitalRead(button_input_pin_bit3) && debounceArray[3] == debounceTime){
    buttonValue |= (1<<3);
    debounceArray[3] = 0;
  }

  if(digitalRead(nibbleSwitchPin)) buttonValue = buttonValue << 4;
  currentGuess = currentGuess ^= buttonValue;
  for(int b=0; b<4; b++){
    if(debounceArray[b] < debounceTime) debounceArray[b]++;
  }
}

//sets the button lights to the value in the nibble of buttonValue that the
//switch is set to.
void setButtons(byte currentGuesss, bool ignoreSwitch){
  if(digitalRead(nibbleSwitchPin) & !ignoreSwitch) currentGuesss = currentGuesss >> 4;
  //                to bit v                        v from bit  
  bitWrite(frameCube[9][2],0,!bitRead(currentGuesss, 3));//b1, set D0 low
  bitWrite(frameCube[9][1],4,!bitRead(currentGuesss, 2));//b2, set C4 low
  bitWrite(frameCube[9][1],5,!bitRead(currentGuesss, 1));//b3, set C5 low
  bitWrite(frameCube[9][0],3,!bitRead(currentGuesss, 0));//b4, set B3 low 
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
//if string contains less than 4 characters, right-justify it
void displayString(const char* dString){
  clearCube();
  for(int i=0; i<strlen(dString); i++){
    displayChar( i + (4-strlen(dString)),dString[i]);
  }
}

//display the specified char at the specified digit on the display
//pretty makes space act as a printed blank char and not "SP"
void displayChar(byte digit, char dChar, bool pretty){
  switch(dChar){
    //if asked to display a non-printing character, print its 3-letter abbreviation
    case 0:
      displayString("NUL");
      break;
    case 1:
      displayString("SOH");
      break;
    case 2:
      displayString("STX");
      break;
    case 3:
      displayString("ETX");
      break;
    case 4:
      displayString("EOT");
      break;
    case 5:
      displayString("ENQ");
      break;
    case 6:
      displayString("ACK");
      break;
    case 7:
      displayString("BEL");
      break;
    case 8:
      displayString("BS");
      break;
    case 9:
      displayString("TAB");
      break;
    case 10:
      displayString("LF");
      break;
    case 11:
      displayString("VT");
      break;
    case 12:
      displayString("FF");
      break;
    case 13:
      displayString("CR");
      break;
    case 14:
      displayString("SO");
      break;
    case 15:
      displayString("SI");
      break;
    case 16:
      displayString("DLE");
      break;
    case 17:
      displayString("DC1");
      break;
    case 18:
      displayString("DC2");
      break;
    case 19:
      displayString("DC3");
      break;
    case 20:
      displayString("DC4");
      break;
    case 21:
      displayString("NAK");
      break;
    case 22:
      displayString("SYN");
      break;
    case 23:
      displayString("ETB");
      break;
    case 24:
      displayString("CAN");
      break;
    case 25:
      displayString("EM");
      break;
    case 26:
      displayString("SUB");
      break;
    case 27:
      displayString("ESC");
      break;
    case 28:
      displayString("FS");
      break;
    case 29:
      displayString("GS");
      break;
    case 30:
      displayString("RS");
      break;
    case 31:
      displayString("US");
      break;
    case ' ': //sometimes this is ' ', sometimes it's 'SP'
      if(pretty){
        //                   ABCDEFGCn   GHJKLMNDp
        setSegments(digit, 0b00000000, 0b00000000);
      }else{
        displayString("SP");
      }
      break;
    case 127:
      displayString("DEL");
      break;
    default:  //printing char, pull it from the lookup table
      setSegments(digit,pgm_read_byte_near(asciiSegs + (dChar*2-66)),pgm_read_byte_near(asciiSegs + (dChar*2-65)));
  }
}
