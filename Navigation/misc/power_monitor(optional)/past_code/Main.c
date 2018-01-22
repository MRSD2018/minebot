// including the required functions' header files

#include "declare.h"
#include "timer_thread.h"
#include "encoder_thread.h" 
#include "magnet_thread.h"
#include "filters.h" 
#include "energy_thread.h" 
#include "calculate_I_ref.h" 


int comparse(int argc, char *argv[]) 
{
    if (argc < 3) 
	{  // must have two arguments: kp and kd
        fprintf(stderr, "\nInsufficient command line arguments, try again\n");
        return EXIT_FAILURE;
	}	
	
	kp = atof(argv[1]);
	kd = atof(argv[2]);
	
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) 
{
	bcm2835_init();
	 
	if(comparse(argc, argv) == EXIT_FAILURE) return 0;
	 
	// setting PWM_PIN as pwm from channel 0 in markspace mode with range = RANGE
	bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);  //ALT5 is pwm mode
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16); // pwm freq = 19.2 / 16 MHz
	bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);		     // markspace mode
	bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
	
	
	bcm2835_gpio_fsel(OE_SHIFTER, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_set_pud(OE_SHIFTER, BCM2835_GPIO_PUD_DOWN); //pull-down for output enable of logic shifters

	bcm2835_gpio_fsel(MOTOR_D3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_set_pud(MOTOR_D3, BCM2835_GPIO_PUD_DOWN); //pull-down for motor enable
	
	bcm2835_gpio_fsel(PA0, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_set_pud(PA0, BCM2835_GPIO_PUD_UP);
	bcm2835_gpio_write(PA0, HIGH);
	
	bcm2835_gpio_write(OE_SHIFTER, HIGH);
	bcm2835_gpio_write(MOTOR_D3, LOW);
	
	// creating and running threads
	pthread_t th1, th2, th3, th4, th5, th6, th7;
	pthread_create(&th1, NULL, (void*)encoder_time_thread, NULL);
	pthread_create(&th2, NULL, (void*)magnet_time_thread, NULL);
	pthread_create(&th3, NULL, (void*)encoder_thread, NULL);
	pthread_create(&th4, NULL, (void*)magnet_thread, NULL);
	pthread_create(&th5, NULL, (void*)energy_time_thread, NULL);
	pthread_create(&th6, NULL, (void*)calculate_energy, NULL);
	pthread_create(&th7, NULL, (void*)calculate_I_ref, NULL);
	
	bcm2835_delay(100); // delay to make sure that all threads are initialised and iC-MU is conofigured
	printf("\nPress enter to start the motor.");
	getchar(); 
	bcm2835_gpio_write(MOTOR_D3, HIGH);
	
	start = 1;
	printf("Started.\n");

	printf("\nPress enter to stop the motor.\n");
	getchar();
	bcm2835_gpio_write(MOTOR_D3, LOW);
	
	bcm2835_spi_end();
	bcm2835_close();
	
	
	return 0;
}
