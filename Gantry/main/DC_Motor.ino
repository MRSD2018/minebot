/******** Motor Specific Variables ********/

//Speeds
double Right = 20;
double Left = 180;
double Forward = 200;
double Backward = 0;

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
//  pinMode(limSwitch6, INPUT);

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
/* INITIALIZATION ROUTINE
 *  Hardcoded: Initialize Y, then X axis, the rotate  
 */
 /*****************************************************************/
void initialize() {
  if (Debug) {
    Serial.print("Y Speed:  "); Serial.print(speed_Y);
    Serial.print("   Y Encoder:  "); Serial.print(Y_encoderTicks);
    Serial.print("   Y_Max:  "); Serial.println(Y_max);
    Serial.print("X Speed:  "); Serial.print(speed_X);
    Serial.print("   X Encoder:  "); Serial.print(X_encoderTicks);
    Serial.print("   X_Max:  "); Serial.println(X_max);
    Serial.print("   state:  "); Serial.println(init_state);
  }

  //Init_State 0: go to Y_min limit switch and encoderTicks to 0
  if (init_state == 0) {
    Y_encoderTicks = 0;
    set_speed(zeroSpeed, Right);
  }

  //Init_State 1: go to Y_max limit switch and set Y_max to encoderTicks
  else if (init_state == 1) {
    Y_max = abs(Y_encoderTicks);
    set_speed(zeroSpeed, Left);
  }

  //Init_State 2: go to Y_max/2
  else if (init_state == 2) {
    Y_center = (Y_max)/2;
    set_speed(zeroSpeed, Right);
    if (abs(Y_encoderTicks) < Y_center) {
      init_state =3;
    } 
  }

  //Init_State 3: go to X_min and set encoderTicks to 0
  else if (init_state ==3) {
    X_encoderTicks = 0;
    set_speed(Backward, zeroSpeed);
  }

  //Init_State 4: go to X_max and set encoderTicks to X_max
  else if (init_state == 4){
    X_max = abs(X_encoderTicks);
    set_speed(Forward, zeroSpeed);
  }

  //Init_State 5: go to X_mid
  else if (init_state == 5) {
    X_center = (X_max)/2;
    set_speed(Backward, zeroSpeed);
    if (abs(X_encoderTicks) < X_center) {
      init_state = 6;
    }
  }

  //Init_State 6: rotate rotary plate forward and back to 800 ticks
  else if (init_state == 6){
//    set_speed(zeroSpeed, zeroSpeed);
//    stepper_rot.runToNewPosition(800);
//    stepper_rot.runToNewPosition(-800);
    init_state = 7;
    Initialization_Flag = true;
  }

  //Else, set everything to off
  else {
    set_speed(zeroSpeed,zeroSpeed);
  }
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
  
//  Serial.print(channelBVal); Serial.print(channelAVal); Serial.println(encoderTicks);
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
  
//  Serial.print(channelBVal); Serial.print(channelAVal); Serial.println(encoderTicks);
  if (Y_channelAVal == Y_channelBVal) {
    Y_encoderTicks++;
  }
  else {
    Y_encoderTicks--;
  }
}
