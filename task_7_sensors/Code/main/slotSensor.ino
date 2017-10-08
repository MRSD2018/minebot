//ADD THESE AS DECLARATIONS
/*
#define slotSensor(12)
*/

//ADD THIS INTO setup()
/*
pinMode(slotSensor,INPUT);
*/

//ADD THESE INTO loop()
/*
if(digitalRead(slotSensor)){slotOpen();}
else{slotClosed();}
*/

/*-------------------------------------------------*/

void slotOpen()
{
  //define motor direction here
  //add other functions?
}

void slotClosed()
{
  //define motor direction here
  //add other functions?
}

