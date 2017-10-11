int potIn;
int Angle;

//potServo
//#define pot A0
//#define servoPWM 9

void setupPotServo()
{
  pinMode(pot, INPUT);
  pinMode(servoPWM, OUTPUT);
//  myservo.attach(servoPWM);
}

void potServo()
{
  potIn = analogRead(pot);
  Angle = map(potIn,0,1023,0,180);
  myservo.write(Angle);

  // Serial Monitor
  serialMotorActual = Angle;
  serialMotorDesired = Angle; // setpoint
  serialSensor = potIn;
}

