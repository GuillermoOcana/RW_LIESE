/**
  * @file           : RW_Driver.c
  * @author			: Jesús Yamil Robles López
  * @author			: Guillermo Ocaña Anaya
  * @date			: 10/12/2021
  * @brief          : Functions for RW Driver
  *
  */

#include "RW_Driver.h"

/**
  * @brief RW_Driver Initialization Function
  * @param RW_Driver Structure that contains the information of the wheels
  * @param FlyWheel0 Contains information about the Wheel 0
  * @param FlyWheel1 Contains information about the Wheel 1
  * @param FlyWheel2 Contains information about the Wheel 2
  * @param FlyWheel3 Contains information about the Wheel 3
  * @retval None
  */
void RW_Driver_Init(RW_Driver_Type* RW_Driver, FlyWheel_Type* FlyWheel0, FlyWheel_Type* FlyWheel1, FlyWheel_Type* FlyWheel2, FlyWheel_Type * FlyWheel3){
	RW_Driver->FlyWheel0=FlyWheel0;
	RW_Driver->FlyWheel1=FlyWheel1;
	RW_Driver->FlyWheel2=FlyWheel2;
	RW_Driver->FlyWheel3=FlyWheel3;

	RW_Driver->DriverStatus=0;
}

/**
 * @brief CAN request to Get information
 * @param RW_Driver Structure that contains the information of the wheels
 * @param RxData CAN Frame that details the requested info
 * @retval CAN Frame with the requested info
 */
uint8_t RW_Driver_ReadSolicitude(RW_Driver_Type* RW_Driver, uint8_t RxData[]){

	uint8_t ID= (RxData[0]>>6) & 0x3;
	uint8_t Instruction=(RxData[0]>>1) & 0x1F;
	uint8_t Data=0;
	FlyWheel_Type* FlyWheel;

	switch(ID){
		case 0:
			FlyWheel= RW_Driver->FlyWheel0;
			break;
		case 1:
			FlyWheel= RW_Driver->FlyWheel1;
			break;
		case 2:
			FlyWheel= RW_Driver->FlyWheel2;
			break;
		case 3:
			FlyWheel= RW_Driver->FlyWheel3;
			break;
		default:
			return Data;
	}

	if(Instruction==0){ //Se pidió PowerMode
		Data= FlyWheel->PowerMode;
	}
	else if(Instruction==1){
		Data= FlyWheel->DutyCicle;
	}
	else{
		Data= RW_Driver->DriverStatus;
	}

	return Data;

}

/**
 * @brief CAN request to Set information
 * @param RW_Driver Structure that contains the information of the wheels
 * @param RxData CAN Frame that details the information to modify
 * @retval None
 */
void RW_Driver_WriteSolicitude(RW_Driver_Type* RW_Driver, uint8_t RxData[]){

	uint8_t ID= (RxData[0]>>6) & 0x3;
	uint8_t Instruction=(RxData[0]>>1) & 0x1F;
	uint8_t Data=RxData[1];
	FlyWheel_Type* FlyWheel;

	switch(ID){
		case 0:
			FlyWheel= RW_Driver->FlyWheel0;
			break;
		case 1:
			FlyWheel= RW_Driver->FlyWheel1;
			break;
		case 2:
			FlyWheel= RW_Driver->FlyWheel2;
			break;
		case 3:
			FlyWheel= RW_Driver->FlyWheel3;
			break;
		default:
			FlyWheel= NULL;
	}

	if (FlyWheel != NULL){
		if(Instruction==0){ //Se pidió modificar PowerMode
			//FlyWheel->PowerMode= Data;
			RW_Driver_SetPowerMode(RW_Driver, FlyWheel, Data);
		}
		else if (Instruction==1){
			if (Data>=0 && Data<=100)
				FlyWheel->DutyCicle= Data;
			else
				FlyWheel->DutyCicle= 50;
		}
		else{
			if(Data){
				RW_Diver_Start(RW_Driver);
			}
			else{
				RW_Diver_Stop(RW_Driver);
			}
		}
	}

}

/**
 * @brief RW_Driver starts working
 * @param RW_Driver Structure that contains the information of the wheels
 * @retval None
 */
void RW_Diver_Start(RW_Driver_Type* RW_Driver){
	if (!(RW_Driver->DriverStatus)){
		FlyWheel_StartPWM(RW_Driver->FlyWheel0);
		FlyWheel_StartPWM(RW_Driver->FlyWheel1);
		FlyWheel_StartPWM(RW_Driver->FlyWheel2);
		FlyWheel_StartPWM(RW_Driver->FlyWheel3);

		RW_Driver->DriverStatus=1;

		uint8_t phase;

		phase= (GPIOD->IDR) & 0x7;
		FlyWheel_MoveMotor(RW_Driver->FlyWheel0, phase);

		phase= ((GPIOE->IDR) & 0x70)>>4;
		FlyWheel_MoveMotor(RW_Driver->FlyWheel1, phase);

		phase= (((GPIOB->IDR) & 0x500)|((GPIOC->IDR) & 0x200))>>8;
		FlyWheel_MoveMotor(RW_Driver->FlyWheel2, phase);

		phase= ((GPIOB->IDR) & 0xE000)>>13;;
		FlyWheel_MoveMotor(RW_Driver->FlyWheel3, phase);
	}
}

/**
 * @brief RW_Driver stops working
 * @param RW_Driver Structure that contains the information of the wheels
 * @retval None
 */
void RW_Diver_Stop(RW_Driver_Type* RW_Driver){
	if (RW_Driver->DriverStatus){
		FlyWheel_StopPWM(RW_Driver->FlyWheel0);
		FlyWheel_StopPWM(RW_Driver->FlyWheel1);
		FlyWheel_StopPWM(RW_Driver->FlyWheel2);
		FlyWheel_StopPWM(RW_Driver->FlyWheel3);

		RW_Driver->DriverStatus=0;
	}
}

/**
 * @brief Turn a wheel On/Off
 * @param RW_Driver Structure that contains the information of the wheels
 * @param FlyWheel Structure that contains the information of one specific wheel
 * @param Data Set the status of the Wheel
 */
void RW_Driver_SetPowerMode(RW_Driver_Type* RW_Driver, FlyWheel_Type* FlyWheel, uint8_t Data){
	if(RW_Driver->DriverStatus){
		if(Data){
			FlyWheel_StartPWM(FlyWheel);

			uint8_t phase;
			if(FlyWheel->Wheel_Id==0)
				phase= (GPIOD->IDR) & 0x7;

			else if(FlyWheel->Wheel_Id==1)
				phase= ((GPIOE->IDR) & 0x70)>>4;

			else if (FlyWheel->Wheel_Id==2)
				phase= (((GPIOB->IDR) & 0x500)|((GPIOC->IDR) & 0x200))>>8;

			else if (FlyWheel->Wheel_Id==3)
				phase= ((GPIOB->IDR) & 0xE000)>>13;

			else
				phase=0;

			FlyWheel_MoveMotor(FlyWheel, phase);
		}
		else
			FlyWheel_StopPWM(FlyWheel);
	}
}
