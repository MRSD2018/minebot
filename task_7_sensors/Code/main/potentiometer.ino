//ADD THESE AS DECLARATIONS
/*
#define pot A0
float potAngle;
*/

//ADD THIS INTO setup()
/*
pinMode(pot,INPUT);
*/

//ADD THESE INTO loop()
/*
float potAngle = getPotAngle(); //0 to 180 in 2 decimal places
*/

/*---------------------------------------------------------------------------------------------*/

float getPotAngle()
{
  int potIn = analogRead(pot);
  float angle = (map(potIn,0,1023,0,18000))/100; //to get 2 floating points
  return angle;
}

