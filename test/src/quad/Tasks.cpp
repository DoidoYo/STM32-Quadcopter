/*
 * Tasks.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#include "Quadcopter.h"

int c = 0;
uint32_t tmm;

void TASK_readGyro() {
	gyro.readAngle(angleRaw, angleScaled);
	angleRaw.z *= -1;
	angleScaled.z *= -1;

	angleSmooth.x = (angleSmooth.x * .8) + ((angleRaw.x / 57.14286) * .2);
	angleSmooth.y = (angleSmooth.y * .8) + ((angleRaw.y / 57.14286) * .2);
	angleSmooth.z = (angleSmooth.z * .8) + ((angleRaw.z / 57.14286) * .2);

	//angleSmooth.x = (angleRaw.x/57.14286);
	//angleSmooth.y = (angleRaw.y/57.14286);
	//angleSmooth.z = (angleRaw.z/57.14286);

	//printf2("Angle x: %i \ty: %i \tz: %i \n", (int)angleSmooth.x,(int)angleSmooth.y,(int)angleSmooth.z);
}

void TASK_readBaro() {
	baro.getPressure(pressure);
}

void TASK_readAccel() {

}

void TASK_readMag() {

}

uint32_t armedTimer;

void TASK_flightController() {
	if (Armed == ARMED && scaledInput[RX_THROTTLE] > 1050) {
		armedTimer = millis();

		rateOut[ROLL] = ratePID[ROLL].compute(angleSmooth.x,
				scaledInput[RX_ROLL]);
		rateOut[PITCH] = ratePID[PITCH].compute(angleSmooth.y,
				scaledInput[RX_PITCH]);
		rateOut[YAW] = ratePID[YAW].compute(angleSmooth.z, scaledInput[RX_YAW]);

		//	printf2("Scaled input:\t %i \t %i \t %i\n\n", scaledInput[RX_ROLL], scaledInput[RX_PITCH], scaledInput[RX_YAW]);
		/*printf2("%i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i\n",
		 (int) rateOut[ROLL], (int) rateOut[PITCH], (int) rateOut[YAW],
		 scaledInput[RX_ROLL], scaledInput[RX_PITCH],
		 scaledInput[RX_YAW], (int) angleSmooth.x, (int) angleSmooth.y,
		 (int) angleSmooth.z);*/

		if(scaledInput[RX_THROTTLE] > 1800) {
			scaledInput[RX_THROTTLE] = 1800;
		}
		motors[M_FR] = scaledInput[RX_THROTTLE] + rateOut[ROLL] + rateOut[PITCH]
				- rateOut[YAW];
		motors[M_FL] = scaledInput[RX_THROTTLE] - rateOut[ROLL] + rateOut[PITCH]
				+ rateOut[YAW];
		motors[M_BL] = scaledInput[RX_THROTTLE] - rateOut[ROLL] - rateOut[PITCH]
				- rateOut[YAW];
		motors[M_BR] = scaledInput[RX_THROTTLE] + rateOut[ROLL] - rateOut[PITCH]
				+ rateOut[YAW];

		//printf2("%i \t %i \t %i \t%i \n", motors[M_FL],motors[M_BL],motors[M_FR],motors[M_BR]);

		if (motors[M_FR] > MOTOR_MAX) {
			motors[M_FR] = MOTOR_MAX;
		}
		if (motors[M_FL] > MOTOR_MAX) {
			motors[M_FL] = MOTOR_MAX;
		}
		if (motors[M_BL] > MOTOR_MAX) {
			motors[M_BL] = MOTOR_MAX;
		}
		if (motors[M_BR] > MOTOR_MAX) {
			motors[M_BR] = MOTOR_MAX;
		}

		if (motors[M_FR] < MOTOR_MIN) {
			motors[M_FR] = MOTOR_MIN;
		}
		if (motors[M_FL] < MOTOR_MIN) {
			motors[M_FL] = MOTOR_MIN;
		}
		if (motors[M_BL] < MOTOR_MIN) {
			motors[M_BL] = MOTOR_MIN;
		}
		if (motors[M_BR] < MOTOR_MIN) {
			motors[M_BR] = MOTOR_MIN;
		}

	} else if (Armed == ARMED) {

		if ((millis() - armedTimer) > ARMED_TIMEOUT) {
			Armed = UNARMED;
		}

		LED_G.high();

		motors[M_FR] = 1100;
		motors[M_FL] = 1100;
		motors[M_BL] = 1100;
		motors[M_BR] = 1100;

		for (int i = 0; i < 3; i++) {
			ratePID[i].reset();
		}

	} else {
		LED_G.low();

		motors[M_FR] = 1000;
		motors[M_FL] = 1000;
		motors[M_BL] = 1000;
		motors[M_BR] = 1000;
	}

	updateMotors();

	/*if((millis() - tmm) > 1000) {
	 printf2("hz: %i\n", c);
	 c=0;
	 tmm = millis();
	 } else {
	 c++;
	 }*/
}

