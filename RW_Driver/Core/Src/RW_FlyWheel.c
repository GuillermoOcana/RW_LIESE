/**
  * @file           : RW_FlyWheel.c
  * @author			: Jesús Yamil Robles López
  * @author			: Guillermo Ocaña Anaya
  * @date			: 17/12/2021
  * @brief          : Functions for RW FlyWheel
  *
  */

#include "RW_FlyWheel.h"

/**
  * @brief Configures the Ports that will be used for the High Outputs
  * @param Wheel_HD Structure that contains the Ports and Pins used
  * @param OutputA Indicates to Port for Output A
  * @param Pin_A Indicates the PIN corresponding for the Port in the Output A
  * @param OutputB Indicates to Port for Output B
  * @param Pin_B Indicates the PIN corresponding for the Port in the Output B
  * @param OutputC Indicates to Port for Output C
  * @param Pin_C Indicates the PIN corresponding for the Port in the Output C
  * @retval None
  */
void Configure_High_Output(High_Output_Drive_Type* Wheel_HD, GPIO_TypeDef* OutputA, uint16_t Pin_A, GPIO_TypeDef* OutputB, uint16_t Pin_B, GPIO_TypeDef* OutputC, uint16_t Pin_C){
	Wheel_HD->Port_Output_A= OutputA;
	Wheel_HD->Port_Output_B= OutputB;
	Wheel_HD->Port_Output_C= OutputC;

	Wheel_HD->PIN_Output_A= Pin_A;
	Wheel_HD->PIN_Output_B= Pin_B;
	Wheel_HD->PIN_Output_C= Pin_C;
}

/**
  * @brief FlyWheel Initialization Function
  * @param FlyWheel Contains all the information about the Wheel
  * @param FlyWheel_HD Contains the Ports that will be used for the High Output Driver
  * @param FlyWheel_LD Contains the Ports that will be used for the PWM Output Driver
  * @param FlyWheel_DC Set the Duty Cycle for the PWM
  * @param FlyWheel_PWM Indicates the Timer that will generate PWM
  * @param FlyWheel_Id Set the ID
  * @retval None
  */
void FlyWheel_Init(FlyWheel_Type* FlyWheel, High_Output_Drive_Type* FlyWheel_HD, TIM_TypeDef* FlyWheel_LD, uint16_t FlyWheel_DC, TIM_HandleTypeDef FlyWheel_PWM, uint8_t FlyWheel_Id){
	FlyWheel->PWM_Generator= FlyWheel_PWM;
	FlyWheel->DutyCicle= FlyWheel_DC;
	FlyWheel->HIGH_DRIVE= FlyWheel_HD;
	FlyWheel->LOW_DRIVE= FlyWheel_LD;
	FlyWheel->Wheel_Id= FlyWheel_Id;

	FlyWheel->PowerMode=0;
}

/**
 * @brief The indicated FlyWheel starts working
 * @param FlyWheel Structure that contains the information of the wheel
 * @retval None
 */
void FlyWheel_StartPWM(FlyWheel_Type* FlyWheel){

	HAL_TIM_PWM_Start(&(FlyWheel->PWM_Generator), TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&(FlyWheel->PWM_Generator), TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&(FlyWheel->PWM_Generator), TIM_CHANNEL_3);

	FlyWheel->PowerMode=1;
}

/**
 * @brief The indicated FlyWheel stops working
 * @param FlyWheel Structure that contains the information of the wheel
 * @retval None
 */
void FlyWheel_StopPWM(FlyWheel_Type* FlyWheel){

	HAL_TIM_PWM_Stop(&(FlyWheel->PWM_Generator), TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&(FlyWheel->PWM_Generator), TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&(FlyWheel->PWM_Generator), TIM_CHANNEL_3);

	FlyWheel->PowerMode=0;
}

/**
 * @brief Modifies driver outputs according to motor position
 * @param FlyWheel Structure that contains the information of the wheel
 * @param phase Motor position from Hall Sensors
 * @retval None
 */
void FlyWheel_MoveMotor(FlyWheel_Type* FlyWheel, int phase){
	switch(phase){
		case 1:
			  HAL_GPIO_WritePin(FlyWheel->HIGH_DRIVE->Port_Output_C, FlyWheel->HIGH_DRIVE->PIN_Output_C, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_SET);

			  (FlyWheel->LOW_DRIVE)->CCR1=100;
			  (FlyWheel->LOW_DRIVE)->CCR2=0;
			  (FlyWheel->LOW_DRIVE)->CCR3=(FlyWheel->DutyCicle);
			break;

		case 2:
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_C, (FlyWheel->HIGH_DRIVE)->PIN_Output_C, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_SET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_RESET);

			  (FlyWheel->LOW_DRIVE)->CCR1=0;
			  (FlyWheel->LOW_DRIVE)->CCR2=(FlyWheel->DutyCicle);
			  (FlyWheel->LOW_DRIVE)->CCR3=100;
			break;

		case 3:
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_C, (FlyWheel->HIGH_DRIVE)->PIN_Output_C, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_SET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_RESET);

			  (FlyWheel->LOW_DRIVE)->CCR1=100;
			  (FlyWheel->LOW_DRIVE)->CCR2=(FlyWheel->DutyCicle);
			  (FlyWheel->LOW_DRIVE)->CCR3=0;
			break;

		case 4:
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_C, (FlyWheel->HIGH_DRIVE)->PIN_Output_C, GPIO_PIN_SET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_RESET);

			  (FlyWheel->LOW_DRIVE)->CCR1=(FlyWheel->DutyCicle);
			  (FlyWheel->LOW_DRIVE)->CCR2=100;
			  (FlyWheel->LOW_DRIVE)->CCR3=0;
			break;

		case 5:
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_C, (FlyWheel->HIGH_DRIVE)->PIN_Output_C, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_SET);

			  (FlyWheel->LOW_DRIVE)->CCR1=0;
			  (FlyWheel->LOW_DRIVE)->CCR2=100;
			  (FlyWheel->LOW_DRIVE)->CCR3=(FlyWheel->DutyCicle);
			break;

		case 6:
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_C, (FlyWheel->HIGH_DRIVE)->PIN_Output_C, GPIO_PIN_SET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_RESET);

			  (FlyWheel->LOW_DRIVE)->CCR1=(FlyWheel->DutyCicle);
			  (FlyWheel->LOW_DRIVE)->CCR2=0;
			  (FlyWheel->LOW_DRIVE)->CCR3=100;
			break;

		default:
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_C, (FlyWheel->HIGH_DRIVE)->PIN_Output_C, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_B, (FlyWheel->HIGH_DRIVE)->PIN_Output_B, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin((FlyWheel->HIGH_DRIVE)->Port_Output_A, (FlyWheel->HIGH_DRIVE)->PIN_Output_A, GPIO_PIN_RESET);

			  (FlyWheel->LOW_DRIVE)->CCR1=0;
			  (FlyWheel->LOW_DRIVE)->CCR2=0;
			  (FlyWheel->LOW_DRIVE)->CCR3=0;
		}
}

