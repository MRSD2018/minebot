void setupIRStepper()
{
  ir_stepper = IR_Stepper(irSensor, stepperStep, stepperDir, stepperEn);
  Timer1.initialize(250000); // microseconds
  Timer1.attachInterrupt( [] () { ir_stepper.go_to_des_pos(); } ); 
  Timer1.stop();
  ir_stepper.start_running(); 
}

void irStepper(){ir_stepper.step();}
