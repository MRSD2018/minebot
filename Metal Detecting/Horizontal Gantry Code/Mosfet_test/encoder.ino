//encoder variables
volatile bool channelAVal;
volatile bool channelBVal;
volatile int encoderTicks;

//Setup DCmotor Encoder
void encoderSetup() {
  pinMode(channelAPin, INPUT);
  pinMode(channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
}

//Encoder Counts
void encoderCount(){
  channelAVal = digitalRead(channelAPin);
  channelBVal = digitalRead(channelBPin);
  if (channelAVal == channelBVal) {
    encoderTicks++;
  }
  else {
    encoderTicks--;
  }
  Serial.println(encoderTicks);
}
