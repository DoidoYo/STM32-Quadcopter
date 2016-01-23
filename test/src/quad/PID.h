/*
 * PID.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef PID_H_
#define PID_H_

#include "core.h"

class PID {

public:
	PID();
	void init(float *in, float *out, float *set, float KP, float KI, float KD, float max, float min);
	void restrain(float max, float min);
	void reset();
	void compute();

private:
	float *_in,*_out,*_set,_KP,_KI,_KD;
	float lastError, integral, output;
	float _max,_min;
};


#endif /* PID_H_ */
