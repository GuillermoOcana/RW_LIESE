

#include "hmc5883l.h"


void HMC5883L_Init(HMC5883L_Type * HMC,I2C_HandleTypeDef hi2c, uint8_t HMC5883L_CRA_AVG_MEASURE, uint8_t HMC5883L_CRA_DATA_OUTPUT_RATE, uint8_t HMC5883L_CRA_MEASURE_MODE, uint8_t HMC5883L_CRB_GAIN, uint8_t HMC5883L_MR_MODE){

	HMC->ADDRESS 		= HMC5883L_8_BIT_W_ADDRESS;
	HMC->READ_ADDRESS 	= HMC5883L_8_BIT_R_ADDRESS;
	HMC->WRITE_ADDRESS 	= HMC5883L_8_BIT_W_ADDRESS;
	HMC->i2c = hi2c;
	HMC->MEASUREMENT_AVG	= HMC5883L_CRA_AVG_MEASURE;
	HMC->DATA_OUTPUT_RATE	= HMC5883L_CRA_DATA_OUTPUT_RATE;
	HMC->MEASUREMENT_CONFIG	= HMC5883L_CRA_MEASURE_MODE;

	HMC->GAIN = HMC5883L_CRB_GAIN;

	HMC->OP_MODE = HMC5883L_MR_MODE;

	uint8_t HMC5883L_CRA = (HMC5883L_CRA_RESET<<7) | (HMC->MEASUREMENT_AVG << 5) | (HMC->DATA_OUTPUT_RATE <<2) | (HMC->MEASUREMENT_CONFIG);
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_A, HMC5883L_CRA);

	uint8_t HMC5883L_CRB = HMC->GAIN << 5;
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_B, HMC5883L_CRB);

	uint8_t HMC5883L_MR = (HMC5883L_MR_RESET<<2) | (HMC->OP_MODE);
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_MODE_REGISTER, HMC5883L_MR);

	HMC->SCALE_FACTOR_X=1;
	HMC->SCALE_FACTOR_Y=1;
	HMC->SCALE_FACTOR_Z=1;

	if(HMC5883L_CRA_DATA_OUTPUT_RATE == HMC5883L_CRA_DATA_OUTPUT_RATE_75)
		HMC->DATA_HZ=0.75;
	else if(HMC5883L_CRA_DATA_OUTPUT_RATE == HMC5883L_CRA_DATA_OUTPUT_RATE_150)
		HMC->DATA_HZ=1.5;
	else if(HMC5883L_CRA_DATA_OUTPUT_RATE == HMC5883L_CRA_DATA_OUTPUT_RATE_300)
		HMC->DATA_HZ=3;
	else if(HMC5883L_CRA_DATA_OUTPUT_RATE == HMC5883L_CRA_DATA_OUTPUT_RATE_750)
		HMC->DATA_HZ=7.5;
	else if(HMC5883L_CRA_DATA_OUTPUT_RATE == HMC5883L_CRA_DATA_OUTPUT_RATE_1500)
		HMC->DATA_HZ=15;
	else if(HMC5883L_CRA_DATA_OUTPUT_RATE == HMC5883L_CRA_DATA_OUTPUT_RATE_3000)
		HMC->DATA_HZ=30;
	else
		HMC->DATA_HZ=75;

	if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_1370)
		HMC->GAIN_LSB_GAUSS=1370;
	else if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_1090)
		HMC->GAIN_LSB_GAUSS=1090;
	else if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_820)
		HMC->GAIN_LSB_GAUSS=820;
	else if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_660)
		HMC->GAIN_LSB_GAUSS=660;
	else if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_440)
		HMC->GAIN_LSB_GAUSS=440;
	else if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_390)
		HMC->GAIN_LSB_GAUSS=390;
	else if(HMC5883L_CRB_GAIN == HMC5883L_CRB_GAIN_CONFIG_330)
		HMC->GAIN_LSB_GAUSS=330;
	else
		HMC->GAIN_LSB_GAUSS=230;
}

