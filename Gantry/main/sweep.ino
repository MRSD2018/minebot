//saftey buffer
int buffer = 500;

void sweep() {
  if (Initialization_Flag) {
    if (Y_encoderTicks < buffer) {
      set_speed(zeroSpeed, sweepLeft);
      if (Debug) {Serial.println("going left");}
    }
    else if (Y_encoderTicks > Y_max-buffer){
      set_speed(zeroSpeed, sweepRight);
      if (Debug) {Serial.println("going right");}
    }
  }
}

