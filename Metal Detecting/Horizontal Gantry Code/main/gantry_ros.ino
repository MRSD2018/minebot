/** 
 *Run using: rosrun rosserial_python serial_node.py _baud:=115200 _port:=/dev/serial/by-id/usb-Teensyduino_USB_Serial_3376540-if00
 *rostopic echo gantryStat
**/

//Setup Subscriber
ros::Subscriber<std_msgs::Int16MultiArray> sub_probe("gantry_cmd_send", &gantryCommands);
ros::Subscriber<std_msgs::Int16> sub_probe_hack("gantry_cmd_hack_send", &gantryCommandsHack);

/**************************************************************************/
/*
    Setup ROS messanger
*/
/**************************************************************************/
void rosSetup(){

  delay(1000);

  nh.getHardware()->setBaud(115200);
  nh.initNode();
  gantry_stat.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension)*2);
  gantry_stat.layout.dim[0].label = "height";
  gantry_stat.layout.dim[0].size = 5;
  gantry_stat.layout.dim[0].stride = 1;
  gantry_stat.layout.data_offset = 0;
  gantry_stat.data = (short int *)malloc(sizeof(int)*8);
  gantry_stat.data_length = 5;
  nh.advertise(pub_gantry);

  nh.subscribe(sub_probe);
  nh.subscribe(sub_probe_hack);

}

/**************************************************************************/
/*
    PUBLISHER
    Assign values to gantry_status messages
   * gantry_stat.data[0] ==> STATE
   * gantry_stat.data[1] ==> Initialized?
   * gantry_stat.data[2] ==> Reached desired position?
   * gantry_stat.data[3] ==> gantry x (sweep) position
   * gantry_stat.data[4] ==> gantry y (wheel encoder) position
*/
/**************************************************************************/
void readyGantryStatus()
{
  gantry_stat.data[0] = stateReq;
  gantry_stat.data[1] = Initialized;
  gantry_stat.data[2] = posDesiredArrived;
  gantry_stat.data[3] = posInMM;
  gantry_stat.data[4] = wheelEncoderDist;  //wheelEncoderDist; 
  pub_gantry.publish(&gantry_stat);
  delay(50);
}


/**************************************************************************/
/*
    SUBSCRIBER
    Assign values to gantry_cmd_send messages
   * gantry_cmd_send.data[0] ==> Ready signal
   * gantry_cmd_send.data[1] ==> Desired Position
*/
/**************************************************************************/
void gantryCommands( const std_msgs::Int16MultiArray &gantry_cmd_send){
  probeStat = gantry_cmd_send.data[0];
  desiredPos = gantry_cmd_send.data[1];
}

/**************************************************************************/
/*
    SUBSCRIBER - gantry/gantry_cmd_hack_send
    Assign values to gantry_cmd_hack_send messages
   * gantry_cmd_send.data[0] ==> Ready signal
   * gantry_cmd_send.data[1] ==> Desired Position
*/
/**************************************************************************/
void gantryCommandsHack(const std_msgs::Int16 &gantry_cmd_hack_send){
  stateReq = gantry_cmd_hack_send.data;
}
