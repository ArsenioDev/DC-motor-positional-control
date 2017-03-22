 /* Basic bang-bang control plant for DC motor positional control
  *  12-7-2016
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Arsenio Dev wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   
 * ----------------------------------------------------------------------------
  *  tminusarduino.blogspot.com
  *  @ascii211
  */
  
 int val; 
 int encoder0PinA = 2; //interrupt pin 0 
 int encoder0PinB = 3; //interrrupt pin 1
 int encoder0Pos = 0; //initial encoder count on reset
 int encoder0PinALast = LOW; //assume leading signal is low
 int n = LOW;
 int Trgt = 512; //Setpoint range
int STBY = 10; //standby 
int AIN1 = 5; //Direction
int AIN2 = 6; //Direction
int Speed = 255;
 
 #define Set A0 //analog pin for setpoint potentiometer
 

 void setup() { //Straightforward setup, self explanatory
   pinMode (encoder0PinA,INPUT); //encoder phases need to be inputs to sense
   pinMode (encoder0PinB,INPUT);
   attachInterrupt(0, doEncoderA, CHANGE); //State change triggers interrupt to prevent loss of counts
   attachInterrupt(1, doEncoderB, CHANGE);
   Serial.begin (115200); //fast serial to speed loop  
   pinMode(AIN1, OUTPUT);
   pinMode(AIN2, OUTPUT);
   pinMode(Set, INPUT);
   TCCR0B = (TCCR0B & 0b11111000) | 0x02;
 } 

/*void move_dir(int dir) { //Add functionality for telling motor to move at full speed in direction
  move(1, dir); //motor 1, full speed
  delay(1);
  stop();
}*/

 void loop() { 
  int Target = map(analogRead(Set),0, 1024, 0, Trgt); //map analog read value to range of counts 0-300
  if (encoder0Pos < Target) { //if not there yet, keep going
    Left();
  } 
  else if (encoder0Pos > Target) { //if overshoot, go back
    Right();
  }
  else if (encoder0Pos = Target) {
    Brake();
  }
  Serial.print(Target);// Prints target count
  Serial.print(" ");
  Serial.println(encoder0Pos); //Prints actual count
 }
 
/*void move(int motor, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise
 int inPin1 = 0;
 int inPin2 = 0;

  if(direction == 1){
    inPin1 = 255;
    inPin2 = 0;
  }

  if(motor == 1){
    analogWrite(AIN1, inPin1);
    analogWrite(AIN2, inPin2);
  }
  
}*/
 
void Brake(){
digitalWrite(AIN1, 1);
digitalWrite(AIN2, 1);
}

void Left(){
  analogWrite(AIN1, Speed);
  analogWrite(AIN2, 0);
}

void Right(){
  analogWrite(AIN1, 0);
  analogWrite(AIN2, Speed);
}


void doEncoderA(){

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  //Serial.println (encoder0Pos, DEC);          
  // use for debugging - remember to comment out
}

void doEncoderB(){

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == LOW) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
}
