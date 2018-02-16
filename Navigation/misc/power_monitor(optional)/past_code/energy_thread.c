#include "declare.h"
#include "energy_thread.h"

void calculate_energy(void)
{
	uint8_t send;
	uint8_t data;
	
	char volt[2] = {0x00, 0x00};
	char curr[2] = {0x00, 0x01};
	char pow[2] = {0x00, 0x00};
	char bus_volt[2] = {0x00, 0x00};
	
	uint8_t write_address = 0x40; // find using i2cdetect -y 1
	
	char config_write[3] = {0x00, 0x39, 0x9F};  // first byte is address, next two are data
	char calib_write[3] = {0x05, 0x10, 0x00};   // for value 0x1000 the current LSB = 1 e-3 A. (see datasheet on how to determine calibration register value
	
	char voltage_addr[1] = {0x01};		 // voltage register address
	char current_addr[1] = {0x04};
	char power_addr[1]	 = {0x03};
	char bus_addr[1]	 = {0x02};
	
	int volt_read = 0;
	float voltage  = 0;
	float current = 1.0;
	float power_read = 0;
	float bus_voltage = 0;
	
	printf("Energy thread started.\n");
	
	while(start == 0)
	{}
	
	FILE *fp = fopen( "current_file.txt", "w" ); // Open file for writing
	
	float su2 = 0.0;

	send = bcm2835_i2c_write(config_write, 3);
	send = bcm2835_i2c_write(calib_write, 3);

	while(1)
	{
		if(sample_energy)
		{
			float t = 0.0;
			bcm2835_i2c_begin();  // I2C begin
			bcm2835_i2c_set_baudrate(100000);	

    		bcm2835_i2c_setSlaveAddress(write_address);		 //write
    		bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
	
//			send = bcm2835_i2c_read_register_rs(voltage_addr, volt, 2);
			send = bcm2835_i2c_read_register_rs(current_addr, curr, 2);
			send = bcm2835_i2c_read_register_rs(bus_addr, bus_volt, 2);
			send = bcm2835_i2c_read_register_rs(power_addr, pow, 2);
/*											  
			
			int volt_16 = (volt[0]<<8)|(volt[1]);
			if(volt[0] > 127)		   // if sign bit is 1
			   volt_read = volt_16 - 0x10000;  //(~volt_16 & 0x0000FFFF) + 0x01 ;
			else
			   volt_read = (volt_16);
			
			//default value of voltage resolution is 320 mV.
			//So, we will have to convert the voltage reading to actual voltage value.
						
			voltage =  (float)volt_read / 100000.0;  // in V  // this conversion is given in datasheet
*/		
			// bus voltage, resolution is always 4 mV
			int bus_16 = (bus_volt[0]<<8)|(bus_volt[1]);
			bus_16 = bus_16>>3;
			bus_voltage = (float)bus_16 * 0.004;  // in Volts
			
			
			int curr_16 = (curr[0]<<8)|(curr[1]);
		
			if(curr[0] > 127)	  // if sign bit is 1
				current = (float)(curr_16 - 0x10000) / 1000.0;  // in A (because LSB is 0.5 mA)
			else
				current = (float)curr_16 / 1000.0;   // in A
			
			
			// power LSB = 20 * current LSB = 20 mW. Therefore, power = power read x 20 (mW)
			int pow_16 = (pow[0]<<8)|(pow[1]);
			power = ((float)pow_16 * 20.0 ) / 1000.0;  // (in W)
			if(current < 0)
				power = power * -1.0 ;
			
//			discrete_intg();  // update value of energy
			
			//calculate power
			float power_cal = bus_voltage * current;
		
			printf("pow: V, I, P_cal, P_meas = %f, %f, %f, %f \n", bus_voltage, current, power_cal, power);


			fprintf(fp, "%f\t%f\n", current, t);
			su2++;
		
			bcm2835_i2c_end(); // I2C end
			sample_energy = 0;
			
		}
	}
		fclose(fp);
}
