/*
 * PID.h
 *
 * Created: 10/28/2021 2:08:17 PM
 *  Author: danwo
 */ 


#ifndef PID_H_
#define PID_H_

typedef struct PID{
	float Kp, Ki, Kd;			// PID Constants
	float iMin, iMax;			// Integral Limits
	float Ep, Ei, Ed, E_old;	// Error Values
} PID_t;

void PID_init(float kp, float ki, float kd, float iMin, float iMax);
float PID_error(float SetPoint, float input);
void PID_update(float error);

#endif /* PID_H_ */