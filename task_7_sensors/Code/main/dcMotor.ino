void setupForce(){force.initDataArray();}

void posForce(){motor.posPID(force.getForce(), encoderTicks);}
void velForce(){motor.vel(slotSensorDirection()*force.getForce());}
