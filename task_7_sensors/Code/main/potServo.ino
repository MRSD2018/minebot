int potIn;
int Angle;

void setupPotServo()
{
  pinMode(pot, INPUT);
  pinMode(3, OUTPUT);
  myservo.attach(PWM);
}

void potServo()
{
  potIn = analogRead(pot);
  Angle = map(potIn,0,1023,0,180);
  myservo.write(Angle);
}

