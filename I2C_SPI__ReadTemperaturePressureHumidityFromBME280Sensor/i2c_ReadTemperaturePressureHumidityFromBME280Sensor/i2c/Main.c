#include "Main.h"

// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_SLEEP      0x00
#define MODE_FORCED     0x01
#define MODE_NORMAL     0x03

// Standby Time
#define T_SB_0p5    	0x00
#define T_SB_62p5   	0x01
#define T_SB_125    	0x02
#define T_SB_250    	0x03
#define T_SB_500    	0x04
#define T_SB_1000   	0x05
#define T_SB_10     	0x06
#define T_SB_20     	0x07

// IIR Filter Coefficients
#define IIR_OFF     	0x00
#define IIR_2       	0x01
#define IIR_4       	0x02
#define IIR_8       	0x03
#define IIR_16      	0x04


// REGISTERS DEFINITIONS
#define ID_REG      	0xD0
#define RESET_REG  		0xE0
#define CTRL_HUM_REG    0xF2
#define STATUS_REG      0xF3
#define CTRL_MEAS_REG   0xF4
#define CONFIG_REG      0xF5
#define PRESS_MSB_REG   0xF7


//Copied from data sheet


#define dev_address 0x76



// Read the Trimming parameters saved in the NVM ROM of the device

void I2CMem_read(uint8_t slave_address,uint8_t reg_address,uint8_t* trimdata,uint8_t size)
{
	I2C_Start();
	slave_address = (slave_address << 1);
	I2C_Address(slave_address);
	I2C_Write(reg_address);
	I2C_Start();
	slave_address |= 1;
	I2C_Read(slave_address,trimdata,size);
	
	
	
}

void TrimRead(void)
{
	uint8_t trimdata[32];
	//0x88 because in datasheet, memory value starts from here
	I2CMem_read(dev_address,0x88,trimdata,25);//0xA1- 0x88 contains 25 values
	Delay_ms(100);
	//Reading from non-volatile memory
	
	//Reading remaining values
	I2CMem_read(dev_address,0xE1,(uint8_t *)trimdata+25,7);
	Delay_ms(100);
	
	// Read NVM from 0xE1 to 0xE7

	// Arrange the data as per the datasheet (page no. 24)
	dig_T1 = (trimdata[1]<<8) | trimdata[0];
	dig_T2 = (trimdata[3]<<8) | trimdata[2];
	dig_T3 = (trimdata[5]<<8) | trimdata[4];
	dig_P1 = (trimdata[7]<<8) | trimdata[5];
	dig_P2 = (trimdata[9]<<8) | trimdata[6];
	dig_P3 = (trimdata[11]<<8) | trimdata[10];
	dig_P4 = (trimdata[13]<<8) | trimdata[12];
	dig_P5 = (trimdata[15]<<8) | trimdata[14];
	dig_P6 = (trimdata[17]<<8) | trimdata[16];
	dig_P7 = (trimdata[19]<<8) | trimdata[18];
	dig_P8 = (trimdata[21]<<8) | trimdata[20];
	dig_P9 = (trimdata[23]<<8) | trimdata[22];
	dig_H1 = trimdata[24];
	dig_H2 = (trimdata[26]<<8) | trimdata[25];
	dig_H3 = (trimdata[27]);
	dig_H4 = (trimdata[28]<<4) | (trimdata[29] & 0x0f);
	dig_H5 = (trimdata[30]<<4) | (trimdata[29]>>4);
	dig_H6 = (trimdata[31]);
}



void I2CMem_write(uint8_t slave_address,uint8_t reg_address,uint8_t data)
{
	I2C_Start();
	slave_address = slave_address << 1;
	I2C_Address(slave_address);
	I2C_Write(reg_address);
	I2C_Write(data);
	
}
// COPIED FROM BME280 DATASHEET..........

void BME280Config(uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h, uint8_t mode, uint8_t t_sb, uint8_t filter)
{
	//Read important values for calculating, from the NVM
	TrimRead();
	uint8_t datatowrite = 0;
	uint8_t datacheck = 0;

	// Reset the device
	datatowrite = 0xB6; //0xB6 forcefully resets the device, anyother value has no effect (Reference : BME280 Datasheet)
	I2CMem_write(dev_address,RESET_REG,datatowrite);
	Delay_ms(100);
	//This sets oversampling of humidity to x1 as per datasheet
	datatowrite = osrs_h;
	I2CMem_write(dev_address,CTRL_HUM_REG,datatowrite);
	Delay_ms(100);
	
	//Sets standby time as 0.5 ms
	//Filter coefficient equals 16
	datatowrite = (t_sb <<5) |(filter << 2);
	I2CMem_write(dev_address,CONFIG_REG,datatowrite);
	Delay_ms(100);
	
	//Sets oversampling to two, for temperature
	//Sets oversampling to sixteen , for pressure
	
	datatowrite = (osrs_t <<5) |(osrs_p << 2) | mode;
	I2CMem_write(dev_address,CTRL_MEAS_REG,datatowrite);
	Delay_ms(100);
	
}


