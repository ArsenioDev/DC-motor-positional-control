 /* Full PID control plant for DC motor positional control
  *  3/21/17
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Arsenio Dev wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer(or a burger) in return.   
 * ----------------------------------------------------------------------------
  *  tminusarduino.blogspot.com
  *  @ascii211
  *5 IO variant, PWM half-brake mode on TB6612FNG
  * Connect PWMA and Standby to 5V
  * Connect INA1/2 to PWM
 */
 #include <PID_v1.h>
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
   TCCR0B = (TCCR0B & 0b11111000) | 0x02; //Fast PWM to minimize noise
  Plant.SetMode(AUTOMATIC);
  Plant.SetSampleTime(1); //Makes the system go fast
  Plant.SetOutputLimits(-255, 255); //Limiter on PID to not exceed PWM maximum
 } 

 void loop() { 
  int Target = map(analogRead(Set),0, 1024, 0, Trgt); //map analog read value to range of counts 0-300
  setpoint = Target; //target pos is PID setpoint
  input = encoder0Pos; //Input is current encoder position
  Plant.Compute(); //Calculate PID 
  pwmOut(output); //Call the drive function using the int of PID output

  Serial.print(Target);// Prints target count
  Serial.print(" ");
  Serial.print(encoder0Pos); //Prints actual count
  Serial.print(" ");
  Serial.print(setpoint - input); //Error term
  Serial.print(" ");
  Serial.println(output); //Output for reference
 }

void pwmOut(int out) {                                // to H-Bridge motor driver
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
