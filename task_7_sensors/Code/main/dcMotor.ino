void setupForce(){force.initDataArray();}

void posForce(){
  motor.posPID(force.getForce(), encoderTicks);

  // Serial Monitor
  serialMotorActual = motor.get_current_position();
  serialMotorDesired = force.getForce();
  serialSensor = force.getForce();
}

void velForce(){
  motor.vel(slotSensorDirection()*force.getForce());

  // Serial Monitor
  serialMotorActual = slotSensorDirection()*force.getForce();
  serialMotorDesired = slotSensorDirection()*force.getForce();
  serialSensor = force.getForce();
}