int map(int x, int min, int max, int nmin, int nmax, int deadmin, int deadmax) {

	int nx = 0;

	if (x < deadmax && x > deadmin) {
		return (nmin + nmax) / 2;
	}
	nx = (x - min) * (nmax - nmin) / (max - min) + nmin;
	if (nx > nmax) {
		return nmax;
	}
	if (nx < nmin) {
		return nmin;
	}
	return nx;
}

void TASK_receiverCheck() {

	checkReceiverConnection();

	//scale input
	if (input[0] != 0) {
		scaledInput[RX_THROTTLE] = input[RX_THROTTLE];
		scaledInput[RX_YAW] = map(input[RX_YAW], STICK_MIN, STICK_MAX,
				-RATE_YAW_SCALE, RATE_YAW_SCALE, STICK_DEAD_MIN,
				STICK_DEAD_MAX);
		scaledInput[RX_ROLL] = map(input[RX_ROLL], STICK_MIN, STICK_MAX,
				-RATE_ROLL_SCALE, RATE_ROLL_SCALE, STICK_DEAD_MIN,
				STICK_DEAD_MAX);
		scaledInput[RX_PITCH] = map(input[RX_PITCH], STICK_MIN, STICK_MAX,
				-RATE_PITCH_SCALE, RATE_PITCH_SCALE, STICK_DEAD_MIN,
				STICK_DEAD_MAX);
	} else {
		for (int i = 0; i < 6; i++) {
			scaledInput[i] = 0;
		}
	}
	//printf2("Scaled input: %i \t %i \t %i\n", scaledInput[RX_YAW], scaledInput[RX_PITCH], scaledInput[RX_ROLL]);

	//printf2("%i\t%i\t%i\t%i\t%i\t%i\n", input[0], input[1], input[2], input[3],
	//		input[4], input[5]);
}

int STICKRIGHT(int s) {
	return ((s > STICK_HIGH) ? 1 : 0);
}
int STICKLEFT(int s) {
	return ((s < STICK_LOW) ? 1 : 0);
}
int STICKUP(int s) {
	return ((s > STICK_HIGH) ? 1 : 0);
}
int STICKDOWN(int s) {
	return ((s < STICK_LOW) ? 1 : 0);
}

uint32_t stickTimer;

void TASK_stickReader() {

	int delayOver = ((millis() - stickTimer) > 1500) ? 1 : 0;

	//arm
	if (STICKRIGHT(input[RX_YAW]) && STICKDOWN(input[RX_THROTTLE])) {

		if (delayOver) {
			Armed = ARMED;
			armedTimer = millis();
		}

	} else if (STICKLEFT(input[RX_YAW]) && STICKDOWN(input[RX_THROTTLE])) {

		if (delayOver) {
			Armed = UNARMED;
		}

	} else {
		stickTimer = millis();
	}
}

