#include "declare.h"


// declaring various global variables

int encoder_flag	= 0;		  // set when encoder calculation done
int magnet_flag 	= 0;		  // set when magnet calculation done
double dT_PD    	= 0.001;        // sampling time for PD loop in seconds ( > 0.0001)
double dT_XD    	= 0.001;         // sampling time for xd loop in seconds (>0.0001)
double dT_PO    	= 0.01;          // sampling time for power sampling loop in seconds (>0.01)  
int sample_encoder 	= 0;		  // flag for sampling encoder   (use these to check if the respective calculation is done)
int sample_magnet  	= 0;		  // flag for sampling magnetic sensor
int sample_energy  	= 0;		  // flag for sampling power
float x           	= 0.0;		  // position   of slider in no. of rotations
float xf 		   	= 0.0;		  // filtered value of position in no. of rotations
float xd           	= 0.0;		  // desired value of position  in no. of rotations
float dx         	= 0.0;		  // motor speed
float I_ref      	= 0.0;		  // reference current (control action)
float kp         	= 0.0;		  // kp for PD loop
float kd            = 0.0;		  // kd for PD loop
float I_range    	= 2.0;		  // for range -x to +x put I_range = x // for calculating PWM value
float power 		= 0.0;		  // instantaneous power measured by IC
float energy		= 0.0;

double freq_diff	= 200.0;
float freq_filt     = 200.0;
int start 			= 0;
