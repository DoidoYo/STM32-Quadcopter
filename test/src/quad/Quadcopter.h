/*
 * Quadcopter.h
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef QUADCOPTER_H_
#define QUADCOPTER_H_

#include "core.h"

#include "libs/I2C.h"
#include "libs/GPIO.h"

#include "sensors/BMP180.h"
#include "sensors/L3G4200D.h"

#define BUTTON_PIN GPIO_Pin_0
#define BUTTON_PORT GPIOA

#define LEDR_PIN GPIO_Pin_5
#define LEDR_PORT GPIOA

#define LEDG_PIN GPIO_Pin_4
#define LEDG_PORT GPIOA

#define BUZZER_PIN GPIO_Pin_5
#define BUZZER_PORT GPIOB

extern I2C i2c;

extern BMP180 baro;
extern L3G4200D gyro;

extern GPIO LED_G;
extern GPIO LED_R;
extern GPIO LED_SYS;
extern GPIO BUZZER;
extern GPIO BUTTON;

extern vector angleRaw, angleScaled;
extern vector accelRaw, accelScaled;
extern vector headingRaw, headingScaled;
extern long pressure;

void TASK_readGyro();
void TASK_readBaro();
void TASK_readAccel();
void TASK_readMag();
void TASK_computeIMU();

#endif /* QUADCOPTER_H_ */
