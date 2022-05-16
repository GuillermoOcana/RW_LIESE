


#ifndef INC_WIRE_H_
#define INC_WIRE_H_

#include "stm32f4xx_hal.h"


void writereg(I2C_HandleTypeDef hi2c,uint8_t sensor_address,uint8_t subaddress,uint8_t data);
uint8_t readreg(I2C_HandleTypeDef hi2c,uint8_t sensor_address,uint8_t subaddress);


#endif /* INC_WIRE_H_ */
