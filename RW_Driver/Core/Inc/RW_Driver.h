/**
  * @file           : RW_Driver.h
  * @author			: Jesús Yamil Robles López
  * @author			: Guillermo Ocaña Anaya
  * @date			: 10/12/2021
  * @brief          : Header file for RW Driver
  *
  */

#ifndef INC_RW_DRIVER_H_
#define INC_RW_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "RW_FlyWheel.h"

/**
 * @brief RW_Driver Structure definition
 */
typedef struct{
	FlyWheel_Type * FlyWheel0; 	/**< Contains information about the Wheel 0 */
	FlyWheel_Type * FlyWheel1; 	/**< Contains information about the Wheel 1 */
	FlyWheel_Type * FlyWheel2; 	/**< Contains information about the Wheel 2 */
	FlyWheel_Type * FlyWheel3; 	/**< Contains information about the Wheel 3 */

	uint8_t DriverStatus;		/**< Contains information about the Driver Status (On/Off) */

}RW_Driver_Type;

void RW_Driver_Init(RW_Driver_Type* RW_Driver, FlyWheel_Type* FlyWheel0, FlyWheel_Type* FlyWheel1, FlyWheel_Type* FlyWheel2, FlyWheel_Type * FlyWheel3);
uint8_t RW_Driver_ReadSolicitude(RW_Driver_Type* RW_Driver, uint8_t RxData[]);
void RW_Driver_WriteSolicitude(RW_Driver_Type* RW_Driver, uint8_t RxData[]);
void RW_Diver_Start(RW_Driver_Type* RW_Driver);
void RW_Diver_Stop(RW_Driver_Type* RW_Driver);
void RW_Driver_SetPowerMode(RW_Driver_Type* RW_Driver, FlyWheel_Type* FlyWheel, uint8_t Data);

#endif /* INC_RW_DRIVER_H_ */
