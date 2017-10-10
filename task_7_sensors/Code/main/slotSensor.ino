int slotSensorDirection()
{
  if(digitalRead(slotSensor)){return 1;} //ccw
  else{return -1;} //cw
}

