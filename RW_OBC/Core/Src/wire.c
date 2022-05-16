/*
 * wire.c
 *
 *  Created on: Mar 1, 2022
 *      Author: guill
 */


#include "wire.h"

void writereg(I2C_HandleTypeDef hi2c ,uint8_t sensor_address,uint8_t subaddress,uint8_t data){

	uint8_t msg[2];
	msg[0]=subaddress;
	msg[1]=data;
	HAL_I2C_Master_Transmit(&hi2c,sensor_address & 0XFE,msg,2,10);

}

uint8_t readreg(I2C_HandleTypeDef hi2c,uint8_t sensor_address,uint8_t subaddress){

	uint8_t data;
	HAL_I2C_Master_Transmit(&hi2c,sensor_address & 0XFE ,&subaddress,1, 10);
	HAL_I2C_Master_Receive(&hi2c,sensor_address + 0x01, &data,1, 10);
	return data;

}


