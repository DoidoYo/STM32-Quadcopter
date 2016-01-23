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

vector angleRaw, angleScaled, angleSmooth;
vector accelRaw, accelScaled;
vector headingRaw, headingScaled;
long pressure;

int scaledInput[6];

PID ratePID[3];
float rateSet[3], rateOut[3], rateIn[3];

int Armed;

void initQuad() {

	Armed = UNARMED;

	//init tasks
	TaskManager::addTask(&TASK_readGyro, 150);
	//TaskManager::addTask(&TASK_readBaro, 50 * (4));

	TaskManager::addTask(&TASK_flightController, 200);
	TaskManager::addTask(&TASK_receiverCheck, 55); //rc only updates at 55hz
	TaskManager::addTask(&TASK_stickReader, 20);

	//init
	initReceiver();
	initMotors();

	//populate variables

	LED_G.init(LEDG_PORT, LEDG_PIN, GPIO_Mode_Out_PP);
	LED_R.init(LEDR_PORT, LEDR_PIN, GPIO_Mode_Out_PP);
	LED_SYS.init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP);
	BUZZER.init(BUZZER_PORT, BUZZER_PIN, GPIO_Mode_Out_PP);
	BUTTON.init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_IPU);

	i2c.init(I2C1, 100000);

	baro.init(&i2c, 2);
	gyro.init(&i2c);

	ratePID[ROLL].init(ROLL_KP, ROLL_KI, ROLL_KD, ROLL_MAX);
	ratePID[PITCH].init(PITCH_KP, PITCH_KI, PITCH_KD, PITCH_MAX);
	ratePID[YAW].init(YAW_KP, YAW_KI, YAW_KD, YAW_MAX);

	//throttle bypass to configure esc
	if (BUTTON.read() == 0) {
		LED_G.high();
		LED_R.high();
		motors[M_FR] = MOTOR_MAX;
		motors[M_FL] = MOTOR_MAX;
		motors[M_BL] = MOTOR_MAX;
		motors[M_BR] = MOTOR_MAX;
		updateMotors();
		delayMs(4000);
		motors[M_FR] = MOTOR_MIN;
		motors[M_FL] = MOTOR_MIN;
		motors[M_BL] = MOTOR_MIN;
		motors[M_BR] = MOTOR_MIN;
		updateMotors();
		LED_G.low();
		LED_R.low();
	}

	delayMs(1000);

	LED_R.high();
	gyro.calibrate();
	LED_R.low();

}

void loop() {
	TaskManager::run();
}
