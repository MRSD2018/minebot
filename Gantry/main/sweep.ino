//saftey buffer
double buffer = 300;
double slowZone = 2000;

void sweep() {
//  if (Initialization_Flag) {
    if (Y_encoderTicks < buffer) {
      set_speed(zeroSpeed, sweepLeft);
      if (Debug) {Serial.println("going left");}
    }
    else if (Y_encoderTicks > Y_max-buffer){
      set_speed(zeroSpeed, sweepRight);
      if (Debug) {Serial.println("going right");}
    }
//    else if (Y_encoderTicks < slowZone && speed_Y < 90) {
//      set_speed(zeroSpeed, .7*sweepRight*Y_encoderTicks/slowZone);
//    }
//    else if (Y_encoderTicks > Y_max - slowZone && speed_Y > 90) {
//      set_speed(zeroSpeed, 1.5*sweepLeft*(Y_max - Y_encoderTicks)/slowZone);
//    }
//  }
}

