void setup() {
  //anodes
  /*
  pinMode(1, OUTPUT); //pin 16->3->1
  pinMode(0, OUTPUT); //pin 1->2->0
  pinMode(9, OUTPUT); //pin 15->15->9
  pinMode(A4, OUTPUT); //pin 2->27->A4
  pinMode(A5, OUTPUT); //pin 7->28->A5
  //pinMode(1, OUTPUT); //pin 11->9->OSC1
  pinMode(11, OUTPUT); //pin 8->17->11
  pinMode(7, OUTPUT); //pin 9->13->7

  //cathodes
  pinMode(2, OUTPUT); //pin 14->4->2
  pinMode(3, OUTPUT); //pin 13->5->3
  pinMode(4, OUTPUT); //pin 4->6->4
  pinMode(5, OUTPUT); //pin 3->11->5
  pinMode(12, OUTPUT); //pin 6->18->12
//  pinMode(OSC2, OUTPUT); //pin 12->10->OSC2
  pinMode(13, OUTPUT); //pin 5->19->13
*/
    DDRB = 0b11111111;
    DDRC = 0b11110000;
    DDRD = 0b11111111;

    pinMode(A0,INPUT_PULLUP);
    pinMode(A1,INPUT_PULLUP);
    pinMode(A2,INPUT_PULLUP);
    pinMode(A3,INPUT_PULLUP);
    
}


int buttonPress = 0;

void loop() {
  
    pinMode(A0,INPUT_PULLUP);
    pinMode(A1,INPUT_PULLUP);
    pinMode(A2,INPUT_PULLUP);
    pinMode(A3,INPUT_PULLUP);

    //int buttonPress = 
    readButtons();

    switch (buttonPress){
      case 1:
        fuck();
        break;
      case 2:
        shit();
        break;
      case 3:
        ass();
        break;
      case 4:
        tits();
        break;
      default:
        juno();
        break;
    }
}

void readButtons(){
  if(!digitalRead(A0)){
    if(!digitalRead(A3)){
      buttonPress = 0;
      delay(50);
    }else{
    buttonPress = 1;
    }
  }
  else if(!digitalRead(A1)) buttonPress = 2;
  else if(!digitalRead(A2)) buttonPress = 3;
  else if(!digitalRead(A3)) buttonPress = 4;
//  return 0;
}

void empty(){
  
  //
  //high 
  //low 
  //        76543210
  PORTB = 0b10110000;
  PORTC = 0b00000000;
  PORTD = 0b00111100;
  //        76543210

  delay(1);

}

void tits(){
  
  
  //T (just the top)
  //high 1A
  //low A
  //        76543210
  PORTB = 0b10110000;
  PORTC = 0b00000000;
  PORTD = 0b00111010;
  //        76543210

  delay(1);
  
  //T (bottom)
  //high 1B
  //low J,M
  //        76543210
  PORTB = 0b00110000;
  PORTC = 0b00000000;
  PORTD = 0b00101101;
  //        76543210

  delay(1);
  
  //i
  //high 2B
  //low M
  //        76543210
  PORTB = 0b00110000;
  PORTC = 0b00010000;
  PORTD = 0b00111100;
  //        76543210

  delay(1);
  
  //t (most)
  //high 3B
  //low J,G2,M
  //        76543210
  PORTB = 0b00110100;
  PORTC = 0b00000000;
  PORTD = 0b00101000;
  //        76543210

  delay(1);
  
  //t (left part)
  //high 4A
  //low G1
  //        76543210
  PORTB = 0b11010000;
  PORTC = 0b00000000;
  PORTD = 0b00111100;
  //        76543210

  delay(1);
  
  //s
  //high 1A
  //low D
  //        76543210
  PORTB = 0b10111000;
  PORTC = 0b00000000;
  PORTD = 0b00011100;
  //        76543210

  delay(1);

  //s (rest of it)
  //high 1B
  //low G2,L
  //        76543210
  PORTB = 0b10100000;
  PORTC = 0b00000000;
  PORTD = 0b10111000;
  //        76543210

  delay(1);
}

