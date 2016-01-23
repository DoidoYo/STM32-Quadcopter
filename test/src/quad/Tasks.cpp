/*
 * Tasks.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#include "Quadcopter.h"

int16_t avgX, avgY, avgZ;
int counter;

void TASK_readGyro() {
	gyro.readAngle(angleRaw, angleScaled);
	angleRaw.z *= -1;
	angleScaled.z *= -1;

	counter++;

	avgX += angleRaw.x;
	avgY += angleRaw.y;
	avgZ += angleRaw.z;

	//printf2("Angle x: %i \ty: %i \tz: %i \tAvg x: %i \ty: %i \tz: %i\n", angleRaw.x,angleRaw.y,angleRaw.z, (avgX/counter), (avgY/counter), (avgZ/counter));
}

void TASK_readBaro() {
	baro.getPressure(pressure);
}

void TASK_readAccel() {

}

void TASK_readMag() {

}

void TASK_flightController() {
	motors[M_FL] = input[RX_THROTTLE];
	updateMotors();
}

void TASK_receiverCheck() {

	checkReceiverConnection();

	printf2("%i\t%i\t%i\t%i\t%i\t%i\n", input[0], input[1], input[2], input[3],input[4], input[5]);
}
