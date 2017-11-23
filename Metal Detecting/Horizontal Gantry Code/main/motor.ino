/**************************************************************************/
/*
    Setup motor
*/
/**************************************************************************/
void motorSetup(){
  pinMode(PWM, OUTPUT);
  digitalWrite(PWM, LOW);
}

/*****************************************************************/
/* INITIALIZE
 *  Calculate the position of the gantry endstops by manually moving the gantry plate
 * between the two limit switches. The limit switch near the motor is always first and 
 * is the "zero" position. Calculate the length of the gantry in encoder ticks and cm. 
 */
 /*****************************************************************/
void initialize() {
  
  //limit indicator value signifies that both limit switches have been activated in the correct order
  if (limitIndicator == 2){
    dist = abs(limitFar - limitNear); //calculate the distance between the limit switches in encoder ticks
    distInCM = dist*teethPerRotation*toothPitch/encoderTicksPerRotation; //calculate the distance in cm
    
    Serial.println("Initialization Complete!");
    Serial.print("Distance in cm ==> "); Serial.println(distInCM);
    
    limitIndicator = 3; //update to push limit switches into error mode
  }
}


/*****************************************************************/
/* GANTRY SWEEP
 *  Move the gantry plate at a set rate between two pre-determined endstops 100 
 *  encoder ticks from either limit switch.
 */
 /*****************************************************************/
void sweep() {

  //map current gantry plate position
  posInTicks = map(encoderTicks, 0, dist, limitNear, limitFar); //calculate current position in encoder ticks
  posInCM = posInTicks*teethPerRotation*toothPitch/encoderTicksPerRotation; // calculate current position in cm
  Serial.print(dist); Serial.print("  ==>  ");Serial.print(posInTicks); Serial.print("  ==>  "); Serial.println(posInCM);

  //If gantry plate is too close to end-stops, swich motor direction
  if (posInTicks < 200) {
    analogWrite(PWM, 200);
    Serial.println("FORWARD");
  }
  if (posInTicks > (dist-200)) {
    Serial.println("BACKWARD");
    analogWrite(PWM, 30);
  }
}

/*****************************************************************/
/* POSITION CONTROL
 *  Given a position serial command, move to that position using 
 *  PID control.
 */
 /*****************************************************************/
void posControl() {

  //map current gantry plate position
  posInTicks = map(encoderTicks, 0, dist, limitNear, limitFar);

  //wait for serial position command. Integer. 
  if (Serial.available() > 0){
    newPos = Serial.readString().toInt();
  }

  //Move gantry plate if position command is valid.
  if (newPos < dist && newPos > 0){
    // calculate current time and timestep
    nowTime = millis();
    dt = (double)(nowTime - prevTime); // note this is in milliseconds still

    // get current position
    currentPos = (double)posInTicks;

    // calculate errors
    positionError = newPos - currentPos;
    positionErrorSum += positionError * dt;

    // calculate PWM
    
    pwmToWritePos =  kp * positionError
                  + kd * (positionError - prevPositionError) / dt;
//                  + ki * (positionErrorSum);
    motorInputScaledPos = zeroSpeed + pwmToWritePos; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (motorInputScaledPos > 255){ motorInputScaledPos = 255; }
    if (motorInputScaledPos < 0){ motorInputScaledPos = 0; }

    // calculate current time and timestepive Motor
    analogWrite(PWM, motorInputScaledPos);
    Serial.print(currentPos); Serial.print("   ===>    "); Serial.print(newPos); Serial.print("   ===>    "); Serial.println(pwmToWritePos);

    // update values for next timestep
    prevPositionError = positionError;
    delay(1); //delay to ensure a difference between prevTime and nowTime
    prevTime = nowTime;
  }
  else {
    analogWrite(PWM, zeroSpeed);
  }
}


