void setupIRStepper()
{
  ir_stepper = IR_Stepper(irSensor, stepperStep, stepperDir, stepperEn);
}

void startStepper()
{
  Timer1.initialize(250000); // microseconds
  Timer1.attachInterrupt( [] () { ir_stepper.go_to_des_pos(); } ); 
  Timer1.stop();
  ir_stepper.stop_running();
  ir_stepper.start_running(); 
}

void stopStepper()
{
  ir_stepper.stop_running();
}

void irStepper(){
  ir_stepper.step();

  // Serial Monitor
  serialMotorActual = ir_stepper.get_current_position();
  serialMotorDesired = ir_stepper.get_desired_position();
  serialSensor = ir_stepper.get_ir_sensor();
}
