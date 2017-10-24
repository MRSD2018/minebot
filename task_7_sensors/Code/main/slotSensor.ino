int slotSensorDirection()
{
  bool slot = digitalRead(slotSensor); 
  if(slot){return 1;} //ccw
  else{return -1;}    //cw
}

