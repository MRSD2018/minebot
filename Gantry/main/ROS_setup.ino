//setup subscriber
ros::Subscriber<gantry::to_gantry_msg> sub("gantry_cmd", messageCb);

double cmToTicksY = 52.8562;
double ticksToCmY = 1/cmToTicksY;

double cmToTicksX = 805.53;
double ticksToCmX = 1/cmToTicksX;

void rosSetup() {
  
  delay(1000);
  
  if (!Debug) {
    nh.getHardware()->setBaud(115200); //115200
    nh.initNode();
  }
  
  nh.advertise(gantryStatus);
  nh.subscribe(sub);
  
}


/**************************************************************************/
/*
    PUBLISHER
    Assign values to gantry_status messages
   * gantry_status.state <== STATE
   * gantry_status.sweep_speed <== speed_Y
   * gantry_status.x <==  X_encoderTicks
   * gantry_status.y <==  Y_encoderTicks
   * gantry_status.yaw <== 
   * gantry_status.probe_angle <== 
   * gantry_status.position_reached <== arrived
*/
/**************************************************************************/
void readyGantryMsg()
{
   gantry_status.state = STATE;
   gantry_status.sweep_speed = speed_Y - 90;
   gantry_status.x =  X_encoderTicks * ticksToCmX;
   gantry_status.y =  Y_encoderTicks * ticksToCmY;
   gantry_status.yaw = 0;
   gantry_status.probe_angle = 0;
   gantry_status.position_reached = arrived;
}

/**************************************************************************/
/*
    SUBSCRIBER
    Assign values to gantry_cmd_send messages
   * to_gantry_msg.state_desired ==> STATE
   * to_gantry_msg.sweep_speed_desired ==> sweepRight/sweepLeft
   * to_gantry_msg.x_desired ==> X_desired
   * to_gantry_msg.y_desired ==> Y_desired
   * to_gantry_msg.yaw_desired ==> R_desired
   * gantry_status.probe_angle_desired; ==>
*/
/**************************************************************************/
void messageCb( const gantry::to_gantry_msg& gantry_cmd){
  STATE = gantry_cmd.state_desired;
  sweepRight = 90 - gantry_cmd.sweep_speed_desired;
  sweepLeft = 90 + gantry_cmd.sweep_speed_desired;
  if (sweepRight < 0) {sweepRight == 0;} 
  if (sweepLeft > 255) {sweepLeft == 255;} 
  X_desired = gantry_cmd.x_desired * cmToTicksX;
  Y_desired = gantry_cmd.y_desired * cmToTicksY;
  R_desired = gantry_cmd.yaw_desired;
}
