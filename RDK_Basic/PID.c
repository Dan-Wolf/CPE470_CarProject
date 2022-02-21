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

volatile PID_t pid;

void PID_init(float kp, float ki, float kd, float iMin, float iMax) {
	pid.Kp = kp;
	pid.Ki = ki;
	pid.Kd = kd;
	pid.iMin = iMin;
	pid.iMax = iMax;
	pid.Ep = 0;
	pid.Ei = 0;
	pid.Ed = 0;
	pid.E_old = 0;
}

float PID_error(float SetPoint, float input) {
	float error = SetPoint - input;		// Calculate error
	pid.Ep = error * pid.Kp;				// Calculate Ep
	pid.Ei += error * pid.Ki;				// Calculate Ei
	
	// Integral Windup Compensation 
	if (pid.Ei > pid.iMax) {				// Check vs Max
		pid.Ei = pid.iMax;
	}
	else if (pid.Ei < pid.iMin) {			// Check vs Min
		pid.Ei = pid.iMin;
	}
	
	pid.Ed = (error - pid.E_old) * pid.Kd;	// Calculate Ed
	pid.E_old = error;						// Update old value
	
	return pid.Ep + pid.Ei + pid.Ed;
}

void PID_update(float error) {
	if (error > 10000) {
		OC2R = 9999;
        OC2RS = 9999;
        OC3R = 9999;
        OC3RS = 9999;
	}
	else if (error < 0) {
		OC2R = 0;
        OC2RS = 0;
        OC3R = 0;
        OC3RS = 0;
	}
	else {
		error = floor(error);           // Makes whole number       
        OC2R = (uint32_t)(error);         // Cast as integer
        OC2RS = (uint32_t)(error);
        OC3R = (uint32_t)(error);
        OC3RS = (uint32_t)(error);
	}
}