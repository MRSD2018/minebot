#include "Arduino.h"
#include "ForceSensor.h"

ForceSensor::ForceSensor(int pin)
{
	pinMode(pin, INPUT);
	_pin = pin;
}

int ForceSensor::getRawOutput(){
	return analogRead(_pin);
}

float ForceSensor::getForce(){ // outputs force [grams]
  float V = (getFilteredOutput()/1023.0)*5.0;
  return exp((V-0.5975)/0.5459);
}

float ForceSensor::getFilteredOutput(){
  // code from: https://www.arduino.cc/en/Tutorial/Smoothing	
  total = total - readings[readIndex];
  readings[readIndex] = getRawOutput();
  total = total + readings[readIndex];
  readIndex++;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  return total / numReadings;
}

void ForceSensor::initDataArray(){
	// start with an array of zeros
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}
