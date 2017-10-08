#include <digitalWriteFast.h> //thank you jrraines!! 
//https://code.google.com/archive/p/digitalwritefast/downloads

#define PWM 3
#define L1 4        //L1 high L2 low counter clockwise, L1 low L2 high clockwise
#define L2 5
#define channelAPin 2
#define channelBPin 6
#define holesPerRevo 180  //how many holes are there in the encoder for 1 motor output turn
#define resPerCycle 2 //if i read only RISING then it will be 1, CHANGE means both RISING and FALLING so = 2

volatile bool channelAVal;    //binary high or low from encoder
volatile bool channelBVal;
volatile long encoderTicks = 0;   //a tick is a change in channel A, both RISING and FALLING edges

long past = 0;   //time in microseconds
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

long startEncoder =0;

void setup() {
  Serial.begin(9600);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(PWM, OUTPUT);

  pinMode(channelAPin, INPUT); //should I do internal pullup?
  pinMode(channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(channelAPin), handleChannelA, CHANGE);   //attaching the callback
}

void loop() {
  cw();

  while(Serial.available())
  {
    serialstuff = Serial.readString();
    rpmDesired = serialstuff.toInt();
  }
  
  //calculate the revolutions
  revo = (double)encoderTicks/(holesPerRevo * resPerCycle);  //revolutions in float

  now = micros();
  rpm = (revo - pastRevo)*60/((double)(now-past)/1000000);  //finding the rpm
  pastRevo = revo;
  past = now;

  pwmToWrite += (kp*((double)rpmDesired-rpm));
  if(pwmToWrite>255){pwmToWrite = 255;}
  else if(pwmToWrite<0){pwmToWrite = 0;}

  analogWrite(PWM,pwmToWrite);  //analogWrite only allows 0 to 255 on UNO
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
  digitalWriteFast(L1,HIGH);
  digitalWriteFast(L2,LOW);
}

void ccw()
{
  digitalWriteFast(L1,LOW);
  digitalWriteFast(L2,HIGH);
}

void posControl(deg)
{
  startEncoder = encoderTickes;
  ticks = deg/2
  while ticks >= encoderTicks - startEncoder {
    if deg > 0 {
      cw();
      analogWrite(PWM, 50);
    }
    if deg < 0 {
      ccw();
      analogWrite(PWM, 50);
    }
  }
}

