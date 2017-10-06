#include <digitalWriteFast.h> //thank you jrraines!! 
//https://code.google.com/archive/p/digitalwritefast/downloads

#define PWM 3
#define L1 4
#define L2 5
#define channelAPin 2
#define channelBPin 6
#define ticksPerRevo 180
#define resPerCycle 2 //if i read only RISING then it will be 1

volatile bool channelAVal;
volatile bool channelBVal;
volatile bool channelAPrevVal;
volatile bool channelBPrevVal;
volatile long encoderTicks = 0;

long past = 0;
long now = 0;

double revo = 0;
double pastRevo = 0;
double rpm = 0;

double kp = 0.1;
double kd = 0;
double ki = 0;

String serialstuff;
long rpmDesired = 40;
long pwmToWrite = 0;

void setup() {
  Serial.begin(9600);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(PWM, OUTPUT);

  pinMode(channelAPin, INPUT); //should I do internal pullup?
  pinMode(channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(channelAPin), handleChannelA, CHANGE);
}

void loop() {
  cw();

  while(Serial.available())
  {
    serialstuff = Serial.readString();
    rpmDesired = serialstuff.toInt();
  }
  
  //calculate the revolutions
  revo = (double)encoderTicks/(ticksPerRevo * resPerCycle);

  now = micros();
  rpm = (revo - pastRevo)*60/((double)(now-past)/1000000);
  pastRevo = revo;
  past = now;

  pwmToWrite += (kp*((double)rpmDesired-rpm));
  if(pwmToWrite>255){pwmToWrite = 255;}
  else if(pwmToWrite<0){pwmToWrite = 0;}

  analogWrite(PWM,pwmToWrite);
//  analogWrite(PWM,100);
//  Serial.print(pwmToWrite);
//  Serial.print("\t");
  Serial.println(rpm);
}

void handleChannelA()
{
  channelAVal = digitalReadFast(channelAPin);
  channelBVal = digitalReadFast(channelBPin);
  if(channelAVal == channelBVal){encoderTicks++;} //cw as positive
  else{encoderTicks--;}
}

void cw()
{
  digitalWrite(L1,HIGH);
  digitalWrite(L2,LOW);
}

void ccw()
{
  digitalWrite(L1,LOW);
  digitalWrite(L2,HIGH);
}

