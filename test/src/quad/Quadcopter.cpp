/*
 * Quadcopter.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel's PC
 */

#include "Quadcopter.h"

I2C i2c;

BMP180 baro;
L3G4200D gyro;

GPIO LED_G;
GPIO LED_R;
GPIO LED_SYS;
GPIO BUZZER;
GPIO BUTTON;

vector angleRaw, angleScaled;
vector accelRaw, accelScaled;
vector headingRaw, headingScaled;
long pressure;

PID ratePID[3];
float rateSet[3], rateOut[3], rateIn[3];

void initQuad() {

	//init tasks
	TaskManager::addTask(&TASK_readGyro, 200);
	TaskManager::addTask(&TASK_readBaro, 50 * (4));

	TaskManager::addTask(&TASK_flightController, 200);
	TaskManager::addTask(&TASK_receiverCheck, 55); //rc only updates at 55hz

	//init
	initReceiver();
	initMotors();

	//populate variables
	i2c.init(I2C1, 400000);

	baro.init(&i2c, 2);
	gyro.init(&i2c);
	//gyro.calibrate();

	LED_G.init(LEDG_PORT, LEDG_PIN, GPIO_Mode_Out_PP);
	LED_R.init(LEDR_PORT, LEDR_PIN, GPIO_Mode_Out_PP);
	LED_SYS.init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP);
	BUZZER.init(BUZZER_PORT, BUZZER_PIN, GPIO_Mode_Out_PP);
	BUTTON.init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_IPU);

	ratePID[ROLL].init(&rateIn[ROLL], &rateOut[ROLL], &rateSet[ROLL], ROLL_KP, ROLL_KI, ROLL_KD, ROLL_MAX, -ROLL_MAX);
	ratePID[PITCH].init(&rateIn[PITCH], &rateOut[PITCH], &rateSet[PITCH], PITCH_KP, PITCH_KI, PITCH_KD, PITCH_MAX, -PITCH_MAX);
	ratePID[YAW].init(&rateIn[YAW], &rateOut[YAW], &rateSet[YAW], YAW_KP, YAW_KI, YAW_KD, YAW_MAX, -YAW_MAX);

}

void loop() {
	TaskManager::run();
}
