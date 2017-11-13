#include "digitalWriteFast.h"

//Pins
#define Speed 30
//#define channelAPin 34
//#define channelBPin 33
#define limitSwitch 28

//LEDs
//#define LED1 5
//#define LED2 6
//#define LED3 7
//#define LED4 8
//#define Enable 11

//constants
static float encoderTicksPerRotation = 659.232/2;
static float teethPerRotation = 72;
static float toothPitch = .2;

//encoder variables
volatile bool channelAVal;
volatile bool channelBVal;
volatile int encoderTicks;

//switch variables
volatile int but_interrupt_flag = 0;
const int debounce_time = 20;  //ms
int last_switch_time = 0;
const int debounce_timeout = 1000; // ms
int initCounter = 0;
volatile int limitL;
volatile int limitR;

//positioning variables
int dist;
int posInTicks;
float posInCM;
int stopTicks;
int zeroSpeed = 127;
int newPos;

//Contral variables
double nowTime;
double prevTime=0;
double dt;
float kd = .3;
float kp = .3;
float ki = 0;
int currentPos;
int positionError;
double positionErrorSum;
double pwmToWritePos;
int motorInputScaledPos;
double prevPositionError;
int newSpeed;


void setup() {
  Serial.begin(9600);

  //ISR
//  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(limitSwitch), limit_switch, RISING); //change State when B0 input is rising

  //Pin initialization
  pinMode(limitSwitch,INPUT);
  pinMode(Speed, OUTPUT);
//  pinMode(channelAPin, INPUT);
//  pinMode(channelBPin, INPUT);

//  //Motor initialization
//  analogWrite(Speed, zeroSpeed);
//  digitalWrite(Enable, LOW);
//
//  //LED Init
//  pinMode(LED1, OUTPUT);
//  digitalWrite(LED1, LOW);
//  pinMode(LED2, OUTPUT);
//  digitalWrite(LED2, LOW);
//  pinMode(LED3, OUTPUT);
//  digitalWrite(LED3, LOW);
//  pinMode(LED4, OUTPUT);
//  digitalWrite(LED4, LOW);
}

void loop() {

  //Initialization Step
  Serial.println(but_interrupt_flag);
  if (but_interrupt_flag){
    debounce(limitSwitch);
//    if (initCounter == 1) {
//      limitL = encoderTicks;
//      Serial.print("Limit L = "); Serial.println(limitL);
//      digitalWrite(LED1, HIGH);
//    }
//    if (initCounter == 2) {
//      limitR = encoderTicks;
//      dist = abs(limitL - limitR); 
//      Serial.print("Limit R = "); Serial.println(limitR);
//      digitalWrite(LED2, HIGH);
//    }
//    if (initCounter > 4){
//      digitalWrite(Enable, LOW);
//      analogWrite(Speed, zeroSpeed);
//    }
    initCounter ++;
    Serial.println(initCounter);
    but_interrupt_flag = 0;
  }

//  //Run System
//  if (initCounter > 2) {
//    posInTicks = map(encoderTicks, 0, dist, limitL, limitR);
//    posInCM = posInTicks*teethPerRotation*toothPitch/encoderTicksPerRotation;
//    Serial.print(dist); Serial.print("  ==>  ");Serial.print(posInTicks); Serial.print("  ==>  "); Serial.println(posInCM);
//  }
//
//  //Sweeping Demo
//  if (initCounter == 3) {
//    digitalWrite(Enable, HIGH);
//    if (posInTicks < 100) {
//      analogWrite(Speed, 50);
//    }
//    if (posInTicks > (dist-100)) {
//      analogWrite(Speed, 205);
//    }
//  }
//
//
//  //Position Control Demo
//  if (initCounter > 4){
//    digitalWrite(Enable, HIGH);
//    digitalWrite(LED4, HIGH);
//    analogWrite(Speed, zeroSpeed);
//    if (Serial.available() > 0){
//      newPos = Serial.readString().toInt();
//      Serial.println(newPos);
//      Serial.println(dist);
//    }
//    if (newPos < dist && newPos > 0){
//      // calculate current time and timestep
//      nowTime = millis();
//      dt = double(nowTime - prevTime); // note this is in milliseconds still
//    
//      // get current position
//      currentPos = (double)posInTicks; 
//    
//      // calculate errors
//      positionError = newPos - currentPos;
//      positionErrorSum += positionError * dt;
//    
//      // calculate PWM
//      pwmToWritePos =  kp * positionError
//                    + kd * (positionError - prevPositionError) / dt
//                    + ki * (positionErrorSum);
//      motorInputScaledPos = zeroSpeed - pwmToWritePos;
//      if (motorInputScaledPos > 255){ motorInputScaledPos = 255; }
//      if (motorInputScaledPos < 0){ motorInputScaledPos = 0; }
//      analogWrite(Speed, motorInputScaledPos);
//      Serial.println(motorInputScaledPos - 127);
//    
//      // update values for next timestep
//      prevPositionError = positionError;
//      prevTime = nowTime;
//    }
//  }
  
}

//debouncing
void debounce(int button){
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout){
    last_switch_time = cur_time;
  }
}

//Initialize the system
void limit_switch() {
  Serial.print("HERE");
  if (but_interrupt_flag == 0) {
    but_interrupt_flag = 1;
  }
}

////Encoder Counts
//void encoderCount()
//{
//  channelAVal = digitalReadFast(channelAPin);
//  channelBVal = digitalReadFast(channelBPin);
//  if (channelAVal == channelBVal) {
//    encoderTicks++;
//  }
//  else {
//    encoderTicks--;
//  }
//}


