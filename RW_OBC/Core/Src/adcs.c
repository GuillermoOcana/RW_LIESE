/*
 * adcs.c
 *
 *      Created on: May 5, 2022
 *      Author: guill
 */


#include "adcs.h"

Quaternion_Type ADCS_RPY2Quaternion(double phi,double theta,double psi){

	Quaternion_Type q;
	Rotation_Matrix_Type r;
	r=ADCS_RPY2RotationMatrix(phi,theta,psi);
	q=ADCS_RotationMatrix2Quaternion(&r);
	return q;

}

Rotation_Matrix_Type ADCS_RPY2RotationMatrix(double phi, double theta, double psi){

	Rotation_Matrix_Type r;
	double r11,r12,r13,r21,r22,r23,r31,r32,r33;
	double c_phi, c_theta, c_psi, s_phi, s_theta, s_psi;

	c_phi=cos(phi);
	c_theta=cos(theta);
	c_psi=cos(psi);

	s_phi=sin(phi);
	s_theta=sin(theta);
	s_psi=sin(psi);

	r11=c_phi*c_theta;
	r12=-s_phi*c_psi+c_psi*s_theta*s_psi;
	r13=s_phi*s_psi+c_phi*s_theta*c_psi;
	r21=s_phi*c_theta;
	r22=c_phi*c_psi+s_phi*s_theta*s_psi;
	r23=-c_phi*s_psi+s_phi*s_theta*c_psi;
	r31=-s_theta;
	r32=c_theta*s_psi;
	r33=c_theta*c_psi;

	Rotation_Matrix_Type_Set(&r,r11,r12,r13,r21,r22,r23,r31,r32,r33);

	return r;

}

Quaternion_Type ADCS_RotationMatrix2Quaternion(Rotation_Matrix_Type * r){

	Quaternion_Type q;
	double q0,q1,q2,q3;

	q0=0.5*sqrt(r->r11+r->r22+r->r33+1);
	//q0=0.5;
	if(q0==0){
		q3=-1;
		q2=-1;
		q1=-1;
	}
	else{
	q3=(r->r21-r->r12)/(4*q0);
	q2=(r->r13-r->r31)/(4*q0);
	q1=(r->r23-r->r32)/(4*q0);
	}

	Quaternion_Set(&q,q0,q1,q2,q3);

	return q;

}

void Quaternion_Set(Quaternion_Type * q , double q0, double q1, double q2, double q3){

	q->q0=q0;
	q->q1=q1;
	q->q2=q2;
	q->q3=q3;

}

void Rotation_Matrix_Type_Set(Rotation_Matrix_Type * r , double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33){

	r->r11=r11;
	r->r12=r12;
	r->r13=r13;
	r->r21=r21;
	r->r22=r22;
	r->r23=r23;
	r->r31=r31;
	r->r32=r32;
	r->r33=r33;

}

void Quaternion_Get(Quaternion_Type * q , double * q0, double * q1, double * q2, double * q3){

	*q0=q->q0;
	*q1=q->q1;
	*q2=q->q2;
	*q3=q->q2;
}


void Rotation_Matrix_Type_Get(Rotation_Matrix_Type * r , double * r11, double * r12, double * r13, double * r21, double * r22, double * r23, double * r31, double * r32, double * r33){
	*r11=r->r11;
	*r12=r->r12;
	*r13=r->r13;
	*r21=r->r21;
	*r22=r->r22;
	*r23=r->r23;
	*r31=r->r31;
	*r32=r->r32;
	*r33=r->r33;
}




