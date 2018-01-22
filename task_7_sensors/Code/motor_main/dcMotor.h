#ifndef dcMotor_h
#define dcMotor_h

#include "Arduino.h"
#include "digitalWriteFast.h" //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

class dcMotor {
  public:
    dcMotor(int PWM, int L1, int L2);
    void cw();
    void ccw();
    void speed(float percentage); //by percentage?
    void fullControl(float val); //maps -100 percent to 100 percent, with direction

  private:
    int _PWM, _L1, _L2;
    // volatile bool chanAVal, chanBVal;
    // volatile long encoderTicks = 0;

};

#endif