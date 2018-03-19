//saftey buffer
int buffer = 500;

//Sweep speeds
int sweepRight = 20;
int sweepLeft = 200;

void sweep() {
  analogWrite(X_Motor, zeroSpeed);
  if (Initialization_Flag) {
    if (Y_encoderTicks < buffer) {
      analogWrite(Y_Motor, sweepLeft);
      Serial.println("going left");
    }
    else if (Y_encoderTicks > Y_max-buffer){
      analogWrite(Y_Motor, sweepRight);
      Serial.println("going right");
    }
  }
}

