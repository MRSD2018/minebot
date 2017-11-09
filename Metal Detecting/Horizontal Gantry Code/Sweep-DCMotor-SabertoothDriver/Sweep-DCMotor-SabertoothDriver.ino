#include "digitalWriteFast.h"

//Pins
#define Speed 10
#define channelAPin 2
#define channelBPin 4
#define limitSwitch 3
#define inidcatorLED 6

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
const int debounce_time = 100;  //ms
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

void setup() {
  Serial.begin(9600);

  //ISR
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(limitSwitch), initialize, RISING); //change State when B0 input is rising

  //Pin initialization
  pinMode(limitSwitch,INPUT);
  pinMode(Speed, OUTPUT);
  pinMode(channelAPin, INPUT);
  pinMode(channelBPin, INPUT);

  //Motor initialization
  analogWrite(Speed, 0);
}

void loop() {

  //Initialization Step
  if (but_interrupt_flag){
    debounce(limitSwitch);
    delay(10);
    if (initCounter == 1) {
      limitL = encoderTicks;
      Serial.print("Limit L = "); Serial.println(limitL);
    }
    if (initCounter == 2) {
      limitR = encoderTicks;
      Serial.print("Limit R = "); Serial.println(limitR);
    }
    initCounter ++;
    Serial.println(initCounter);
    but_interrupt_flag = 0;
  }

  //Run System
  if (initCounter > 2) {
    dist = abs(limitL - limitR); 
    posInTicks = map(encoderTicks, 0, dist, limitL, limitR);
    posInCM = posInTicks*teethPerRotation*toothPitch/encoderTicksPerRotation;
//    Serial.print(posInTicks); Serial.print("  ==>  "); Serial.println(posInCM);

    //Sweeping Demo
    if (initCounter = 3) {
      if (posInTicks < 100) {
        analogWrite(Speed, 100);
        Serial.println(100);
      }
      if (posInTicks > (dist-100)) {
        analogWrite(Speed, 155);
        Serial.println(155);
      }
    }
  }
}

//debouncing
void debounce(int button){
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout){
    last_switch_time = cur_time;
  }
}

//Initialize the system
void initialize() {
  if (but_interrupt_flag == 0) {
    but_interrupt_flag = 1;
  }
}

//Encoder Counts
void encoderCount()
{
  channelAVal = digitalReadFast(channelAPin);
  channelBVal = digitalReadFast(channelBPin);
  if (channelAVal == channelBVal) {
    encoderTicks++;
  }
  else {
    encoderTicks--;
  }
}


