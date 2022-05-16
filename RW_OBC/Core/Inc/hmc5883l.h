/*
 * hcm5883l.h
 *
 *  Created on: Nov 16, 2021
 *      Author: Guillermo Ocaña Anaya
 */


#ifndef INC_HMC5883L_H_
#define INC_HMC5883L_H_

#include "stm32f4xx_hal.h"
#include "wire.h"


typedef struct{

	I2C_HandleTypeDef i2c;

	uint8_t ADDRESS;
	uint8_t READ_ADDRESS;
	uint8_t WRITE_ADDRESS;

	uint8_t MEASUREMENT_AVG;
	uint8_t DATA_OUTPUT_RATE;
	uint8_t MEASUREMENT_CONFIG;
	uint8_t GAIN;
	uint8_t OP_MODE;

	uint8_t DATA_REGISTER_READ[6];
	short DATA_REGISTER_READ_16[3];

	uint8_t ID_A;
	uint8_t ID_B;
	uint8_t ID_C;

	double DATA_HZ;
	int GAIN_LSB_GAUSS;

	double GAUSS_RESULTS[3];

	double SCALE_FACTOR_X;
	double SCALE_FACTOR_Y;
	double SCALE_FACTOR_Z;


}HMC5883L_Type;

// Slave Addresses
#define   HMC5883L_7_BIT_ADDRESS		   			0x1E
#define   HMC5883L_8_BIT_R_ADDRESS		   			0x3D
#define   HMC5883L_8_BIT_W_ADDRESS		   			0x3C

// Register Map
#define   HMC5883L_CONFIG_REG_A						0x00
#define   HMC5883L_CONFIG_REG_B						0x01
#define   HMC5883L_MODE_REGISTER					0x02
#define   HMC5883L_DATA_OUTPUT_X_MSB				0x03
#define   HMC5883L_DATA_OUTPUT_X_LSB				0x04
#define   HMC5883L_DATA_OUTPUT_Z_MSB				0x05
#define   HMC5883L_DATA_OUTPUT_Z_LSB				0x06
#define   HMC5883L_DATA_OUTPUT_Y_MSB				0x07
#define   HMC5883L_DATA_OUTPUT_Y_LSB				0x08
#define   HMC5883L_STATUS_REGISTER					0x09
#define   HMC5883L_ID_REGISTER_A					0x0A
#define   HMC5883L_ID_REGISTER_B					0x0B
#define   HMC5883L_ID_REGISTER_C					0x0C

// Configuration Register A: CRA7
#define   HMC5883L_CRA_SET							0x01
#define   HMC5883L_CRA_RESET						0x00

// Configuration Register A: CRA6-CRA5
#define   HMC5883L_CRA_AVG_MEASURE_1				0x00
#define   HMC5883L_CRA_AVG_MEASURE_2				0x01
#define   HMC5883L_CRA_AVG_MEASURE_4				0x02
#define   HMC5883L_CRA_AVG_MEASURE_8				0x03

// Configuration Register A: CRA4-CRA2
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_75			0x00 	//	75 x 10^-2 [Hz]  (0.75 [Hz])
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_150			0x01 	//	150 x 10^-2 [Hz] (1.5 [Hz])
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_300			0x02 	//	300 x 10^-2 [Hz] (3 [Hz])
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_750			0x03 	//	750 x 10^-2 [Hz] (7.5 [Hz])
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_1500		0x04 	//	1500 x 10^-2 [Hz] (15 [Hz])
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_3000		0x05 	//	3000 x 10^-2 [Hz] (30 [Hz])
#define   HMC5883L_CRA_DATA_OUTPUT_RATE_7500		0x06 	//	7500 x 10^-2 [Hz] (75 [Hz])

// Configuration Register A: CRA1-CRA0
#define   HMC5883L_CRA_NORMAL_MEASURE_MODE			0x00
#define   HMC5883L_CRA_POSITIVE_MEASURE_MODE		0x01
#define   HMC5883L_CRA_NEGATIVE_MEASURE_MODE		0x02

// Configuration Register B: CRB7-CRB5
#define   HMC5883L_CRB_GAIN_CONFIG_1370				0x00
#define   HMC5883L_CRB_GAIN_CONFIG_1090				0x01
#define   HMC5883L_CRB_GAIN_CONFIG_820				0x02
#define   HMC5883L_CRB_GAIN_CONFIG_660				0x03
#define   HMC5883L_CRB_GAIN_CONFIG_440				0x04
#define   HMC5883L_CRB_GAIN_CONFIG_390				0x05
#define   HMC5883L_CRB_GAIN_CONFIG_330				0x06
#define   HMC5883L_CRB_GAIN_CONFIG_230				0x07

// Configuration Register B: CRB4-CRB0
#define   HMC5883L_CRB_RESET						0x00

// Mode Register: MR7-MR2
#define   HMC5883L_MR_SET							0x02
#define   HMC5883L_MR_RESET							0x00

// Mode Register: MR1-MR0
#define   HMC5883L_MR_CONTINUOUS_MODE				0x00
#define   HMC5883L_MR_SINGLE_MODE					0x01
#define   HMC5883L_MR_IDLE1_MODE					0x02
#define   HMC5883L_MR_IDLE2_MODE					0x03

// Status Register: SR1
#define   HMC5883L_SR_LOCK							0x02

// Status Register: SR0
#define   HMC5883L_SR_RDY							0x01

// Identification Register A: ASCII H
#define   HMC5883L_ID_A								0x48

// Identification Register B: ASCII 4
#define   HMC5883L_ID_B								0x34

// Identification Register C: ASCII 3
#define   HMC5883L_ID_C								0x33


void HMC5883L_Init(HMC5883L_Type * HMC,I2C_HandleTypeDef hi2c, uint8_t HMC5883L_CRA_AVG_MEASURE, uint8_t HMC5883L_CRA_DATA_OUTPUT_RATE, uint8_t HMC5883L_CRA_MEASURE_MODE, uint8_t HMC5883L_CRB_GAIN, uint8_t HMC5883L_MR_MODE);
void HMC5883L_Measure_Read(HMC5883L_Type * HMC);
void HMC5883L_WhoAmI(HMC5883L_Type * HMC);
void HMC5883L_SelfTest(HMC5883L_Type * HMC);

#endif /* INC_HMC5883L_H_ */
