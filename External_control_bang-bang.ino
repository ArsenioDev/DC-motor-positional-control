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
 int Trgt = 300; //Setpoint range
int STBY = 10; //standby
int PWMA = 6; //Speed control 
int AIN1 = 8; //Direction
int AIN2 = 9; //Direction


 #define LEFT 1 //Defining direction control for motor driver tb6612fng 
 #define RIGHT 0
 #define Set A0 //analog pin for setpoint potentiometer
 

 void setup() { //Straightforward setup, self explanatory
   pinMode (encoder0PinA,INPUT); //encoder phases need to be inputs to sense
   pinMode (encoder0PinB,INPUT);
   attachInterrupt(0, doEncoderA, CHANGE); //State change triggers interrupt to prevent loss of counts
   attachInterrupt(1, doEncoderB, CHANGE);
   Serial.begin (115200); //fast serial to speed loop
   pinMode(STBY, OUTPUT);
   pinMode(PWMA, OUTPUT);
   pinMode(AIN1, OUTPUT);
   pinMode(AIN2, OUTPUT);
   pinMode(Set, INPUT);
 } 

void move_dir(int dir) { //Add functionality for telling motor to move at full speed in direction
  move(1, 255, dir); //motor 1, full speed
  delay(1);
  stop();
}

 void loop() { 
  int Target = map(analogRead(Set),0, 1024, 0, Trgt); //map analog read value to range of counts 0-300
  if (encoder0Pos < Target) { //if not there yet, keep going
    move_dir(LEFT);
  } else if (encoder0Pos > Target) { //if overshoot, go back
    move_dir(RIGHT);
  }
  Serial.print(Target);// Prints target count
  Serial.print(" ");
  Serial.println(encoder0Pos); //Prints actual count
 }
 
void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
  
}
 
void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
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
