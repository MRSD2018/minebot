// #include "Arduino.h"
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

void dcMotor::posPID(const double commandedPositionDeg, int encoderTicks) {
  // Serial.println(encoderTicks);

  // calculate current time and timestep
  nowTime = millis();
  dt = double(nowTime - prevTime); // note this is in milliseconds still

  // get current position
  actualPositionDeg = (double)encoderTicks * holesPerRev * revPerCycle / 360; // 360 degrees in revolution
  // Serial.println(actualPositionDeg);

  // calculate errors
  positionError = commandedPositionDeg - actualPositionDeg;
  positionErrorSum += positionError * dt;
  // Serial.println(positionError);

  // calculate PWM
  pwmToWritePos =  kp * positionError
                + kd * (positionError - prevPositionError) / dt
                + ki * (positionErrorSum);
  motorInputScaledPos = (double)pwmToWritePos / 255 * 100;

  // drive motor
  if (motorInputScaledPos > 100) {
    motorInputScaledPos = 100;
  } else if (motorInputScaledPos < -100) {
    motorInputScaledPos = -100;
  }
  fullControl(motorInputScaledPos);

  // update values for next timestep
  prevPositionError = positionError;
  prevTime = nowTime;
}

void dcMotor::vel(const double commandedVelocity){
  pwmToWriteVel =  kp_v * commandedVelocity;
                // + kd * (positionError - prevPositionError) / dt
                // + ki * (positionErrorSum);
  motorInputScaledVel = (double)pwmToWriteVel / 255 * 100;
    // drive motor
  if (motorInputScaledVel > 100) {
    motorInputScaledVel = 100;
  } else if (motorInputScaledVel < -100) {
    motorInputScaledVel = -100;
  }
  fullControl(motorInputScaledVel);
}