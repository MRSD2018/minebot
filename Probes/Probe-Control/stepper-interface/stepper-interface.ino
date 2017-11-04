// Code for interfacing with Load Cell
// Author: David Robinson

#include "AccelStepper.h"

#define UPPER_SWITCH_PIN 6
#define LOWER_SWITCH_PIN 5
#define STEP_PUL 9
#define STEP_DIR 8

AccelStepper stepper(1,STEP_PUL,STEP_DIR);

void setup() {

  stepper.setMaxSpeed(3000); // 3000
  stepper.setAcceleration(1000); // 1000
  
  pinMode(UPPER_SWITCH_PIN,INPUT);
  pinMode(LOWER_SWITCH_PIN,INPUT);

  Serial.begin(9600);
}

void loop() {

  int speed = 300;
  if (digitalRead(LOWER_SWITCH_PIN) == 0)
    stepper.setSpeed(speed);
  else
    stepper.setSpeed(-speed);

  stepper.runSpeed();

//  Serial.println(stepper.currentPosition());
//  Serial.print(digitalRead(UPPER_SWITCH_PIN));
//  Serial.print("\t");
//  Serial.print(digitalRead(LOWER_SWITCH_PIN));
  
}

void zero() // triggered from startup routine
{
  stepper.setCurrentPosition(0);
}
