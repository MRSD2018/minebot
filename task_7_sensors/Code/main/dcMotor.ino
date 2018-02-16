void setupForce(){force.initDataArray();}

void posForce(){

  float writeToMotor;
  if (useSensor)
    writeToMotor = force.getForce();
  else
    writeToMotor = manualInput;
      
  motor.posPID(writeToMotor, encoderTicks);

  // Serial Monitor
  serialMotorActual = motor.get_current_position();
  serialMotorDesired = writeToMotor;
  serialSensor = force.getForce();
}

void velForce(){

  float writeToMotor;
  if (useSensor)
    writeToMotor = slotSensorDirection()*force.getForce();
  else
    writeToMotor = manualInput;
  
  motor.vel(writeToMotor);

  // Serial Monitor
  serialMotorActual = writeToMotor;
  serialMotorDesired = writeToMotor;
  serialSensor = force.getForce();
}
