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

int desired_steps = 200;

void setup() {
  Serial.begin(115200);
  stepper.begin(RPM,MICROSTEPS);
//  stepper.enable();
  stepper.setSpeedProfile(stepper.LINEAR_SPEED, 2000, 1000);
//  stepper.startRotate(360);
}

void loop() 
{
  //maybe we can dynamically change the desired_steps, then within the if caase choose direction.

  stepper.startRotate(360); //negative can go the other direction
  //startRotate can be used for direction, and we don't care about the number of steps,
  //then we will use below .nextAction to determine how to count the steps
  static int step = 0;
  unsigned wait_time = stepper.nextAction();
  if (wait_time && step<= desired_steps){
    stepper.enable();
    Serial.print("  step="); Serial.print(step++); //depends on how
    Serial.println();
  } 
  else 
  {
    stepper.disable();
    Serial.println("END");
    delay(5000);
    step = 0;
    desired_steps = desired_steps + 200;
    stepper.enable();
    stepper.startRotate(360); //negative can go the other direction
  }
}


