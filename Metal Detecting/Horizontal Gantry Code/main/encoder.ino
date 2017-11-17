//encoder variables
volatile bool channelAVal;
volatile bool channelBVal;
volatile int encoderTicks;

/**************************************************************************/
/*
    Setup CD motor encoder
*/
/**************************************************************************/
void encoderSetup() {
  pinMode(channelAPin, INPUT);
  pinMode(channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
}

/**************************************************************************/
/*
    Count every-other encoder tick (only using interrupt on channel A)
*/
/**************************************************************************/
void encoderCount(){
  channelAVal = digitalRead(channelAPin);
  channelBVal = digitalRead(channelBPin);
  if (channelAVal == channelBVal) {
    encoderTicks++;
  }
  else {
    encoderTicks--;
  }
}
