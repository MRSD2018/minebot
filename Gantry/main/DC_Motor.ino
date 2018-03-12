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
extern int zeroSpeed = 90;
extern volatile int speed_X = zeroSpeed;
extern volatile int speed_Y = zeroSpeed;
int Right = 20;
int Left = 180;
int Forward = 180;
int Backward = 20;

//Limit Switches
//Y axis switches
#define limSwitch1 34
#define limSwitch2 35
//X axis switches
#define limSwitch3 14
#define limSwitch4 22


//State
int init_state = 0;
int X_count = 0;
int Y_count = 0;
extern volatile bool arrived = false;

//Positioning Variables
extern volatile int X_origin = 0;
extern volatile int Y_origin = 0;
extern volatile int X_max = 0;
extern volatile int Y_max = 0;
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

  //Motor Init
  pinMode(X_Motor, OUTPUT);
  analogWrite(X_Motor, speed_X);
  pinMode(Y_Motor, OUTPUT);
  analogWrite(Y_Motor, speed_Y);

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
  attachInterrupt(digitalPinToInterrupt(limSwitch1),limitState_Y1, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch2),limitState_Y2, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch3),limitState_X1, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch4),limitState_X2, RISING);
}

/*****************************************************************/
/* INITIALIZATION ROUTINE
 *  Given a position serial command, move to that position using 
 *  PID control.
 */
 /*****************************************************************/
void initialize() {
Serial.print("X Speed:  "); Serial.print(speed_X);
Serial.print("   X Encoder:  "); Serial.print(X_encoderTicks);
Serial.print("   X_Max:  "); Serial.println(X_max);
Serial.print("   state:  "); Serial.println(init_state);
  //Serial.println("   Y-axis Init");
  if (init_state == 0) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    Y_encoderTicks = 0;
    speed_Y = Right;
    analogWrite(Y_Motor, speed_Y);
  }
  else if (init_state == 1) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    Y_max = abs(Y_encoderTicks);
    speed_Y = Left;
    analogWrite(Y_Motor, speed_Y);
  }
  else if (init_state == 2) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    Y_center = (Y_max)/2;
    speed_Y = Right;
    analogWrite(Y_Motor, speed_Y);
    if (abs(Y_encoderTicks) < Y_center) {
      speed_Y = zeroSpeed;
      speed_X = Backward;
      analogWrite(Y_Motor, speed_Y);
      analogWrite(X_Motor, speed_X);
      digitalWrite(relay_X, HIGH);
      digitalWrite(relay_Y, LOW);
      X_encoderTicks = 0;
    }
  }
  else if (init_state == 3){
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    X_max = abs(X_encoderTicks);
    speed_X = Forward;
    analogWrite(X_Motor, speed_X);
  }
  else if (init_state ==4) {
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    X_center = (X_max)/2;
    speed_X = Backward;
    analogWrite(X_Motor, speed_X);
    if (abs(X_encoderTicks) < X_center) {
      speed_Y = zeroSpeed;
      speed_X = zeroSpeed;
      analogWrite(Y_Motor, speed_Y);
      analogWrite(X_Motor, speed_X);
      digitalWrite(relay_X, LOW);
      digitalWrite(relay_Y, LOW);
    }
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
 *  Given a position serial command, move to that position using 
 *  PID control.
 */
 /*****************************************************************/
void limitState_Y1() {
  if (but_interrupt_flag){
    if (digitalRead(limSwitch1) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch1);
      init_state = 1;
    }
  }
}

void limitState_Y2() {
  if (but_interrupt_flag){
    if (digitalRead(limSwitch2) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch2);
      init_state = 2;
    }
  }
}

void limitState_X1() {
  if (but_interrupt_flag){
    if (digitalRead(limSwitch3) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch3);
      init_state = 3;
    }
  }
}

void limitState_X2() {
  if (but_interrupt_flag){
    if (digitalRead(limSwitch4) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch4);
      init_state = 4;
    }
  }
}

/*****************************************************************/
/* ENCODER ISRs
 *  Given a position serial command, move to that position using 
 *  PID control.
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
