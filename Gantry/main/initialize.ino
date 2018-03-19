//Timing Variables
//X
int nowTime_X = 0;
int prevTime_X = 0;
double dt_X= 0;
//Y
int nowTime_Y = 0;
int prevTime_Y = 0;
double dt_Y= 0;

//error variables
//X
int Error_X = 10;
int positionError_X = 0;
double positionErrorSum_X = 0;
int prevPositionError_X = 0;
//Y
int Error_Y = 10;
int positionError_Y = 0;
double positionErrorSum_Y = 0;
int prevPositionError_Y = 0;

//Motor Variables
double pwmToWritePos_X = 0;
double pwmToWritePos_Y = 0;

//Constants
//X
double kp_X = 0.1;
double kd_X = 0;
//Y
double kp_Y = 0.1;
double kd_Y = 0;

/*****************************************************************/
/* POSITION CONTROL
 *  Given a position serial command, move to that position using 
 *  PID control.
 */
 /*****************************************************************/
bool PIDControl_X(int X_goal) {

  Serial.println("PID CONTROL");

  //Move gantry plate if position command is valid.
  if (X_goal < X_max && X_goal > 0){
    
    // calculate current time and timestep
    nowTime_X = millis();
    dt_X = (double)(nowTime_X - prevTime_X); // note this is in milliseconds still

    // calculate errors
    positionError_X = X_encoderTicks - X_goal;
    positionErrorSum_X += positionError_X * dt_X;

    // calculate PWM
    
    pwmToWritePos_X =  kp_X * positionError_X
                  + kd_X * (positionError_X - prevPositionError_X) / dt_X;
//                  + ki * (positionErrorSum);
    speed_X = zeroSpeed + pwmToWritePos_X; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (speed_X > zeroSpeed+60){ speed_X = zeroSpeed+60; }
    if (speed_X < zeroSpeed-50){ speed_X = zeroSpeed-50; }

    // calculate current time and timestepive Motor
    analogWrite(X_Motor, speed_X);
    Serial.print(X_encoderTicks); Serial.print("   ===>    "); Serial.print(X_goal); Serial.print("   ===>    "); Serial.println(pwmToWritePos_X);

    //If gantry is within the position error and not moving, set that the desired position has been reached
    if ((abs(positionError_X) <= Error_X) && (prevPositionError_X == positionError_X)){ 
      arrived_X = true;
    }
    else {
      arrived_X = false;
    }
    // update values for next timestep
    prevPositionError_X = positionError_X;
    delay(1); //delay to ensure a difference between prevTime and nowTime
    prevTime_X = nowTime_X;
  }
  return arrived_X;
}

bool PIDControl_Y(int Y_goal) {

  Serial.println("PID CONTROL");

  //Move gantry plate if position command is valid.
  if (Y_goal < X_max && X_goal > 0){
    
    // calculate current time and timestep
    nowTime_Y = millis();
    dt_Y = (double)(nowTime_Y - prevTime_Y); // note this is in milliseconds still

    // calculate errors
    positionError_Y = Y_encoderTicks - Y_goal;
    positionErrorSum_Y += positionError_Y * dt_Y;

    // calculate PWM
    
    pwmToWritePos_Y =  kp_Y * positionError_Y
                  + kd_Y * (positionError_Y - prevPositionError_Y) / dt_Y;
//                  + ki * (positionErrorSum);
    speed_Y = zeroSpeed + pwmToWritePos_Y; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (speed_Y > zeroSpeed+60){ speed_Y = zeroSpeed+60; }
    if (speed_Y < zeroSpeed-50){ speed_Y = zeroSpeed-50; }

    // calculate current time and timestepive Motor
    analogWrite(Y_Motor, speed_Y);
    Serial.print(Y_encoderTicks); Serial.print("   ===>    "); Serial.print(Y_goal); Serial.print("   ===>    "); Serial.println(pwmToWritePos_Y);

    //If gantry is within the position error and not moving, set that the desired position has been reached
    if ((abs(positionError_Y) <= Error_Y) && (prevPositionError_Y == positionError_Y)){ 
      arrived_Y = true;
    }
    else {
      arrived_Y = false;
    }
    // update values for next timestep
    prevPositionError_Y = positionError_Y;
    delay(1); //delay to ensure a difference between prevTime and nowTime
    prevTime_Y = nowTime_Y;
  }
  return arrived_Y;
}