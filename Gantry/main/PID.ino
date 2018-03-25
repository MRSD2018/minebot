//Timing Variables
int nowTime = 0;
int prevTime = 0;
double dt= 0;

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
double kp_X = 5;
double kd_X = 0;
//Y
double kp_Y = 10;
double kd_Y = 0;

/*****************************************************************/
/* POSITION CONTROL
 *  Given a position serial command, move to that position using 
 *  PID control.
 */
 /*****************************************************************/
bool PIDControl(int X_goal, int Y_goal) {

  //Move gantry plate if position command is valid.
  if (X_goal < X_max && X_goal > 0 && Y_goal < Y_max && Y_goal > 0 && Initialization_Flag){
        
    // calculate current time and timestep
    nowTime = millis();
    dt = (double)(nowTime - prevTime); // note this is in milliseconds still

    // calculate errors X
    positionError_X = X_goal - X_encoderTicks;
    positionErrorSum_X += positionError_X * dt;

    // calculate errors X
    positionError_Y = Y_goal - Y_encoderTicks;
    positionErrorSum_X += positionError_Y * dt;

    // calculate PWM X
    pwmToWritePos_X =  kp_X * positionError_X
                  + kd_X * (positionError_X - prevPositionError_X) / dt;
//                  + ki * (positionErrorSum);
    speed_X = zeroSpeed + pwmToWritePos_X; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (speed_X > zeroSpeed+100){ speed_X = zeroSpeed+100; }
    if (speed_X < zeroSpeed-70){ speed_X = zeroSpeed-70; }

     // calculate PWM Y
    pwmToWritePos_Y =  kp_Y * positionError_Y
                  + kd_Y * (positionError_Y - prevPositionError_Y) / dt;
    speed_Y = zeroSpeed + pwmToWritePos_Y; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver

    //Truncate PWMs above and below limits
    if (speed_Y > zeroSpeed+100){ speed_Y = zeroSpeed+100; }
    if (speed_Y < zeroSpeed-60){ speed_Y = zeroSpeed-60; }

    // calculate current time and timestepive Motor
    set_speed(speed_X, speed_Y);
    if (Debug) {
      Serial.print(X_encoderTicks); Serial.print("   ===>    "); Serial.print(X_goal); Serial.print("   ===>    "); Serial.println(speed_X);
      Serial.print(Y_encoderTicks); Serial.print("   ===>    "); Serial.print(Y_goal); Serial.print("   ===>    "); Serial.println(speed_Y);
    }

    //If gantry is within the position error and not moving, set that the desired position has been reached
    if ((abs(positionError_X) <= Error_X) && (prevPositionError_X == positionError_X)){ 
      arrived_X = true;
    }
    else {
      arrived_X = false;
    }

    //If gantry is within the position error and not moving, set that the desired position has been reached
    if ((abs(positionError_Y) <= Error_Y) && (prevPositionError_Y == positionError_Y)){ 
      arrived_Y = true;
    }
    else {
      arrived_Y = false;
    }
    // update values for next timestep
    prevPositionError_X = positionError_X;
    prevTime = nowTime;
  }
  
  if (arrived_X && arrived_Y){ arrived = true;}
  else {arrived = false;}
  
}
 
//bool PIDControl_X(int X_goal) {
//
//  //Move gantry plate if position command is valid.
//  if (X_goal < X_max && X_goal > 0 && Initialization_Flag){
//    
//    arrived_X = false;
//    
//    // calculate current time and timestep
//    nowTime_X = millis();
//    dt_X = (double)(nowTime_X - prevTime_X); // note this is in milliseconds still
//
//    // calculate errors
//    positionError_X = X_goal - X_encoderTicks;
//    positionErrorSum_X += positionError_X * dt_X;
//
//    // calculate PWM
//    
//    pwmToWritePos_X =  kp_X * positionError_X
//                  + kd_X * (positionError_X - prevPositionError_X) / dt_X;
////                  + ki * (positionErrorSum);
//    speed_X = zeroSpeed + pwmToWritePos_X; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver
//
//    //Truncate PWMs above and below limits
//    if (speed_X > zeroSpeed+80){ speed_X = zeroSpeed+80; }
//    if (speed_X < zeroSpeed-30){ speed_X = zeroSpeed-30; }
//
//    // calculate current time and timestepive Motor
//    analogWrite(X_Motor, speed_X);
//    Serial.print(X_encoderTicks); Serial.print("   ===>    "); Serial.print(X_goal); Serial.print("   ===>    "); Serial.println(speed_X);
//
//    //If gantry is within the position error and not moving, set that the desired position has been reached
//    if ((abs(positionError_X) <= Error_X) && (prevPositionError_X == positionError_X)){ 
//      arrived_X = true;
//    }
//    else {
//      arrived_X = false;
//    }
//    // update values for next timestep
//    prevPositionError_X = positionError_X;
//    delay(1); //delay to ensure a difference between prevTime and nowTime
//    prevTime_X = nowTime_X;
//  }
//  return arrived_X;
//}
//
//bool PIDControl_Y(int Y_goal) {
//
//  //Move gantry plate if position command is valid.
//  if (Y_goal < Y_max && Y_goal > 0 && Initialization_Flag){
//
//    arrived_Y = false;
//    
//    // calculate current time and timestep
//    nowTime_Y = millis();
//    dt_Y = (double)(nowTime_Y - prevTime_Y); // note this is in milliseconds still
//
//    // calculate errors
//    positionError_Y = Y_goal - Y_encoderTicks ;
//    positionErrorSum_Y += positionError_Y * dt_Y;
//
//    // calculate PWM
//    
//    pwmToWritePos_Y =  kp_Y * positionError_Y
//                  + kd_Y * (positionError_Y - prevPositionError_Y) / dt_Y;
////                  + ki * (positionErrorSum);
//    speed_Y = zeroSpeed + pwmToWritePos_Y; //Calcualte PWM as a reference to the zero speed PWM for Sabertooth Driver
//
//    //Truncate PWMs above and below limits
//    if (speed_Y > zeroSpeed+80){ speed_Y = zeroSpeed+80; }
//    if (speed_Y < zeroSpeed-30){ speed_Y = zeroSpeed-30; }
//
//    // calculate current time and timestepive Motor
//    analogWrite(Y_Motor, speed_Y);
//    Serial.print(Y_encoderTicks); Serial.print("   ===>    "); Serial.print(Y_goal); Serial.print("   ===>    "); Serial.println(speed_Y);
//
//    //If gantry is within the position error and not moving, set that the desired position has been reached
//    if ((abs(positionError_Y) <= Error_Y) && (prevPositionError_Y == positionError_Y)){ 
//      arrived_Y = true;
//    }
//    else {
//      arrived_Y = false;
//    }
//    // update values for next timestep
//    prevPositionError_Y = positionError_Y;
//    delay(1); //delay to ensure a difference between prevTime and nowTime
//    prevTime_Y = nowTime_Y;
//  }
//  return arrived_Y;
//}
