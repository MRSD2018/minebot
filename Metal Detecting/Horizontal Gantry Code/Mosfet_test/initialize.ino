void initialize() {
  if (limitIndicator == 2){
    Serial.println("Initialization Complete!");
    dist = abs(limitNear - limitFar);
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
    analogWrite(PWM, 50);
  }
  if (posInTicks > (dist-100)) {
    analogWrite(PWM, 205);
  }
}

