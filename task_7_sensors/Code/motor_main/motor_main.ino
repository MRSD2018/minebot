#include "dcMotor.h"
//#include <digitalWriteFast.h> //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

dcMotor dc_test(3, 4, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  dc_test.speed(50);
  dc_test.cw();
  delay(5000);
  dc_test.speed(80);
  delay(5000);
  dc_test.speed(50);
  delay(5000);
  dc_test.ccw();
  dc_test.speed(80);
  delay(5000);
  dc_test.speed(0);
  delay(10000);
}
