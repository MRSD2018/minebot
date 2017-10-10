#ifndef dcMotor_h
#define dcMotor_h

#include "Arduino.h"
#include "digitalWriteFast.h" //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

class dcMotor {
  public:
    dcMotor(int PWM, int L1, int L2, int channelAPin, int channelBPin);
    void cw();
    void ccw();
    void speed(float percentage); //by percentage?
    void fullControl(float val); //maps -100 percent to 100 percent, with direction
    void posPID(const double commandedPositionDeg);
    // void velPID(const double commandedVelocity);
  private:
    int _PWM, _L1, _L2, _channelAPin, _channelBPin;

    const int holesPerRev = 180;  //how many holes are there in the encoder for 1 motor output turn
    const int revPerCycle = 2; //if i read only RISING then it will be 1, CHANGE means both RISING and FALLING so = 2 

    // position control variables
    float prevActualPositionDeg;
    float prevPositionError;
    float positionErrorSum;
    float motorInputScaled;
    float actualPositionDeg;
    float positionError;

    // position control gains
    double kp = 7;
    double kd = 250;
    double ki = 0.00018;

    unsigned long prevTime;
    unsigned long nowTime;
    double dt;

    int pwmToWrite;
};

#endif