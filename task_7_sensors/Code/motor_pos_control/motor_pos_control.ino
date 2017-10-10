#include "digitalWriteFast.h" //thank you jrraines!! 
#include "dcMotor.h"
//https://code.google.com/archive/p/digitalwritefast/downloads

#define PWM 6
#define L1 4
#define L2 5
#define channelAPin 3
#define channelBPin 7
#define holesPerRev 180  //how many holes are there in the encoder for 1 motor output turn
#define revPerCycle 2 //if i read only RISING then it will be 1, CHANGE means both RISING and FALLING so = 2 

dcMotor motor(PWM, L1, L2);

volatile bool channelAVal;    //binary high or low from encoder
volatile bool channelBVal;
volatile long encoderTicks = 0;   //a tick is a change in channel A, both RISING and FALLING edges

float commandedPositionDeg;
float prevActualPositionDeg;
float prevPositionError;

double kp = 7;
double kd = 250;
double ki = 0.00018;

unsigned long prevTime;
float positionErrorSum;
float motorInputScaled;

unsigned long nowTime;
double dt;

float actualPositionDeg;
float positionError;

int pwmToWrite;
String serialInput;

void setup() {
  Serial.begin(9600);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(PWM, OUTPUT);

  pinMode(channelAPin, INPUT);
  pinMode(channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
}

void loop() {
  // calculate current time and timestep
  nowTime = millis();
  dt = double(nowTime - prevTime); // note this is in milliseconds still

  // parse serial input
  while (Serial.available()) {
    serialInput = Serial.readString();
    commandedPositionDeg = serialInput.toInt();
  }

  // get current position
  actualPositionDeg = encoderTicks * (double)holesPerRev * revPerCycle / 360; // 360 degrees in revolution

  // calculate errors
  positionError = commandedPositionDeg - actualPositionDeg;
  positionErrorSum += positionError * dt;

  // calculate PWM
  pwmToWrite =  kp * positionError
                + kd * (positionError - prevPositionError) / dt
                + ki * (positionErrorSum);
  motorInputScaled = (double)pwmToWrite / 255 * 100;

  // drive motor
  if (motorInputScaled > 100) {
    motorInputScaled = 100;
  } else if (motorInputScaled < -100) {
    motorInputScaled = -100;
  }
  motor.fullControl(motorInputScaled);

  // update values for next timestep
  prevPositionError = positionError;
  prevTime = nowTime;

  // print to serial monitor
  Serial.print("Encoder counts: ");
  Serial.print(encoderTicks);
  Serial.print("Commanded position: ");
  Serial.print(commandedPositionDeg);
  Serial.print("  Position error: ");
  Serial.println(positionError);
}

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

void ccw()
{
  digitalWriteFast(L1, HIGH);
  digitalWriteFast(L2, LOW);
}

void cw()
{
  digitalWriteFast(L1, LOW);
  digitalWriteFast(L2, HIGH);
}


