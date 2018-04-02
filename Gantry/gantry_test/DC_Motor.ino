/******** Motor Specific Variables ********/
//
//Speeds
extern volatile int speed_X = zeroSpeed;
extern volatile int speed_Y = zeroSpeed;
int Right = 20;
int Left = 180;
int Forward = 180;
int Backward = 20;

//State
int X_count = 0;
int Y_count = 0;

//Positioning Variables
int X_center = 0;
int Y_center = 0;
extern volatile int X_goal = 0;
extern volatile int Y_goal = 0;

void motor_setup() {

  //Switch Init
  pinMode(limSwitch1, INPUT);
  pinMode(limSwitch2, INPUT);
  pinMode(limSwitch3, INPUT);
  pinMode(limSwitch4, INPUT);
  pinMode(limSwitch5, INPUT);
  pinMode(limSwitch6, INPUT);

  //Motor Init
  pinMode(X_Motor, OUTPUT);
  analogWrite(X_Motor, speed_X);
  pinMode(Y_Motor, OUTPUT);
  analogWrite(Y_Motor, speed_Y);

  //Stepper Motor Init
  stepper_rot.setMaxSpeed(1000);
  stepper_rot.setAcceleration(2000);
  stepper_rot.setSpeed(400);
  stepper_rot.setCurrentPosition(0.0); 
  pinMode(Enable, OUTPUT);

  //Encoder Init
  //X
  pinMode(X_channelAPin, INPUT);
  pinMode(X_channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(X_channelBPin), X_encoderCount, CHANGE);

  //Y
  pinMode(Y_channelAPin, INPUT);
  pinMode(Y_channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(Y_channelBPin), Y_encoderCount, CHANGE);

}

/*****************************************************************/
/* Motor Speed Setting
 *  Set speeds and relay levels for X and Y motors
 */
 /*****************************************************************/
void set_speed(int X_speed, int Y_speed) {
  speed_X = X_speed;
  if (speed_X == zeroSpeed) {
    digitalWrite(relay_X, LOW);
  }
  else {digitalWrite(relay_X, HIGH);}
  speed_Y = Y_speed;
  if (speed_Y == zeroSpeed) {
    digitalWrite(relay_Y, LOW);
  } 
  else {digitalWrite(relay_Y, HIGH);}
  analogWrite(X_Motor, speed_X);
  analogWrite(Y_Motor, speed_Y);
}

/*****************************************************************/
/* ENCODER ISRs
 *  Count encoder steps for X and Y motors
 */
 /*****************************************************************/
void X_encoderCount(){
  X_channelAVal = digitalRead(X_channelAPin);
  X_channelBVal = digitalRead(X_channelBPin);
  
  Serial.print("X:  ");  Serial.println(X_encoderTicks);
  if (X_channelAVal == X_channelBVal) {
    X_encoderTicks++;
  }
  else {
    X_encoderTicks--;
  }
}

void Y_encoderCount(){
  Y_channelAVal = digitalRead(Y_channelAPin);
  Y_channelBVal = digitalRead(Y_channelBPin);
  
  Serial.print("Y:  ");Serial.println(Y_encoderTicks);
  if (Y_channelAVal == Y_channelBVal) {
    Y_encoderTicks++;
  }
  else {
    Y_encoderTicks--;
  }
}
