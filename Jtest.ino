void setup() {
  //anodes
  
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

    DDRB = 0b11111111;
    DDRD = 0b11111111;
}

void loop() {

  PORTB = 0b11100010;
  PORTD = 0b00000110;
  //J
//  digitalWrite(1, HIGH); //pin 16->3->1
//  digitalWrite(9, HIGH); //pin 15->15->9
  digitalWrite(11, HIGH); //pin 8->17->11
//  digitalWrite(11, HIGH); //pin 11->9->OSC1
  digitalWrite(0, LOW); //pin 1->2->0
  
  digitalWrite(3, LOW); //pin 13->5->3
  //digitalWrite(13, LOW); //pin 5->19->13
  digitalWrite(2, HIGH); //pin 14->4->2
  digitalWrite(4, LOW); //pin 4->6->4
  digitalWrite(5, LOW); //pin 3->11->5
  digitalWrite(1, LOW); //pin 6->18->12
    
}
