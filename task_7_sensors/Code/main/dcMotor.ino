//ADD THESE AS DECLARATIONS
/*
#include "dcMotor.h"
#include <digitalWriteFast.h> //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads
dcMotor dc(3, 4, 5);
*/

//ADD THIS INTO setup()
/*

*/

//ADD THESE INTO loop()
/*
  dc.fullControl(50);   //maps -100 to 100 in floats, takes care of position and power (as percentage)
  dc.fullControl(-80.5);  //for example, these are fine
  dc.ccw();
  dc.cw();
*/

/*---------------------------------------------------------------------------------------------*/

