#include "declare.h"
#include "magnet_thread.h"

void magnet_thread(void)
{
	printf("Magnet thread started.\n");

	bcm2835_spi_begin();	
	bcm2835_gpio_fsel(D6, BCM2835_GPIO_FSEL_INPT);   // ATTENTION: If there's a problem in reading encoder data (the D6 and/or D5 Pins), this is the issue. Can be solved by changing the library.
	bcm2835_gpio_fsel(D5, BCM2835_GPIO_FSEL_INPT);
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);  // 32 = 7.8125 MHz, 128 = 1.95 Mhz
 	//bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    //bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

/*	// the sensor can be configured without the eeprom by writing data into it's registers using the following code
	char write_buf[] = {0xD2, 0x0B, 0x00};   //  Write data: first byte is op code (D2 for Register write), second is address third is data
	bcm2835_gpio_write(PA0, LOW);
	bcm2835_spi_transfern(write_buf, sizeof(write_buf));
	bcm2835_gpio_write(PA0, HIGH);
	
	// repeat the above code for settting up values of all the registers
*/	
	while(start == 0)
	{}

	bcm2835_gpio_fsel(D6, BCM2835_GPIO_FSEL_INPT);   // ATTENTION: If there's a problem in reading encoder data (the D6 and/or D5 Pins), this is the issue. Can be solved by changing the library.
	bcm2835_gpio_fsel(D5, BCM2835_GPIO_FSEL_INPT);
    
	while(1)
	{
		if(sample_magnet)
	    {
		
			//code for obtaining value from iC-MU
			
			char mag_buf[] = {0xF5, 0x00};   //  Data to send: first byte is op code, rest depends on the opcode
			char mag_in[]  = {0x00, 0x00};
			
			bcm2835_gpio_write(PA0, LOW);
			bcm2835_spi_transfernb(mag_buf, mag_in, sizeof(mag_in));
			bcm2835_gpio_write(PA0, HIGH);
			
//			printf("OUT at %X is %X\n", mag_buf[0], mag_buf[1]);
//			printf("IN at %X is %X\n", mag_in[0], mag_in[1]);

			if(mag_in[1] == 0x80)
				printf("");
			else
				printf("DATA NOT VALID\n");
			
			char status[] = {0xA6};
			char status_in[] = {0x00, 0x00, 0x00};
			
			bcm2835_gpio_write(PA0, LOW);
			bcm2835_spi_transfernb(status, status_in, sizeof(status_in));
			bcm2835_gpio_write(PA0, HIGH);
			
//			printf("OUT at %X is %X\n", status[0], status[1]);
//			printf("status is %X and data is %X, %X, %X, %X, %X, %X\n", status_in[0], status_in[1], status_in[2], status_in[3], status_in[4], status_in[5], status_in[6]);
			
			
			float mag_reading = (256.0 * status_in[1]) + status_in[2];



			// code for calculating xd

			float mag_position = (360.0 * mag_reading) / 65536.0;
			
//			printf("mag: Reading: %f,	angle: %f,	read: %X, %X\n", mag_reading, mag_position, status_in[1], status_in[2]);
			
//			xd = pow(sin(mag_reading),-0.5);

			xd = mag_position / 2.0;
//			xd = 100.0;
			//bcm2835_gpio_write(MAG_PIN, LOW);

			
			// reset sampling flag
			sample_magnet = 0;
			
			//set magnet flag high
			magnet_flag = 1;
	  	}
	}
	//			bcm2835_spi_end();
}
