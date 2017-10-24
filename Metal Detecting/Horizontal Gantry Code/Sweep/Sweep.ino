#include <Arduino.h>

#define MOTOR_STEPS 200 //how many steps per revo
#define RPM 120
#define MICROSTEPS 16

#define DIR 10
#define STEP 8
#define ENABLE 11

#include "DRV8834.h"
#define M0 10  //not sure what these are, haven't checked deeper into the libs
#define M1 11
DRV8834 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, M0, M1);

//Limit Switchds
#define B0 2

int desired_steps = 200;

void setup() {
  Serial.begin(115200);
  stepper.begin(RPM,MICROSTEPS);
//  stepper.enable();
  stepper.setSpeedProfile(stepper.LINEAR_SPEED, 2000, 1000);
//  stepper.startRotate(360);

  //Enable Interrupts
  attachInterrupt(0, Limit_1, LOW); //change State when B0 input is low
  //attachInterrupt(1, Limit_2, LOW); //change LED state when B1 input is rising
}

//Switch motor direction when limit switch is contacted
void Limit_1() {
  RPM = -1*RPM;
}

void loop() 
{
  //maybe we can dynamically change the desired_steps, then within the if caase choose direction.
  stepper.enable();
  stepper.setSpeedProfile(CONSTANT_SPEED, short accel=1000, short decel=1000);
  stepper.setRPM(RPM);
  
}


