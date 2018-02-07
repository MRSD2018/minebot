#include "declare.h"
#include "encoder_thread.h"

void encoder_thread(void)
{
	bcm2835_gpio_fsel(ENC_PIN, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(RST_COUNT, BCM2835_GPIO_FSEL_OUTP); // reset count
	bcm2835_gpio_write(RST_COUNT, LOW);
	
	// setting modes of counter pins

	bcm2835_gpio_fsel(OE_COUNT, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_set_pud(OE_COUNT, BCM2835_GPIO_PUD_UP); //pull-up for eoutput enable
	bcm2835_gpio_fsel(SEL1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(SEL2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(D0, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D1, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D2, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D3, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D4, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D5, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D6, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(D7, BCM2835_GPIO_FSEL_INPT);
	
	bcm2835_gpio_set_pud(D0, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D1, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D2, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D3, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D4, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D5, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D6, BCM2835_GPIO_PUD_DOWN);
	bcm2835_gpio_set_pud(D7, BCM2835_GPIO_PUD_DOWN);
	
	
	printf("Encoder Thread started.\n");
	
	while(start == 0)
	{}
	
	bcm2835_gpio_write(RST_COUNT, HIGH); // now start counting
	
	while(1)
	{
		if(sample_encoder)
		{
			//bcm2835_gpio_write(ENC_PIN, HIGH);		  // for the oscilloscope
			
			// code for obtaining value from encoder IC
			x = calculate_encoder();  // no. of rotations

//			x = (x / 4.81) * 0.002;	// distance traveled by slider in metres

			// code for calculating xf and dx
			
//			printf("enc_th: freq are : %f,  %f\n", freq_diff, freq_filt);
			discrete_diff();		// updating value of dx by calling practical differentiator
			low_pass_filter();		// update xf, the filtered value of x

//			printf("enc_th: DX in encoder_thread = %f\n", dx);
//			printf("enc_th: xf in encoder_thread = %f\n", xf);
	//		bcm2835_gpio_write(ENC_PIN, LOW);

			// reset time flag
			sample_encoder = 0;  //reset sampling flag

			//set encoder flag high
		    encoder_flag = 1;   // means encoder calculations done
	  	}
	}

}



float calculate_encoder(void)
{
	int encoder_array[32];

	// setting OE for counter
	bcm2835_gpio_write(OE_COUNT, LOW);

	// reading MSB (24-31)

	bcm2835_gpio_write(SEL1, LOW);
	bcm2835_gpio_write(SEL2, HIGH);

	encoder_array[24] = bcm2835_gpio_lev(D0);
	encoder_array[25] = bcm2835_gpio_lev(D1);
	encoder_array[26] = bcm2835_gpio_lev(D2);
	encoder_array[27] = bcm2835_gpio_lev(D3);
	encoder_array[28] = bcm2835_gpio_lev(D4);
	encoder_array[29] = bcm2835_gpio_lev(D5);
	encoder_array[30] = bcm2835_gpio_lev(D6);
	encoder_array[31] = bcm2835_gpio_lev(D7);


	// reading 3rd Byte (16-23)

	bcm2835_gpio_write(SEL1, HIGH);
	bcm2835_gpio_write(SEL2, HIGH);

	encoder_array[16] = bcm2835_gpio_lev(D0);
	encoder_array[17] = bcm2835_gpio_lev(D1);
	encoder_array[18] = bcm2835_gpio_lev(D2);
	encoder_array[19] = bcm2835_gpio_lev(D3);
	encoder_array[20] = bcm2835_gpio_lev(D4);
	encoder_array[21] = bcm2835_gpio_lev(D5);
	encoder_array[22] = bcm2835_gpio_lev(D6);
	encoder_array[23] = bcm2835_gpio_lev(D7);

	
	// reading 2nd byte (8-15)

	bcm2835_gpio_write(SEL1, LOW);
	bcm2835_gpio_write(SEL2, LOW);

	encoder_array[8]  = bcm2835_gpio_lev(D0);
	encoder_array[9]  = bcm2835_gpio_lev(D1);
	encoder_array[10] = bcm2835_gpio_lev(D2);
	encoder_array[11] = bcm2835_gpio_lev(D3);
	encoder_array[12] = bcm2835_gpio_lev(D4);
	encoder_array[13] = bcm2835_gpio_lev(D5);
	encoder_array[14] = bcm2835_gpio_lev(D6);
	encoder_array[15] = bcm2835_gpio_lev(D7);
	
	
	// reading LSB (0-7)

	bcm2835_gpio_write(SEL1, HIGH);
	bcm2835_gpio_write(SEL2, LOW);

	encoder_array[0] = bcm2835_gpio_lev(D0);
	encoder_array[1] = bcm2835_gpio_lev(D1);
	encoder_array[2] = bcm2835_gpio_lev(D2);
	encoder_array[3] = bcm2835_gpio_lev(D3);
	encoder_array[4] = bcm2835_gpio_lev(D4);
	encoder_array[5] = bcm2835_gpio_lev(D5);
	encoder_array[6] = bcm2835_gpio_lev(D6);
	encoder_array[7] = bcm2835_gpio_lev(D7);

	// reset OE value
	bcm2835_gpio_write(OE_COUNT, HIGH);

	// convert data to decimal

	int count = 0;
	
	int i;
	for (i = 0; i < 32; i++)
	{
//		printf("for loop entered, Di = %d \n",encoder_array[i]);
		count = count + (encoder_array[i] * pow(2,i));
		//printf("Bit %d is  = %d \n",i,encoder_array[i]);
		
	}

	if(count > 1073741824)   // because when motor moves in opposite direction then count becomes 2^31.
		count = 0; // -1 * count;

	float rotation = (float)count / 4096.0;

//	printf("Decimal value of x = %d \n",count);
//	printf("Rotations = %f \n",rotation);

	return rotation;
}
