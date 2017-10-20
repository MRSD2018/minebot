#include "declare.h" 
#include "calculate_I_ref.h"


void calculate_I_ref(void)
{	
	while(start == 0)
	{}
	
	while(1)
	{
		if(encoder_flag) // encoder flag = 1 means xf and dx are calculated
		{
			//calculate I_ref and convert to pwm value
		I_ref = (float)((kp)*(xd - xf)) - (kd)*(dx);
		//	I_range = 2;     // for range -2 to +2, set during motor controller setup
		
		if(I_ref > I_range)
			I_ref = I_range;
		if(I_ref < (-1.0*I_range))
			I_ref = (-1.0*I_range);
	
		// maxon controller requires PWM value to be 10% and 90%
		// so, I_ref should be scaled beetween 103 and 921 (10% and 90% of 1024)
	
		float pwm_value = (204.0 * I_ref) + 512.0;
		
		bcm2835_pwm_set_data(PWM_CHANNEL, pwm_value);
	
//		printf("i_ref: X, Xf and Xd = %f,  %f,  %f\n", x, xf, xd);
		//reset flag
		encoder_flag = 0;
		}
	}	

}
