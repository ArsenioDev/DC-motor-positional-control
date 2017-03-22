 #include <PID_v1.h>
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
int Set = A0; //analog pin for setpoint potentiometer

double kp = 5 , ki = 1 , kd = 0.01;             // modify for optimal performance
double input = 0, output = 0, setpoint = 0;
PID Plant(&input, &output, &setpoint, kp, ki, kd, DIRECT);

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
  Plant.SetMode(AUTOMATIC);
  Plant.SetSampleTime(1);
  Plant.SetOutputLimits(-255, 255);
 } 

/*void move_dir(int dir) { //Add functionality for telling motor to move at full speed in direction
  move(1, dir); //motor 1, full speed
  delay(1);
  stop();
}*/

 void loop() { 
  int Target = map(analogRead(Set),0, 1024, 0, Trgt); //map analog read value to range of counts 0-300
  setpoint = Target;
  input = encoder0Pos;
  Plant.Compute();
  pwmOut(output);

  Serial.print(Target);// Prints target count
  Serial.print(" ");
  Serial.print(encoder0Pos); //Prints actual count
  Serial.print(" ");
  Serial.print(setpoint - input);
  Serial.print(" ");
  Serial.println(output);
 }

void pwmOut(int out) {                                // to H-Bridge board
  if (out > 0) {
    analogWrite(AIN1, out);                             // drive motor CW
    analogWrite(AIN2, 0);
  }
  else {
    analogWrite(AIN1, 0);
    analogWrite(AIN2, abs(out));                        // drive motor CCW
  }
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
