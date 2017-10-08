#ifndef dcMotor_h
#define dcMotor_h

#include "Arduino.h"
#include "digitalWriteFast.h" //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

class dcMotor {
  public:
    dcMotor(int PWM, int L1, int L2, int chanA, int chanB);
    void cw();
    void ccw();
    void speed(); //by percentage?
    void handleChanA();
    long getTicks() const {return encoderTicks;}
    //get ticks
    //get rpm
    //get position

  private:
    int _PWM, _L1, _L2, _chanA, _chanB;
    volatile bool chanAVal, chanBVal;
    volatile long encoderTicks = 0;
    int stop = 0;

}

#endif