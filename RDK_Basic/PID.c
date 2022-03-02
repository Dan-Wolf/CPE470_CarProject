/*
 * PID.c
 *
 * Created: 10/28/2021 2:08:02 PM
 *  Author: danwo
 */ 

/*	TODO: This should be updated to only use floats instead of doubles.
	Could be further optimized by using ints. This would require scaling the 
	ADC inputs and then using constants that are powers of 2 to allow for bit 
	shifting instead of multiplies
*/

#include "PID.h"
#include <plib.h>
#include <math.h>
#include "stdtypes.h"

volatile PID_t pid_2;
volatile PID_t pid_3;

void PID_init_2(float kp, float ki, float kd, float iMin, float iMax) {
	pid_2.Kp = kp;
	pid_2.Ki = ki;
	pid_2.Kd = kd;
	pid_2.iMin = iMin;
	pid_2.iMax = iMax;
	pid_2.Ep = 0;
	pid_2.Ei = 0;
	pid_2.Ed = 0;
	pid_2.E_old = 0;
}

void PID_init_3(float kp, float ki, float kd, float iMin, float iMax) {
	pid_3.Kp = kp;
	pid_3.Ki = ki;
	pid_3.Kd = kd;
	pid_3.iMin = iMin;
	pid_3.iMax = iMax;
	pid_3.Ep = 0;
	pid_3.Ei = 0;
	pid_3.Ed = 0;
	pid_3.E_old = 0;
}

float PID_error_2(float SetPoint, float input) {
	float error = SetPoint - input;		// Calculate error
	pid_2.Ep = error * pid_2.Kp;				// Calculate Ep
	pid_2.Ei += error * pid_2.Ki;				// Calculate Ei
	
	// Integral Windup Compensation 
	if (pid_2.Ei > pid_2.iMax) {				// Check vs Max
		pid_2.Ei = pid_2.iMax;
	}
	else if (pid_2.Ei < pid_2.iMin) {			// Check vs Min
		pid_2.Ei = pid_2.iMin;
	}
	
	pid_2.Ed = (error - pid_2.E_old) * pid_2.Kd;	// Calculate Ed
	pid_2.E_old = error;						// Update old value
	
	return pid_2.Ep + pid_2.Ei + pid_2.Ed;
}

float PID_error_3(float SetPoint, float input) {
	float error = SetPoint - input;		// Calculate error
	pid_3.Ep = error * pid_3.Kp;				// Calculate Ep
	pid_3.Ei += error * pid_3.Ki;				// Calculate Ei
	
	// Integral Windup Compensation 
	if (pid_3.Ei > pid_3.iMax) {				// Check vs Max
		pid_3.Ei = pid_3.iMax;
	}
	else if (pid_3.Ei < pid_3.iMin) {			// Check vs Min
		pid_3.Ei = pid_3.iMin;
	}
	
	pid_3.Ed = (error - pid_3.E_old) * pid_3.Kd;	// Calculate Ed
	pid_3.E_old = error;						// Update old value
	
	return pid_3.Ep + pid_3.Ei + pid_3.Ed;
}

void PID_update_2(float error) {
	if (error > 10000) {
		OC2R = 9999;
        OC2RS = 9999;
        //OC3R = 9999;
       // OC3RS = 9999;
	}
	else if (error < 1000) {
		OC2R = 1000;
        OC2RS = 1000;
        //OC3R = 1000;
        //OC3RS = 1000;
	}
	else {
		//error = floor(error);           // Makes whole number       
        OC2R = (uint32_t)(error);         // Cast as integer
        OC2RS = (uint32_t)(error);
        //OC3R = (uint32_t)(error);
        //OC3RS = (uint32_t)(error);
	}
}

void PID_update_3(float error) {
	if (error > 10000) {
		//OC2R = 9999;
        //OC2RS = 9999;
        OC3R = 9999;
        OC3RS = 9999;
	}
	else if (error < 1000) {
		//OC2R = 1000;
        //OC2RS = 1000;
        OC3R = 1000;
        OC3RS = 1000;
	}
	else {
		//error = floor(error);           // Makes whole number       
        //OC2R = (uint32_t)(error);         // Cast as integer
        //OC2RS = (uint32_t)(error);
        OC3R = (uint32_t)(error);
        OC3RS = (uint32_t)(error);
	}
}