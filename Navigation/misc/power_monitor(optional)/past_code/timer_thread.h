#ifndef TIME_THREADS_H
#define TIME_THREADS_H

#include<math.h>
#include<stdio.h>
#include<bcm2835.h>
#include<unistd.h>
#include<time.h>


// define various pins for testing

#define TIME_PIN RPI_BPLUS_GPIO_J8_11  //time flag
#define BILLION 1E9

/*
// declaring various global variables

extern double dT_PD		 ;         // sampling time for PD loop in seconds
extern double dT_XD	 	 ;         // sampling time for xd loop in seconds
extern int sample_encoder;		  // flag for sampling encoder
extern int sample_magnet ;		  // flag for sampling magnetic sensor

*/
// declare the related functions

void encoder_time_thread(void);
void magnet_time_thread(void);
void energy_time_thread(void);
void clock_delay(double interval); 

#endif