void HMC5883L_Measure_Read(HMC5883L_Type * HMC){

	uint8_t RDY;

	RDY= (readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_STATUS_REGISTER)) & 0x01;

	if(HMC->OP_MODE==HMC5883L_MR_SINGLE_MODE){
		uint8_t HMC5883L_CRB = HMC->GAIN << 5;
		writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_B, HMC5883L_CRB);
	}

	if(RDY){

		HMC->DATA_REGISTER_READ[0]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_X_MSB);
		HMC->DATA_REGISTER_READ[1]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_X_LSB);
		HMC->DATA_REGISTER_READ[2]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Z_MSB);
		HMC->DATA_REGISTER_READ[3]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Z_LSB);
		HMC->DATA_REGISTER_READ[4]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Y_MSB);
		HMC->DATA_REGISTER_READ[5]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Y_LSB);

		HMC->DATA_REGISTER_READ_16[0]= (HMC->DATA_REGISTER_READ[0]<<8) | (HMC->DATA_REGISTER_READ[1]);
		HMC->DATA_REGISTER_READ_16[1]= (HMC->DATA_REGISTER_READ[2]<<8) | (HMC->DATA_REGISTER_READ[3]);
		HMC->DATA_REGISTER_READ_16[2]= (HMC->DATA_REGISTER_READ[4]<<8) | (HMC->DATA_REGISTER_READ[5]);

		HMC->GAUSS_RESULTS[0]= HMC->SCALE_FACTOR_X*(HMC->DATA_REGISTER_READ_16[0]/HMC->GAIN_LSB_GAUSS);
		HMC->GAUSS_RESULTS[1]= HMC->SCALE_FACTOR_Z*(HMC->DATA_REGISTER_READ_16[1]/HMC->GAIN_LSB_GAUSS);
		HMC->GAUSS_RESULTS[2]= HMC->SCALE_FACTOR_Y*(HMC->DATA_REGISTER_READ_16[2]/HMC->GAIN_LSB_GAUSS);

	}
}

void HMC5883L_WhoAmI(HMC5883L_Type * HMC){
	HMC->ID_A=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_ID_REGISTER_A);
	HMC->ID_B=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_ID_REGISTER_B);
	HMC->ID_C=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_ID_REGISTER_C);
}

void HMC5883L_SelfTest(HMC5883L_Type * HMC){

	uint8_t HMC5883L_CRA = (HMC5883L_CRA_RESET<<7) | (HMC5883L_CRA_AVG_MEASURE_8 << 5) | (HMC5883L_CRA_DATA_OUTPUT_RATE_1500 <<2) | (HMC5883L_CRA_POSITIVE_MEASURE_MODE);
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_A, HMC5883L_CRA);

	uint8_t HMC5883L_CRB = HMC5883L_CRB_GAIN_CONFIG_660 << 5;
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_B, HMC5883L_CRB);

	uint8_t HMC5883L_MR = (HMC5883L_MR_RESET<<2) | (HMC5883L_MR_SINGLE_MODE);
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_MODE_REGISTER, HMC5883L_MR);

	int Delay;
	Delay=2/15*1000;

	HAL_Delay(Delay);

	uint8_t DATA_REGISTER_READ[6];
	short DATA_REGISTER_READ_16[3];

	DATA_REGISTER_READ[0]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_X_MSB);
	DATA_REGISTER_READ[1]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_X_LSB);
	DATA_REGISTER_READ[2]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Z_MSB);
	DATA_REGISTER_READ[3]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Z_LSB);
	DATA_REGISTER_READ[4]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Y_MSB);
	DATA_REGISTER_READ[5]=readreg(HMC->i2c, HMC->ADDRESS, HMC5883L_DATA_OUTPUT_Y_LSB);

	DATA_REGISTER_READ_16[0]= (DATA_REGISTER_READ[0]<<8) | (DATA_REGISTER_READ[1]);
	DATA_REGISTER_READ_16[1]= (DATA_REGISTER_READ[2]<<8) | (DATA_REGISTER_READ[3]);
	DATA_REGISTER_READ_16[2]= (DATA_REGISTER_READ[4]<<8) | (DATA_REGISTER_READ[5]);

	HMC->SCALE_FACTOR_X=766/DATA_REGISTER_READ_16[0];
	HMC->SCALE_FACTOR_Z=713/DATA_REGISTER_READ_16[1];
	HMC->SCALE_FACTOR_Y=766/DATA_REGISTER_READ_16[2];

	HMC5883L_CRA = (HMC5883L_CRA_RESET<<7) | (HMC->MEASUREMENT_AVG << 5) | (HMC->DATA_OUTPUT_RATE <<2) | (HMC->MEASUREMENT_CONFIG);
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_A, HMC5883L_CRA);

	HMC5883L_CRB = HMC->GAIN << 5;
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_CONFIG_REG_B, HMC5883L_CRB);

	HMC5883L_MR = (HMC5883L_MR_RESET<<2) | (HMC->OP_MODE);
	writereg(HMC->i2c, HMC->ADDRESS, HMC5883L_MODE_REGISTER, HMC5883L_MR);

}



