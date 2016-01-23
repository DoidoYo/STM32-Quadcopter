/*
 * PID.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel Fernandes
 */

#include "PID.h"

PID::PID() {
}

void PID::init(float *in, float *out, float *set, float KP, float KI,
		float KD, float max,float min) {

	_in = in;
	_out = out;
	_set = set;
	_KP = KP;
	_KI = KI;
	_KD = KD;

	_max = max;
		_min = min;
}

void PID::restrain(float max, float min) {
	_max = max;
	_min = min;
}

void PID::reset() {
	lastError = 0;
	integral = 0;
}

void PID::compute() {

	float error = *_in - *_set;

	integral += _KI * error;
	if (integral > _max)
		integral = _max;
	else if (integral < _min)
		integral = _min;

	lastError = error;

	output = (_KP * error) + (_KI) + (_KD * (error - lastError));
	if (output > _max)
		output = _max;
	else if (output < _min)
		output = _min;

	*_out = output;
}
