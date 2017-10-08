//ADD THESE AS DECLARATIONS
/*
#define channelA 3
#define channelB 6
extern volatile int channelAVal;
extern volatile int channelBVal;
extern volatile long encoderTicks = 0;
*/

//ADD THIS INTO setup()
/*
attachInterrupt(digitalPinToInterrupt(channelA),handleChannelA,CHANGE);
*/

//ADD THESE INTO loop()
/*

*/

/*---------------------------------------------------------------------------------------------*/

//ISR
void handleChannelA()
{
  channelAVal = digitalReadFast(channelA);
  channelBVal = digitalReadFast(channelB);
  if(channelAVal == channelBVal){encoderTicks++;} //ccw as positive
  else{encoderTicks--;}
}
