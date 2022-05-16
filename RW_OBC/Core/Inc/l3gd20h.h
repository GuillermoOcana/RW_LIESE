/*
 * l3gd20h.h
 *
 *  Created on: Nov 16, 2021
 *      Author: Guillermo Ocaña Anaya
 */

#ifndef INC_L3GD20H_H_
#define INC_L3GD20H_H_

#include "wire.h"
#include <math.h>

typedef struct{

	I2C_HandleTypeDef i2c;
	uint8_t ADDRESS;
	uint8_t LOWODR;
	uint8_t FR;
	uint8_t POWER_MODE;
	uint8_t DR;
	uint8_t BW;

	double  gain_fr;
	uint8_t gyro_raw[6];
	int offset[3];
	short hil_gyro[3];
	double  gyro[3];
	double  angle[3];

}L3GD20H_Type;


#define   L3GD20H_ADDRESS		   0XD6

#define   L3GD20H_BW_1			   0X00
#define   L3GD20H_BW_2			   0X10
#define   L3GD20H_BW_3			   0X20
#define   L3GD20H_BW_4			   0X30
#define   L3GD20H_DR_100	       0X00
#define   L3GD20H_DR_200		   0X40
#define   L3GD20H_DR_400		   0X80
#define   L3GD20H_DR_800		   0XC0
#define   L3GD20H_POWER_DOWN       0X00
#define   L3GD20H_NORMAL           0X0F
#define   L3GD20H_SLEEP     	   0X04

#define   L3GD20H_FR_250		   0X00
#define   L3GD20H_FR_500		   0X10
#define   L3GD20H_FR_2000		   0X20

#define   L3GD20H_LOW_ODR_D		   0X00
#define   L3GD20H_WHO_AM_I         0x0F

#define   L3GD20H_CTRL1            0x20
#define   L3GD20H_CTRL2            0x21
#define   L3GD20H_CTRL3            0x22
#define   L3GD20H_CTRL4            0x23
#define   L3GD20H_CTRL5            0x24
#define   L3GD20H_REFERENCE        0x25
#define   L3GD20H_OUT_TEMP         0x26
#define   L3GD20H_STATUS           0x27

#define   L3GD20H_OUT_X_L          0x28
#define   L3GD20H_OUT_X_H          0x29
#define   L3GD20H_OUT_Y_L          0x2A
#define   L3GD20H_OUT_Y_H          0x2B
#define   L3GD20H_OUT_Z_L          0x2C
#define   L3GD20H_OUT_Z_H          0x2D

#define   L3GD20H_FIFO_CTRL        0x2E
#define   L3GD20H_FIFO_SRC         0x2F

#define   L3GD20H_IG_CFG           0x30
#define   L3GD20H_IG_SRC           0x31
#define   L3GD20H_IG_THS_XH        0x32
#define   L3GD20H_IG_THS_XL        0x33
#define   L3GD20H_IG_THS_YH        0x34
#define   L3GD20H_IG_THS_YL        0x35
#define   L3GD20H_IG_THS_ZH        0x36
#define   L3GD20H_IG_THS_ZL        0x37
#define   L3GD20H_IG_DURATION      0x38

#define   L3GD20H_LOW_ODR          0x39


void L3GD20H_Init(L3GD20H_Type * l3g,I2C_HandleTypeDef hi2c,uint8_t L3GD20H_DR,uint8_t L3GD20H_FR, uint8_t L3GD20H_BW, uint8_t L3GD20H_POWER_MODE,uint8_t L3GD20H_LOWODR);

void L3GD20H_Read(L3GD20H_Type * l3g,float dt);

void L3GD20H_Calibration(L3GD20H_Type * l3g,TIM_HandleTypeDef htim1);





#endif /* INC_L3GD20H_H_ */