void BMEReadRaw(void)
{
	uint8_t RawData[8];

	// Check the chip ID before reading
	//HAL_I2C_Mem_Read(&hi2c1, BME280_ADDRESS, ID_REG, 1, &chipID, 1, 1000);

	
	
		// Read the Registers 0xF7 to 0xFE
	I2CMem_read(dev_address,PRESS_MSB_REG,RawData,8);
		/* Calculate the Raw data for the parameters
		 * Here the Pressure and Temperature are in 20 bit format and humidity in 16 bit format
		 */
	pRaw = (RawData[0]<<12)|(RawData[1]<<4)|(RawData[2]>>4);
	tRaw = (RawData[3]<<12)|(RawData[4]<<4)|(RawData[5]>>4);
	hRaw = (RawData[6]<<8)|(RawData[7]);
}



int32_t t_fine;
int32_t BME280_compensate_T_int32(int32_t adc_T)
{
	int32_t var1, var2, T;
	var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1)))>> 12) *((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}


// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t BME280_compensate_P_int32(int32_t adc_P)
{
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
	var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
	var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
	var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) *var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
		p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
	return p;
}


uint32_t bme280_compensate_H_int32(int32_t adc_H)
{
	int32_t v_x1_u32r;
	v_x1_u32r = (t_fine - ((int32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) *\
			v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *\
					((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) +\
							((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) +\
					8192) >> 14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *\
			((int32_t)dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (uint32_t)(v_x1_u32r>>12);
}




double bme280_compensate_H_double(uint32_t adc_H)
{
	double var_H;
	var_H = (((double)t_fine) - 76800.0);
	var_H = (adc_H - (((double)dig_H4) * 64.0 + ((double)dig_H5) / 16384.0 * var_H)) * (((double)dig_H2) / 65536.0 * (1.0 + ((double)dig_H6) / 	67108864.0 * var_H *
	(1.0 + ((double)dig_H3) / 67108864.0 * var_H)));
	var_H = var_H * (1.0 - ((double)dig_H1) * var_H / 524288.0);
	if (var_H > 100.0)
	var_H = 100.0;
	else if (var_H < 0.0)
	var_H = 0.0;
	return var_H;
}






void BME280_Measure (void)
{
	BMEReadRaw();
	
		  if (tRaw == 0x800000) Temperature = 0; // value in case temp measurement was disabled
		  else
		  {
			  Temperature = (float)((BME280_compensate_T_int32 (tRaw))/100.0);  // as per datasheet, the temp is x100
		  }

		  if (pRaw == 0x800000) Pressure = 0; // value in case temp measurement was disabled
		  else
		  {
			  Pressure = (float)(BME280_compensate_P_int32 (pRaw));  // as per datasheet, the pressure is Pa
		  }

		  if (hRaw == 0x800000) Humidity = 0; // value in case temp measurement was disabled
		  else
		  {
			  Humidity = (float)((bme280_compensate_H_int32 (hRaw))/1024.0);  // as per datasheet, the temp is x1024
		  }
			//char buffer[64];
			//snprintf(buffer, sizeof buffer, "Temp: %f Press: %f Hum: %f\n", Temperature, Pressure, Humidity);
			//return buffer;

}
//void USART2_IRQHandler(void) {
//	
//}

//END
int main(){
	
	SysClockConfig();
	I2C_Config();
	TIM6Config();
	gpio_output_init();
	Uart2Config();
	gpio_usart_init();
	//	enable_interrupts();
	//NVIC_SetPriority(USART2_IRQn, 1);
	//NVIC_EnableIRQ(USART2_IRQn);
	uint8_t tempString[80];
	uint8_t presString[80];
	sprintf(tempString,"!START!");
	for(int i=0;i<6;i++)UART2_SendChar(tempString[i]);
	
	

	
	
	
	BME280Config(OSRS_2, OSRS_16, OSRS_1, MODE_NORMAL, T_SB_0p5, IIR_16);
	
	while(1){
		
		BME280_Measure();
		sprintf(tempString,"temperature is: %f.",Temperature);
		for(int i=0;i<25;i++)UART2_SendChar(tempString[i]);
		
		sprintf(presString,"pressure is: %f.",Pressure);
		for(int i=0;i<20;i++)UART2_SendChar(presString[i]);
		
		Delay_ms(100);
	}
	
}