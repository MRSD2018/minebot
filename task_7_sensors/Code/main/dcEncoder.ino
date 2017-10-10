volatile bool channelAVal;
volatile bool channelBVal;
volatile long encoderTicks;

void setupEncoder(){attachInterrupt(digitalPinToInterrupt(channelAPin),encoderCount, CHANGE);}

//ISR
void encoderCount()
{
  channelAVal = digitalReadFast(channelAPin);
  channelBVal = digitalReadFast(channelBPin);
  if(channelAVal == channelBVal){encoderTicks++;} //ccw as positive
  else{encoderTicks--;}
}
