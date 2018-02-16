#include "Arduino.h"
#include "dcMotor.h"
// #include "digitalWriteFast.h" //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

dcMotor::dcMotor(int PWM, int L1, int L2)
// dcMotor::dcMotor(int PWM, int L1, int L2, int chanA, int chanB)
{
	pinMode(PWM, OUTPUT);
	pinMode(L1,OUTPUT);
	pinMode(L2,OUTPUT);

	_PWM = PWM;
	_L1 = L1;
	_L2 = L2;
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