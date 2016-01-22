/*
 * Tasks.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */




#include "Quadcopter.h"

int i = 0;
uint32_t time;

 void TASK_readGyro() {
	gyro.readAngle(angleRaw, angleScaled);

	if((millis() - time) > 1000) {
		printf2("Hz: %i\n", i);
		time = millis();
		i = 0;
	} else {
		i++;
	}

	//printf2("%i \t %i \t %i\n", angleRaw.x, angleRaw.y, angleRaw.z);
}

 void TASK_readBaro() {
	baro.getPressure(pressure);
}

 void TASK_readAccel() {

}

 void TASK_readMag() {

}
