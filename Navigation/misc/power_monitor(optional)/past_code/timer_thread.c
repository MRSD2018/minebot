#include "declare.h"
#include "timer_thread.h"

void encoder_time_thread(void)
{//	 printf("Time_enc\n");
	while(start == 0)
	{}
	while(1)
	{
		sample_encoder = 1;   // set sampling flag for encoder
		clock_delay(dT_PD);
//		printf("Enc time.\n");
	}
}
								

void magnet_time_thread(void)
{   while(start == 0)
	{}
	while(1)
	{
		sample_magnet = 1;   // set sampling flag for magnet
		clock_delay(dT_XD);
	}
}


void energy_time_thread(void)
{   while(start == 0)
	{}
	while(1)
	{
		sample_energy = 1;   // set sampling flag for magnet
		clock_delay(dT_PO);
	}
}

void clock_delay(double interval)
{
	//printf("clock_delay\n");
//	printf("Time_enc\n");
	struct timespec requestStart, requestEnd;
	double elapsed;

	clock_gettime(CLOCK_REALTIME, &requestStart);
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	elapsed = (double)( requestEnd.tv_sec - requestStart.tv_sec ) + (double)( requestEnd.tv_nsec - requestStart.tv_nsec )/ BILLION;

	while( elapsed  < interval ) //interval in seconds
	{
		clock_gettime(CLOCK_REALTIME, &requestEnd);
		elapsed = (double)( requestEnd.tv_sec - requestStart.tv_sec ) + (double)( requestEnd.tv_nsec - requestStart.tv_nsec )/ BILLION;
	}

}


