#include "RccConfig.h"
#include "Delay.h"
#include "I2C.h"
#include "Uart.h"
#include "Gpio.h"
#include "string.h"
#include <stdio.h>
#include <math.h>


uint8_t chipID;

uint8_t TrimParam[36];
int32_t tRaw, pRaw, hRaw;

uint16_t dig_T1,dig_P1,dig_H1, dig_H3;

int16_t  dig_T2, dig_T3,dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, dig_H2,  dig_H4, dig_H5, dig_H6;

float Temperature, Pressure, Humidity;


void I2CMem_read(uint8_t slave_address,uint8_t reg_address,uint8_t* trimdata,uint8_t size);

void TrimRead(void);

void I2CMem_write(uint8_t slave_address,uint8_t reg_address,uint8_t data);

void BME280Config(uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h, uint8_t mode, uint8_t t_sb, uint8_t filter);

void BMEReadRaw(void);

uint32_t BME280_compensate_P_int32(int32_t adc_P);

uint32_t bme280_compensate_H_int32(int32_t adc_H);

int32_t BME280_compensate_T_int32(int32_t adc_T);

double bme280_compensate_H_double(uint32_t adc_H);

void BME280_Measure (void);