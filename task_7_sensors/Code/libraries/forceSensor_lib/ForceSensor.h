#ifndef FORCE_SENSOR_H
#define FORCE_SENSOR_H

#include "Arduino.h"

class ForceSensor
{
  public:
    ForceSensor(int pin);
    int getRawOutput();
    float getForce();
    float getFilteredOutput();
    void initDataArray(); // have to call this in setup() function
  private:
    int _pin;
    static const int numReadings = 20;
    int readings[numReadings];
    int readIndex = 0;
    int total = 0;
    int average = 0;
};

#endif
