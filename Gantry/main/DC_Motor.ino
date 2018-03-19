/******** Motor Specific Variables ********/
//Motors
#define X_Motor 29
#define Y_Motor 30

//Encoder X
#define X_channelAPin 15
#define X_channelBPin 16
volatile bool X_channelAVal;
volatile bool X_channelBVal;
extern volatile int X_encoderTicks = 0;

//Encoder Y
#define Y_channelAPin 38
#define Y_channelBPin 39
volatile bool Y_channelAVal;
volatile bool Y_channelBVal;
extern volatile int Y_encoderTicks = 0;

//Speeds
extern volatile int speed_X = zeroSpeed;
extern volatile int speed_Y = zeroSpeed;
int Right = 40;
int Left = 160;
int Forward = 180;
int Backward = 20;

//Limit Switches
//Y axis switches
#define limSwitch1 34 //Y_max
#define limSwitch2 35 //Y_min
//X_max axis switches
#define limSwitch3 14
#define limSwitch4 7
//X_min axis switches
#define limSwitch5 8
//#define limSwitch6 22


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

  //Limit ISR
  attachInterrupt(digitalPinToInterrupt(limSwitch1),limitState_Y_max, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch2),limitState_Y_min, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch3),limitState_X_max, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch4),limitState_X_max, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch5),limitState_X_min, RISING);
//  attachInterrupt(digitalPinToInterrupt(limSwitch6),limitState_X_min, RISING);
}

/*****************************************************************/
/* INITIALIZATION ROUTINE
 *  Hardcoded: Initialize Y, then X axis, the rotate  
 */
 /*****************************************************************/
void initialize() {
  Serial.print("Y Speed:  "); Serial.print(speed_Y);
  Serial.print("   Y Encoder:  "); Serial.print(Y_encoderTicks);
  Serial.print("   Y_Max:  "); Serial.println(Y_max);
  Serial.print("X Speed:  "); Serial.print(speed_X);
  Serial.print("   X Encoder:  "); Serial.print(X_encoderTicks);
  Serial.print("   X_Max:  "); Serial.println(X_max);
  Serial.print("   state:  "); Serial.println(init_state);
    
  if (init_state == 0) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    Y_encoderTicks = 0;
    speed_Y = Right;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
  }
  
  else if (init_state == 1) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    Y_max = abs(Y_encoderTicks);
    speed_Y = Left;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
  }
  
  else if (init_state == 2) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    Y_center = (Y_max)/2;
    speed_Y = Right;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
    if (abs(Y_encoderTicks) < Y_center) {
      init_state =3;
    } 
  }

  else if (init_state ==3) {
    speed_Y = zeroSpeed;
    speed_X = Backward;
    analogWrite(Y_Motor, speed_Y);
    analogWrite(X_Motor, speed_X);
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    X_encoderTicks = 0;
  }
  
  else if (init_state == 4){
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    X_max = abs(X_encoderTicks);
    speed_X = Forward;
    analogWrite(X_Motor, speed_X);
    speed_Y = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
  }
  
  else if (init_state == 5) {
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    X_center = (X_max)/2;
    speed_X = Backward;
    analogWrite(X_Motor, speed_X);
    speed_Y = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    if (abs(X_encoderTicks) < X_center) {
      init_state = 6;
    }
  }

  else if (init_state == 6){
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, LOW);
    speed_Y = zeroSpeed;
    speed_X = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    analogWrite(X_Motor, speed_X);
    
    stepper_rot.runToNewPosition(800);
    stepper_rot.runToNewPosition(-800);
    init_state = 7;
    Initialization_Flag = true;
  }
  
  else {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, LOW);
    speed_Y = zeroSpeed;
    speed_X = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    analogWrite(X_Motor, speed_X);
  }
}



/*****************************************************************/
/* LIMIT ISRs
 *  Associates each limit switch with a state 
 */
 /*****************************************************************/
void limitState_Y_min() {
  if (but_interrupt_flag){ digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    speed_Y = Left;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
    if (digitalRead(limSwitch2) == HIGH && init_state == 0){
      but_interrupt_flag = 0;
      debounce(limSwitch2);
      init_state = 1;
    }
  }
}

void limitState_Y_max() {
  if (but_interrupt_flag){
    speed_Y = Right;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
    if (digitalRead(limSwitch1) == HIGH && init_state == 1){
      but_interrupt_flag = 0;
      debounce(limSwitch1);
      init_state =  2;
    }
  }
}

void limitState_X_max() {
  if (but_interrupt_flag){
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    speed_X = Backward;
    analogWrite(X_Motor, speed_X);
    speed_Y = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    if (digitalRead(limSwitch3) == HIGH && init_state == 4){
      but_interrupt_flag = 0;
      debounce(limSwitch3);
      init_state = 5;
    }
    else if (digitalRead(limSwitch4) == HIGH && init_state == 4){
      but_interrupt_flag = 0;
      debounce(limSwitch4);
      init_state = 5;
    }
  }
}

void limitState_X_min() {
  if (but_interrupt_flag){
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    speed_X = Forward;
    analogWrite(X_Motor, speed_X);
    speed_Y = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    if (digitalRead(limSwitch5) == HIGH && init_state == 3){
      but_interrupt_flag = 0;
      debounce(limSwitch5);
      init_state = 4;
    }
//    if (digitalRead(limSwitch6) == HIGH && init_state == 3){
//      but_interrupt_flag = 0;
//      debounce(limSwitch6);
//      init_state = 4;
//    }
  }
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
