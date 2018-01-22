// stupid simple library, uses interrupts if available, otherwise has to read continuously (poor performance)
// https://github.com/PaulStoffregen/Encoder
#include "Encoder.h"

// basically, let's you use more pins as interrupts, and we need more than two
#include <PinChangeInt.h>

// this is how the motor controller I'd like to use works
#define motorSpeedPin A0
#define motorDirPin A1

// encoders on dedicated external interrupt pins
#define channelAPin 2
#define channelBPin 3

// there's a perf advantage to putting the button on a different pin grouping (analogs, 0-7, 8-13)
#define buttonPin 8

Encoder enc(channelAPin, channelBPin);

volatile int button_interrupt_flag = 0;
int last_switch_time = 0;
const int debounce_timeout = 1000; // ms

// the last five encoder readings
int encoderTicks[5] = {0};
int encoderTimes[5] = {0};
int encoderIndex = 0;
int motor_direction;

bool speed_control;
float speed_setpoint;
float position_setpoint;

void setup() {
  Serial.begin(115200);
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  PCintPort::attachInterrupt(buttonPin, limitSwitch, RISING); 
  motor_direction = 1;

  // put info in here to run at speed or to position
  speed_control = true;
  speed_setpoint = 0.0f;
  position_setpoint = 0.0f;
  
  analogWrite(motorSpeedPin, 0.0f);
  analogWrite(motorDirPin, 5.0f);
}

void loop() {
  int enc_count = enc.read();
  Serial.println(enc_count);

  // used for speed control
  encoderTicks[encoderIndex] = enc_count;
  encoderTimes[encoderIndex] = millis();
  encoderIndex = (encoderIndex + 1) % 5;

  // read serial commands for now?

  // then execute
  if (speed_control)
  {
    //do_speed_control();
  }
  else // do position control
  {
    //do_pos_control();
  }
}

//Change motor direction when limit switch is hit
void limitSwitch(){
  //Debounce code
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout)
  {
    last_switch_time = cur_time;
    motor_direction *= -1;
  }
}

