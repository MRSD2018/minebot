//#include "digitalWriteFast.h"
#include "dcMotor.h"
#include "ForceSensor.h"

#define PWM 6
#define L1 4
#define L2 5
#define channelAPin 3
#define channelBPin 7

dcMotor motor(PWM, L1, L2, channelAPin, channelBPin);
ForceSensor force(A1);

volatile bool channelAVal;
volatile bool channelBVal;
volatile long encoderTicks;// = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
  force.initDataArray();
}

void loop() {
//  Serial.println(force.getForce());
//  Serial.println(encoderTicks);
  motor.posPID(force.getForce(), encoderTicks);
  // motor.vel(force.getForce());
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
