
/**************************************************************************/
/*
    Setup ROS messanger
*/
/**************************************************************************/
void rosSetup(void)
{
//  Serial.begin(115200);

  delay(1000);

  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(gantryStatPub);
}

/**************************************************************************/
/*
    Assign values to gantry_status messages
   * gantry_status.x ==> x position of the gantry plate
   * gantry_status.y ==> STATE
   * gantry_status.z ==> Right now just 1... eventually wheel odometry pos.
*/
/**************************************************************************/
void readyGantryStatus()
{
  gantry_status.x = posInCM;
  gantry_status.y = STATE;
  gantry_status.z = 1;
  delay(1000);
}
