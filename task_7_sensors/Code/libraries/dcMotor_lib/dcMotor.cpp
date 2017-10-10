#include "Arduino.h"
#include "dcMotor.h"

dcMotor::dcMotor(int PWM, int L1, int L2, int chanA, int chanB)
{
	pinMode(PWM, OUTPUT);
	pinMode(L1,OUTPUT);
	pinMode(L2,OUTPUT);
	pinMode(chanA,OUTPUT);
	pinMode(chanB,OUTPUT);

	_PWM = PWM;
	_L1 = L1;
	_L2 = L2;
	_chanA = chanA;
	_chanB = chanB;
}

void dcMotor::ccw()
{
	digitalWriteFast(_L1,HIGH);
	digitalWriteFast(_L2,LOW);
}

void dcMotor::cw()
{
	digitalWriteFast(_L1,LOW);
	digitalWriteFast(_L2,HIGH);
}

void dcMotor::speed(float percentage) //0 to 100
{
	int val = (int)(255*percentage/100);
	analogWrite(_PWM, val);
}

void dcMotor::fullControl(float val)
{
	if (val == 0.0){speed(0.0);}
	else if (val < 0.0)
	{
		cw();
		speed(abs(val));
	}
	else
	{
		ccw();
		speed(val);
	}
}

void dcMotor::posPID(const double commandedPositionDeg) {
  // calculate current time and timestep
  nowTime = millis();
  dt = double(nowTime - prevTime); // note this is in milliseconds still

  // get current position
  actualPositionDeg = (double)encoderTicks * holesPerRev * revPerCycle / 360; // 360 degrees in revolution

  // calculate errors
  positionError = commandedPositionDeg - actualPositionDeg;
  positionErrorSum += positionError * dt;

  // calculate PWM
  pwmToWrite =  kp * positionError
                + kd * (positionError - prevPositionError) / dt
                + ki * (positionErrorSum);
  motorInputScaled = (double)pwmToWrite / 255 * 100;

  // drive motor
  if (motorInputScaled > 100) {
    motorInputScaled = 100;
  } else if (motorInputScaled < -100) {
    motorInputScaled = -100;
  }
  fullControl(motorInputScaled);

  // update values for next timestep
  prevPositionError = positionError;
  prevTime = nowTime;
}
