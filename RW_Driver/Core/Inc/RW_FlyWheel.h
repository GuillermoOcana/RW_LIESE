/**
  * @file           : RW_FlyWheel.h
  * @author			: Jesús Yamil Robles López
  * @author			: Guillermo Ocaña Anaya
  * @date			: 17/12/2021
  * @brief          : Header file for RW FlyWheel
  *
  */
#ifndef INC_RW_FLYWHEEL_H_
#define INC_RW_FLYWHEEL_H_

#include "stm32f4xx_hal.h"

/**
 * @brief High_Output_Drive Structure definition
 */
typedef struct{

	GPIO_TypeDef* Port_Output_A;	/**< Indicates to Port for Output A */
	uint16_t PIN_Output_A;			/**< Indicates the PIN corresponding for the Port in the Output A */

	GPIO_TypeDef* Port_Output_B;	/**< Indicates to Port for Output B */
	uint16_t PIN_Output_B;			/**< Indicates the PIN corresponding for the Port in the Output B */

	GPIO_TypeDef* Port_Output_C;	/**< Indicates to Port for Output C */
	uint16_t PIN_Output_C;			/**< Indicates the PIN corresponding for the Port in the Output C */

}High_Output_Drive_Type;

/**
 * @brief FlyWheel Structure definition
 */
typedef struct{

	TIM_HandleTypeDef PWM_Generator;		/**< Indicates the Timer that will generate PWM on the Wheel */

	uint16_t DutyCicle;						/**< Set the Duty Cycle for the PWM */

	High_Output_Drive_Type* HIGH_DRIVE;		/**< Contains the Ports that will be used for the High Output Driver */
	TIM_TypeDef* LOW_DRIVE;					/**< Contains the Ports that will be used for the PWM Output Driver*/

	uint8_t PowerMode;						/**< Contains information about the wHEEL Status (On/Off) */
	uint8_t Wheel_Id;						/**< Contains the ID of the Wheel*/

}FlyWheel_Type;

void Configure_High_Output(High_Output_Drive_Type* Wheel_HD, GPIO_TypeDef* OutputA, uint16_t Pin_A, GPIO_TypeDef* OutputB, uint16_t Pin_B, GPIO_TypeDef* OutputC, uint16_t Pin_C);
void FlyWheel_Init(FlyWheel_Type* FlyWheel, High_Output_Drive_Type* FlyWheel_HD, TIM_TypeDef* FlyWheel_LD, uint16_t FlyWheel_DC, TIM_HandleTypeDef FlyWheel_PWM, uint8_t FlyWheel_Id);
void FlyWheel_StartPWM(FlyWheel_Type* FlyWheel);
void FlyWheel_StopPWM(FlyWheel_Type* FlyWheel);
void FlyWheel_MoveMotor(FlyWheel_Type* FlyWheel, int phase);


#endif /* INC_RW_FLYWHEEL_H_ */
