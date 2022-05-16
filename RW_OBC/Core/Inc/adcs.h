/*
 * l3gd20h.h
 *
 *  Created on: Nov 16, 2021
 *      Author: Guillermo Ocaña Anaya
 */

#ifndef INC_ADCS_H_
#define INC_ADCS_H_

#include <math.h>

typedef struct{

	double r11;
	double r12;
	double r13;
	double r21;
	double r22;
	double r23;
	double r31;
	double r32;
	double r33;

}Rotation_Matrix_Type;

typedef struct{

	double q0;
	double q1;
	double q2;
	double q3;

}Quaternion_Type;

void Quaternion_Set(Quaternion_Type * q , double q0, double q1, double q2, double q3);
void Rotation_Matrix_Type_Set(Rotation_Matrix_Type * r , double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33);

void Quaternion_Get(Quaternion_Type * q , double * q0, double * q1, double * q2, double * q3);
void Rotation_Matrix_Type_Get(Rotation_Matrix_Type * r , double * r11, double * r12, double * r13, double * r21, double * r22, double * r23, double * r31, double * r32, double * r33);

Quaternion_Type ADCS_RPY2Quaternion(double phi,double theta,double psi);
Rotation_Matrix_Type ADCS_RPY2RotationMatrix(double phi, double theta, double psi);
Quaternion_Type ADCS_RotationMatrix2Quaternion(Rotation_Matrix_Type * r);

#endif /* INC_ADCS_H_ */




