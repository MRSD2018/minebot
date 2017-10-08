#include "Arduino.h"
#include "dcMotor.h"
#include "digitalWriteFast.h" //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

dcMotor::dcMotor(int PWM, int L1, int L2, int chanA, int chanB)
{
	pinMode(PWM, OUTPUT);

	_PWM = PWM;
	_L1 = L1;
	_L2 = L2;
	_chanA = chanA;
	_chanB = chanB;
}

void dcMotor::cw()
{

}

void dcMotor::ccw()
{

}

void dcMotor::speed()
{
	
}

void dcMotor::handleChanA()
{

}