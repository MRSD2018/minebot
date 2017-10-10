#include <ir_stepper.h>

// this does need to be a global
IR_Stepper ir_stepper;

void setup() {
  Serial.setTimeout(100);
  Serial.begin(9600);

  // this block needs to be in setup
  ir_stepper = IR_Stepper(A5, 8, 7);
  Timer1.initialize(250000); // microseconds
  Timer1.attachInterrupt( [] () { ir_stepper.go_to_des_pos(); } ); 
  Timer1.stop();

  // do this to start running
  // can't run all the time because the stepper will overheat
  ir_stepper.start_running(); 
  // do "ir_stepper.stop_running(); " to stop
}

void loop() {
  // this line needs to be in loop
  // this sets the desired position of the stepper
  ir_stepper.step(); 
}
