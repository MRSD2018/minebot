#include <Arduino.h>

#define MOTOR_STEPS 400 //how many steps per revo
#define RPM 200
#define MICROSTEPS 1

#define DIR 10
#define STEP 8
#define ENABLE 11

#include "DRV8825.h"
#define M0 10  //not sure what these are, haven't checked deeper into the libs
#define M1 11
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, M0, M1);

#define BUTTON 2

int desired_steps = 200;
int dist = 2;
volatile int button_interrupt_flag = 0;
const int debounce_time = 20;  //ms

int last_switch_time = 0;
const int debounce_timeout = 1000; // ms

void setup() {
  Serial.begin(9600);
  stepper.begin(RPM,MICROSTEPS);
  
  pinMode(BUTTON, INPUT_PULLUP);
  //stepper.setSpeedProfile(stepper.LINEAR_SPEED, 2000, 1000);
  attachInterrupt(0, limitSwitch, RISING); //change State when B0 input is rising
}

//Change motor direction when limit switch is hit
void limitSwitch(){
  //Debounce code
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout)
  {
    last_switch_time = cur_time;
    dist *= -1; //change motor direction
    Serial.println(dist);
  }
}

void loop() 
{
  //move stepper in desired direction
    stepper.rotate(dist);
    Serial.println(dist);
}

