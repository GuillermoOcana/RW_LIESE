/*
 * hil.h
 *
 *  Created on: Nov 16, 2021
 *      Author: guill
 */

#ifndef INC_HIL_H_
#define INC_HIL_H_

//#include "usb_device.h"
#include "stm32f4xx_hal.h"
#include "l3gd20h.h"

#define HIL_GYRO_ANGLE_X		0X01
#define HIL_GYRO_ANGLE_Y		0X02
#define HIL_GYRO_ANGLE_Z		0X03
#define HIL_FLAG	    		0XFE


void HIL_Transmit(uint8_t HIL_Type, uint16_t HIL_Data);
void HIL_Transmit_Gyro(L3GD20H_Type * l3g);


#endif /* INC_HIL_H_ */
