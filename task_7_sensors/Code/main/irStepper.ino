void setupIRStepper()
{
  ir_stepper = IR_Stepper(irSensor, stepperStep, stepperDir, stepperEn);
  ir_stepper.stop_running();
}

void startStepper()
{
  ir_stepper.start_running();
}

void stopStepper()
{
  ir_stepper.stop_running();
}

void irStepper() {

  ir_stepper.step();
  ir_stepper.go_to_des_pos(useSensor, manualInput);

  float writeToMotor;
  if (useSensor)
    writeToMotor = ir_stepper.get_desired_position();
  else
    writeToMotor = manualInput;

  // Serial Monitor
  serialMotorActual = ir_stepper.get_current_position();
  serialMotorDesired = writeToMotor;
  serialSensor = ir_stepper.get_ir_sensor();
}
