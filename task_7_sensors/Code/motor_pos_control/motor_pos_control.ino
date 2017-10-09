#include <digitalWriteFast.h> //thank you jrraines!! 
//https://code.google.com/archive/p/digitalwritefast/downloads

#define PWM 6
#define L1 4        //L1 high L2 low counter clockwise, L1 low L2 high clockwise
#define L2 5
#define channelAPin 3
#define channelBPin 7
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
double kd = 0.1;
double ki = 0;

String serialstuff;
long rpmDesired = 40;
long pwmToWrite = 0;

long pos = 0;
long posPast = 0;
long goalPos = 0;
long deg = 0;

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

  while(Serial.available())
  {
    serialstuff = Serial.readString();
    deg = serialstuff.toInt();
    Serial.print(deg);
  }

//Position Control
  revo = (double)encoderTicks/(holesPerRevo * resPerCycle);  //revolutions in float
  now = micros();
  pos = posPast + ((revo-pastRevo) * 360) ; //finding position in deg

//PD control
  goalPos = (kd*((double)(deg - pos))) + (kp*(double)(revo-pastRevo)*(360/(now-past/1000000)));
  pwmToWrite = abs(goalPos*100); //Set motor speed
  if(pwmToWrite>255){pwmToWrite = 255;} //Limit max PWM
  if(pwmToWrite<255){pwmToWrite = 0;} //Limit min PWM
  
  Serial.println(goalPos);

  //Motor CCW
  if(goalPos<0){
    ccw();
    analogWrite(PWM, pwmToWrite);
  }
  //Motor CW
  if(goalPos>0) {
    cw();
    analogWrite(PWM, pwmToWrite);
  }

  //Update
  pastRevo = revo;
  posPast = pos;
  past = now;
  
  //calculate the revolutions
//  revo = (double)encoderTicks/(holesPerRevo * resPerCycle);  //revolutions in float
//
//  now = micros();
//  rpm = (revo - pastRevo)*60/((double)(now-past)/1000000);  //finding the rpm
//  pastRevo = revo;
//  past = now;
//
//  pwmToWrite += (kp*((double)rpmDesired-rpm));
//  if(pwmToWrite>255){pwmToWrite = 255;}
//  else if(pwmToWrite<0){pwmToWrite = 0;}

//  analogWrite(PWM,pwmToWrite);  //analogWrite only allows 0 to 255 on UNO
//  analogWrite(PWM,100);
//  Serial.print(pwmToWrite);
//  Serial.print("\t");
//  Serial.println(rpm);
}

void handleChannelA()
{
  channelAVal = digitalReadFast(channelAPin);
  channelBVal = digitalReadFast(channelBPin);
  if(channelAVal == channelBVal){encoderTicks--;} 
  else{encoderTicks++;}//cw as positive
}

void ccw()
{
  digitalWriteFast(L1,HIGH);
  digitalWriteFast(L2,LOW);
}

void cw()
{
  digitalWriteFast(L1,LOW);
  digitalWriteFast(L2,HIGH);
}


