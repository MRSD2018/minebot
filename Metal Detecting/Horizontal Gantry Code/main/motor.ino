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
    distInMM = 10*dist*teethPerRotation*toothPitch/encoderTicksPerRotation; //calculate the distance in mm
    
    Serial.println("Initialization Complete!");
    Serial.print("Distance in mm ==> "); Serial.println(distInMM);
    
    limitIndicator = 3; //update to push limit switches into error mode
    Initialized = 1; //Set inidcator to initialized 
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
  posInMM = 10*posInTicks*teethPerRotation*toothPitch/encoderTicksPerRotation; // calculate current position in mm
  Serial.print(dist); Serial.print("  ==>  ");Serial.print(posInTicks); Serial.print("  ==>  "); Serial.println(posInMM);

  //If gantry plate is too close to end-stops, swich motor direction
  if (posInTicks < 200) {
    analogWrite(PWM, zeroSpeed+60);
    Serial.println("FORWARD");
  }
  if (posInTicks > (dist-200)) {
    Serial.println("BACKWARD");
    analogWrite(PWM, zeroSpeed-50);
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
  posInMM = 10*posInTicks*teethPerRotation*toothPitch/encoderTicksPerRotation; // calculate current position in mm

  //wait for serial position command. Integer. 
  if (probeStat == 1){
    newPos = desiredPos;
    posDesiredArrived = 0;
//    count = 0;
  }
  
  if (probeStat == 0) {
    analogWrite(PWM, zeroSpeed);
  }

  //Move gantry plate if position command is valid.
  if (newPos < dist && newPos > 0){
    
    // calculate current time and timestep
    nowTime = millis();
    dt = (double)(nowTime - prevTime); // note this is in milliseconds still

    // calculate errors
    positionError = newPos - posInMM;
    positionErrorSum += positionError * dt;

    // calculate PWM
    
    pwmToWritePos =  kp * positionError
                  + kd * (positionError - prevPositionError) / dt;
//                  + ki * (positionErrorSum);
    motorInputScaledPos = zeroSpeed + pwmToWritePos; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (motorInputScaledPos > zeroSpeed+60){ motorInputScaledPos = zeroSpeed+60; }
    if (motorInputScaledPos < zeroSpeed-50){ motorInputScaledPos = zeroSpeed-50; }

    // calculate current time and timestepive Motor
    analogWrite(PWM, motorInputScaledPos);
    Serial.print(posInMM); Serial.print("   ===>    "); Serial.print(newPos); Serial.print("   ===>    "); Serial.println(pwmToWritePos);

    //If gantry is within the position error and not moving, set that the desired position has been reached
    if ((abs(positionError) <= 10) && (prevPositionError == positionError)){ 
      posDesiredArrived = 1;
//      count += 1;
//      if (count <= 50) {posDesiredArrived = 1;}
//      if (count > 50) {posDesiredArrived = 0;}
    }
    
    // update values for next timestep
    prevPositionError = positionError;
    delay(1); //delay to ensure a difference between prevTime and nowTime
    prevTime = nowTime;
  }
}


