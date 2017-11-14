void initialize() {
  if (limitIndicator == 2){
    Serial.println("Initialization Complete!");
    dist = abs(limitFar - limitNear);
    distInCM = dist*teethPerRotation*toothPitch/encoderTicksPerRotation;
    Serial.print("Distance in cm ==> "); Serial.println(distInCM);
    limitIndicator = 3;
  }
}


void sweep() {
  posInTicks = map(encoderTicks, 0, dist, limitNear, limitFar);
  posInCM = posInTicks*teethPerRotation*toothPitch/encoderTicksPerRotation;
  Serial.print(dist); Serial.print("  ==>  ");Serial.print(posInTicks); Serial.print("  ==>  "); Serial.println(posInCM);
  if (posInTicks < 100) {
    analogWrite(PWM, 205);
    Serial.println("FORWARD");
  }
  if (posInTicks > (dist-100)) {
    analogWrite(PWM, 50);
    Serial.println("BACKWARDS");
  }
}

void posControl() {
  posInTicks = map(encoderTicks, 0, dist, limitNear, limitFar);
  if (Serial.available() > 0){
    newPos = Serial.readString().toInt();
    Serial.println(newPos);
  }
  if (newPos < dist && newPos > 0){
    // calculate current time and timestep
    active = 1;
    nowTime = millis();
    dt = double(nowTime - prevTime); // note this is in milliseconds still

    // get current position
    currentPos = (double)posInTicks;

    // calculate errors
    positionError = newPos - currentPos;
    positionErrorSum += positionError * dt;

    // calculate PWM
    pwmToWritePos =  kp * positionError;
//                  + kd * (positionError - prevPositionError) / dt
//                  + ki * (positionErrorSum);
    motorInputScaledPos = zeroSpeed + pwmToWritePos;
    if (motorInputScaledPos > 255){ motorInputScaledPos = 255; }
    if (motorInputScaledPos < 0){ motorInputScaledPos = 0; }
    analogWrite(PWM, motorInputScaledPos);
    Serial.print(currentPos); Serial.print("   ===>    "); Serial.print(newPos); Serial.print("   ===>    "); Serial.println(pwmToWritePos);

    // update values for next timestep
    prevPositionError = positionError;
    prevTime = nowTime;
  }
}