void ass(){
  
  
  //A (most)
  //high 1A 
  //low A,B,C,E,F,G1
  //        76543210
  PORTB = 0b00000000;
  PORTC = 0b00000000;
  PORTD = 0b00100010;
  //        76543210

  delay(1);

//upper right of A
//high 1B 
//low G2
//          76543210
  PORTB = 0b10110000;
  PORTC = 0b00000000;
  PORTD = 0b00111001;
//          76543210

  delay(1);
  
  //s
  //high 2A,3A
  //low D
  //        76543210
  PORTB = 0b11110010;
  PORTC = 0b00000000;
  PORTD = 0b00011100;
  //        76543210

  delay(1);

  //s (rest of it)
  //high 2B,3B
  //low G2,L
  //        76543210
  PORTB = 0b10100100;
  PORTC = 0b00010000;
  PORTD = 0b00111000;
  //        76543210

  delay(1);

}

void shit(){
  
  //S (most)
  //high 1A
  //low A,F,C,D,G1
  //        76543210
  PORTB = 0b00010000;
  PORTC = 0b00000000;
  PORTD = 0b00001010;
  //        76543210

  delay(1);
  
  //S (rest)
  //high 1B
  //low G2
  //        76543210
  PORTB = 0b10110000;
  PORTC = 0b00000000;
  PORTD = 0b00111001;
  //        76543210

  delay(1);
  
  //h (most)
  //high 2A
  //low C,E,F,G1
  //        76543210
  PORTB = 0b00000010;
  PORTC = 0b00000000;
  PORTD = 0b00101100;
  //        76543210

  delay(1);

//upper right of h
//high 3B 
//low G2
//          76543210
  PORTB = 0b10110000;
  PORTC = 0b00010000;
  PORTD = 0b00111000;
//          76543210

  delay(1);
  
  //i
  //high 3B
  //low M
  //        76543210
  PORTB = 0b00110100;
  PORTC = 0b00000000;
  PORTD = 0b00111100;
  //        76543210

  delay(1);
  
  //t (most)
  //high 4B
  //low J,G2,M
  //        76543210
  PORTB = 0b00110000;
  PORTC = 0b00000000;
  PORTD = 0b10101000;
  //        76543210

  delay(1);
  
  //t (left part)
  //high 4A
  //low G1
  //        76543210
  PORTB = 0b10011000;
  PORTC = 0b00000000;
  PORTD = 0b00111100;
  //        76543210

  delay(1);

}

void fuck(){
  
  //F
  //high 1A
  //low A,E,F,G1
  //        76543210
  PORTB = 0b00000000;
  PORTC = 0b00000000;
  PORTD = 0b00111010;
  //        76543210

  delay(1);

  //u
  //high 2A
  //low C,D,E
  PORTB = 0b10100010;
  PORTC = 0b00000000;
  PORTD = 0b00001100;  

  delay(1);

//c (mostly)
//high 3A
//low D,E,G1
//          76543210
  PORTB = 0b11000000;
  PORTC = 0b00000000;
  PORTD = 0b00011100;  
//          76543210

  delay(1);

//upper right of c
//high 3B 
//low G2
//          76543210
  PORTB = 0b10110100;
  PORTC = 0b00000000;
  PORTD = 0b00111000;
//          76543210

  delay(1);

//k (slanties)
//high 4B
//low K,L
//          76543210
  PORTB = 0b10100000;
  PORTC = 0b00000000;
  PORTD = 0b10011100;
//          76543210

  delay(1);
  
  //k (rest)
  //high 4A
  //low F,E,G1
  //        76543210
  PORTB = 0b00001000;
  PORTC = 0b00000000;
  PORTD = 0b00111100;
  //        76543210

  delay(1);

}


void juno(){
  //J 
  //high  1A,A,F,G1
  //low   2A,2B,3A,3B,4A,4B,DN,B,C,D,E,1B
  PORTB = 0b10100000;
  PORTC = 0b00000000;
  PORTD = 0b00000110;

  delay(1);

  //u, o
  //high  B,2A,4A
  //low   1A
  PORTD = 0b00001100;
  PORTB = 0b10101010;

  delay(1);

  //n (except for the right half of the top)
  //high  3A,4A
  //low   2A,D,G1
  PORTB = 0b11001000;
  PORTD = 0b00101100;

  delay(1);

  //right half of the top of the n and o
  //high  3B,B,C,D,E,F,G1,4B
  //low 3A,A
  PORTB = 0b10110100;
  PORTC = 0b00000000;
  PORTD = 0b10111000;
  
  delay(1);

}
