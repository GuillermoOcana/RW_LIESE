/*
 * hil.c
 *
 *  Created on: Nov 16, 2021
 *      Author: guill
 */

#include "hil.h"
uint16_t count_gyro=0; //test

void HIL_Transmit(uint8_t HIL_Type, uint16_t HIL_Data){

	uint8_t data[2];
	data[0] = HIL_Type;
	data[1] = HIL_Data & 0XFF;
	//CDC_Transmit_FS(data,2);
}

void HIL_Transmit_Gyro(L3GD20H_Type * l3g){


	int angle_x;
	int angle_y;
	int angle_z;

	count_gyro++;

	uint8_t data[16];

	angle_x=(int)(l3g->angle[0]*100);
	angle_y=(int)(l3g->angle[1]*100);
	angle_z=(int)(l3g->angle[2]*100);


	data[0]=angle_x & 0XFF;
	data[1]=(angle_x >> 8);
	data[2]=0x00;
	data[3]=HIL_GYRO_ANGLE_X;

	data[4]=(angle_y & 0XFF) & 0XFF;
	data[5]=angle_y >> 8;
	data[6]=0x00;
	data[7]=HIL_GYRO_ANGLE_Y;

	data[8]=(angle_z & 0XFF) & 0XFF;
	data[9]=angle_z >> 8;
	data[10]=0x00;
	data[11]=HIL_GYRO_ANGLE_Z;


	data[12]=count_gyro & 0XFF;
	data[13]=count_gyro >> 8;
	data[14]=0x00;
	data[15]=HIL_FLAG;

	//CDC_Transmit_FS(data,16);
	/*
	uint8_t data[20];
	count_gyro++;

	data[0] = (l3g->angle[0]*10) & 0xFF;
	data[1] = HIL_GYROXL;
	data[2] = (l3g->hil_gyro[0]>>8);
	data[3] = HIL_GYROXH;
	data[4] = l3g->hil_gyro[1] & 0xFF;
	data[5] = HIL_GYROYL;
	data[6] = (l3g->hil_gyro[1]>>8);
	data[7] = HIL_GYROYH;
	data[8] = l3g->hil_gyro[2] & 0xFF;
	data[9] = HIL_GYROZL;
	data[10] = (l3g->hil_gyro[2]>>8);
	data[11] = HIL_GYROZH;
	data[12] = count_gyro;
	data[13] = HIL_FLAG;


	*/

}

