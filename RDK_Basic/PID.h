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

void PID_init_2(float kp, float ki, float kd, float iMin, float iMax);
void PID_init_3(float kp, float ki, float kd, float iMin, float iMax);
float PID_error_2(float SetPoint, float input);
float PID_error_3(float SetPoint, float input);
void PID_update_2(float error);
void PID_update_3(float error);

#endif /* PID_H_ */