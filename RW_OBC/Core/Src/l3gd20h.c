/*
 * l3gd20h.c
 *
 *  Created on: Nov 16, 2021
 *      Author: guill
 */


#include "l3gd20h.h"

double sx[4],sy[4],sz[4];

void L3GD20H_Init(L3GD20H_Type * l3g,I2C_HandleTypeDef hi2c,uint8_t L3GD20H_DR,uint8_t L3GD20H_FR, uint8_t L3GD20H_BW, uint8_t L3GD20H_POWER_MODE,uint8_t L3GD20H_LOWODR){

	l3g->i2c = hi2c;
	l3g->ADDRESS = L3GD20H_ADDRESS;
	l3g->LOWODR = L3GD20H_LOWODR;
	l3g->FR = L3GD20H_FR;
	l3g->BW = L3GD20H_BW;
	l3g->DR = L3GD20H_DR;
	l3g->POWER_MODE = L3GD20H_POWER_MODE;

	l3g->offset[0] = 0;
	l3g->offset[1] = 0;
	l3g->offset[2] = 0;
	l3g->angle[0]=0;
	l3g->angle[1]=0;
	l3g->angle[2]=0;

	sx[0]=0;
	sx[1]=0;
	sx[2]=0;
	sx[3]=0;
	sy[0]=0;
	sy[1]=0;
	sy[2]=0;
	sy[3]=0;
	sz[0]=0;
	sz[1]=0;
	sz[2]=0;
	sz[3]=0;


	writereg(l3g->i2c,l3g->ADDRESS , L3GD20H_LOW_ODR , L3GD20H_LOWODR);

	writereg(l3g->i2c,l3g->ADDRESS , L3GD20H_CTRL4 , L3GD20H_FR);

	writereg(l3g->i2c,l3g->ADDRESS , L3GD20H_CTRL1 , L3GD20H_DR | L3GD20H_BW | L3GD20H_POWER_MODE );

	if(L3GD20H_FR==L3GD20H_FR_250)
		l3g->gain_fr=250;
	else if(L3GD20H_FR==L3GD20H_FR_500)
		l3g->gain_fr=500;
	else
		l3g->gain_fr=2000;


}

void L3GD20H_Read(L3GD20H_Type * l3g,float dt){

	l3g->gyro_raw[0]=readreg(l3g->i2c,l3g->ADDRESS , L3GD20H_OUT_X_L );
	l3g->gyro_raw[1]=readreg(l3g->i2c,l3g->ADDRESS , L3GD20H_OUT_X_H );
	l3g->gyro_raw[2]=readreg(l3g->i2c,l3g->ADDRESS , L3GD20H_OUT_Y_L );
	l3g->gyro_raw[3]=readreg(l3g->i2c,l3g->ADDRESS , L3GD20H_OUT_Y_H );
	l3g->gyro_raw[4]=readreg(l3g->i2c,l3g->ADDRESS , L3GD20H_OUT_Z_L );
	l3g->gyro_raw[5]=readreg(l3g->i2c,l3g->ADDRESS , L3GD20H_OUT_Z_H );

	l3g->hil_gyro[0]=l3g->gyro_raw[0]+(l3g->gyro_raw[1]<<8)-l3g->offset[0];
	l3g->hil_gyro[1]=l3g->gyro_raw[2]+(l3g->gyro_raw[3]<<8)-l3g->offset[1];
	l3g->hil_gyro[2]=l3g->gyro_raw[4]+(l3g->gyro_raw[5]<<8)-l3g->offset[2];

	l3g->gyro[0]=l3g->hil_gyro[0]*l3g->gain_fr/32768;
	l3g->gyro[1]=l3g->hil_gyro[1]*l3g->gain_fr/32768;
	l3g->gyro[2]=l3g->hil_gyro[2]*l3g->gain_fr/32768;


	sx[3]=sx[2];
	sy[3]=sy[2];
	sz[3]=sz[2];

	sx[2]=sx[1];
	sy[2]=sy[1];
	sz[2]=sz[1];

	sx[1]=sx[0];
	sy[1]=sy[0];
	sz[1]=sz[0];

	sx[0]=l3g->gyro[0];
	sy[0]=l3g->gyro[1];
	sz[0]=l3g->gyro[2];

	l3g->angle[0]+=(dt/8)*(sx[0]+3*(sx[1]+sx[2])+sx[3]);
	l3g->angle[1]+=(dt/8)*(sy[0]+3*(sy[1]+sy[2])+sy[3]);
	l3g->angle[2]+=(dt/8)*(sz[0]+3*(sz[1]+sz[2])+sz[3]);

}

void L3GD20H_Calibration(L3GD20H_Type * l3g , TIM_HandleTypeDef htim1){


	uint8_t i=0;
	float dt=0;
	short Error[3];		//Error utilizado en el cálculo del PI.
	float P[3];			//Termino proporcional
	double I[3]={0,0,0};			//Termino Integral
	//double Esum=0;		//Acumulador del error en las lecturas
	double error=0;		//Suma de errores de los 3 ejes.
	float kP=0.05;		//Ganancia proporcional
	float kI=1;		//Ganancian Integral
	//int Econt=0;		//Contador del número de veces que se calcula el error y el PI
	//double PromE=0;		//Error promedio
	int Ecount=0;
	double ErrorMax=40;	//Máximo error permitido
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	HAL_Delay(10);
	do{

		dt=__HAL_TIM_GET_COUNTER(&htim1);
		dt=dt*0.000001;
		__HAL_TIM_SET_COUNTER(&htim1, 0);
		L3GD20H_Read(l3g,dt);
		error=fabs(l3g->hil_gyro[0])+fabs(l3g->hil_gyro[1])+fabs(l3g->hil_gyro[2]);		//Se suman las lecturas para obtener un error entre estas
		/*
		Esum+=error;																							//Se va acumulando el error obtenido anteriormente.
		Econt++;																								//Se incrementa el contador encargado de ver el número de veces que se realiza el calculo de PI
		PromE=fabs(Esum/Econt);																					//Se calcula el error promedio con el uso de la suma de errores y el contador de las veces que se realiza.
		 */
		for(i=0;i<=2;i++){
			Error[i]=l3g->hil_gyro[i]; 						//Se calcula el error entre un offset 0 y la aceleración en un eje del giroscopio
			P[i]=kP*Error[i];								//Se calcula el término proporcional, multiplicando al error por la gananacia kp
			I[i]+=Error[i]*dt*kI;			    			//Se calcula el término integral acumulando la multiplicación del error obtenido por la ganacia ki y por un delay entre lecturas.
			l3g->offset[i]=(int)(P[i]+I[i]);				//Se juntan los dos términos, el proporcional (P) y el integral (I).

		}

		if(error<ErrorMax){
			Ecount++;
		}
		else{
			Ecount=0;
		}
		HAL_Delay(10);

	}while(Ecount<15);

	l3g->angle[0]=0;
	l3g->angle[1]=0;
	l3g->angle[2]=0;

}


