/*
 * Quadcopter.h
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef QUADCOPTER_H_
#define QUADCOPTER_H_

#include "core.h"
#include "math.h"

#include "libs/I2C.h"
#include "libs/GPIO.h"

#include "sensors/BMP180.h"
#include "sensors/L3G4200D.h"

#include "quad/Input.h"
#include "quad/Motors.h"
#include "quad/PID.h"
#include "quad/TaskManager.h"
///////////////////////////////////////
/////      STICK SCALING     //////////
//////////////////////////////////////

#define RATE_ROLL_SCALE  120
#define RATE_PITCH_SCALE 120
#define RATE_YAW_SCALE   120

////////////////////////////
#define BUTTON_PIN GPIO_Pin_0
#define BUTTON_PORT GPIOA

#define LEDR_PIN GPIO_Pin_5
#define LEDR_PORT GPIOA

#define LEDG_PIN GPIO_Pin_4
#define LEDG_PORT GPIOA

#define BUZZER_PIN GPIO_Pin_5
#define BUZZER_PORT GPIOB
///////////////////////////////////


#define ROLL_KP 1.3
#define ROLL_KI 0.05
#define ROLL_KD 15
#define ROLL_MAX 400

#define PITCH_KP 1.3
#define PITCH_KI 0.05
#define PITCH_KD 15
#define PITCH_MAX 400

#define YAW_KP 4
#define YAW_KI 0.02
#define YAW_KD 0
#define YAW_MAX 400

#define ROLL 0
#define PITCH 1
#define YAW 2
///////////////////////////////

#define MOTOR_MIN 1000
#define MOTOR_MAX 2000

#define ARMED 1
#define UNARMED 0

#define ARMED_TIMEOUT 5000

#define STICK_DEAD 50
#define STICK_HIGH 1800
#define STICK_LOW 1100
#define STICK_MAX MOTOR_MAX
#define STICK_MIN MOTOR_MIN
#define STICK_DEAD_MAX ((STICK_MAX+STICK_MIN)/2)+50
#define STICK_DEAD_MIN ((STICK_MAX+STICK_MIN)/2)-50

///////////////////////////////
extern I2C i2c;

extern BMP180 baro;
extern L3G4200D gyro;

extern GPIO LED_G;
extern GPIO LED_R;
extern GPIO LED_SYS;
extern GPIO BUZZER;
extern GPIO BUTTON;

extern vector angleRaw, angleScaled, angleSmooth;
extern vector accelRaw, accelScaled;
extern vector headingRaw, headingScaled;
extern long pressure;

extern int scaledInput[6];

extern PID ratePID[3];
extern float rateSet[3], rateOut[3], rateIn[3];

extern int Armed;

void initQuad();

void loop();

void TASK_readGyro();
void TASK_readBaro();
void TASK_readAccel();
void TASK_readMag();
void TASK_flightController();
void TASK_receiverCheck();
void TASK_stickReader();


#endif /* QUADCOPTER_H_ */
