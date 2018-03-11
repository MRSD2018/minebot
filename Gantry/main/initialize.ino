//Timing Variables
int nowTime = 0;
int prevTime = 0;
double dt= 0;

//error variables
int Error = 10;
int positionError = 0;
double positionErrorSum = 0;
int prevPositionError = 0;

//Motor Variables
double pwmToWritePos = 0;

//Constants
double kp = 0.1;
double kd = 0;

///*****************************************************************/
///* POSITION CONTROL
// *  Given a position serial command, move to that position using 
// *  PID control.
// */
// /*****************************************************************/
int PIDControl_Y(int Y_goal) {

  Serial.println("PID CONTROL");

  //Move gantry plate if position command is valid.
  if (Y_goal < Y_max && Y_goal > Y_origin){
    
    // calculate current time and timestep
    nowTime = millis();
    dt = (double)(nowTime - prevTime); // note this is in milliseconds still

    // calculate errors
    positionError = Y_encoderTicks - Y_goal;
    positionErrorSum += positionError * dt;

    // calculate PWM
    
    pwmToWritePos =  kp * positionError
                  + kd * (positionError - prevPositionError) / dt;
//                  + ki * (positionErrorSum);
    speed_Y = zeroSpeed + pwmToWritePos; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (speed_Y > zeroSpeed+60){ speed_Y = zeroSpeed+60; }
    if (speed_Y < zeroSpeed-50){ speed_Y = zeroSpeed-50; }

    // calculate current time and timestepive Motor
    analogWrite(Y_Motor, speed_Y);
    Serial.print(Y_encoderTicks); Serial.print("   ===>    "); Serial.print(Y_goal); Serial.print("   ===>    "); Serial.println(pwmToWritePos);

    //If gantry is within the position error and not moving, set that the desired position has been reached
    if ((abs(positionError) <= Error) && (prevPositionError == positionError)){ 
      arrived = true;
    }
    else {
      arrived = false;
    }
    // update values for next timestep
    prevPositionError = positionError;
    delay(1); //delay to ensure a difference between prevTime and nowTime
    prevTime = nowTime;
  }
  return arrived;
}
