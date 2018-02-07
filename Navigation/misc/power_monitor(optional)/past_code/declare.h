#ifndef DEC_H
#define DEC_H


#include<math.h>
#include<stdio.h>
#include<bcm2835.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>


// define various pins for testing

#define PWM_PIN RPI_BPLUS_GPIO_J8_12   //pwm
#define ENC_PIN RPI_BPLUS_GPIO_J8_38
#define MAG_PIN RPI_BPLUS_GPIO_J8_40

#define PWM_CHANNEL 0
#define RANGE 1024		 // pwm range = (0-1024)
#define BILLION 1E9
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

// defining pins for quadrature decoder (encoder counter)

#define OE_COUNT RPI_BPLUS_GPIO_J8_13  // output enable for encoder counter
#define SEL1 RPI_BPLUS_GPIO_J8_16      // sel 1 for encoder counter
#define SEL2 RPI_BPLUS_GPIO_J8_18      // sel 2 for encoder counter
#define D0 RPI_BPLUS_GPIO_J8_22        // 8 data pins
#define D1 RPI_BPLUS_GPIO_J8_33
#define D2 RPI_BPLUS_GPIO_J8_32
#define D3 RPI_BPLUS_GPIO_J8_31
#define D4 RPI_BPLUS_GPIO_J8_29
#define D5 RPI_BPLUS_GPIO_J8_26
#define D6 RPI_BPLUS_GPIO_J8_24
#define D7 RPI_BPLUS_GPIO_J8_35

#define PA0 RPI_BPLUS_GPIO_J8_15

#define OE_SHIFTER RPI_BPLUS_GPIO_J8_07
#define MOTOR_D3 RPI_BPLUS_GPIO_J8_08
#define RST_COUNT RPI_BPLUS_GPIO_J8_11


extern int encoder_flag	  ;		  // set when encoder calculation done
extern int magnet_flag 	  ;		  // set when magnet calculation done
extern double dT_PD       ;        // sampling time for PD loop in seconds
extern double dT_XD    	  ;        // sampling time for xd loop in seconds
extern double dT_PO    	  ;		   // sampling time for power loop in seconds
extern int start		  ;		  // flag for starting the motor and other threads
extern int sample_encoder ;		  // flag for sampling encoder
extern int sample_magnet  ;		  // flag for sampling magnetic sensor
extern int sample_energy  ;		  // flag for sampling power
extern float x            ;		  // position
extern float xf 		  ;		  // filtered value of position
extern float xd           ;		  // desired value of position
extern float dx           ;		  // motor speed
extern float I_ref        ;		  // reference current (control action)
extern float kp           ;		  // kp for PD loop
extern float kd           ;		  // kd for PD loop
extern float I_range      ;		  // for calculating PWM value
extern float power 		  ;		  // instantaneous power measured by IC
extern float energy		  ;
extern double freq_diff   ;		  // differentiator cut-off frequency
extern float freq_filt	  ;

#endif

