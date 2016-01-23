/*
 * test.h
 *
 *  Created on: Jan 23, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef TEST_H_
#define TEST_H_

#include "libs/I2C.h"
#include "libs/GPIO.h"
#include "quad/Input.h"

#define BUTTON_PIN GPIO_Pin_0
#define BUTTON_PORT GPIOA

#define LEDR_PIN GPIO_Pin_5
#define LEDR_PORT GPIOA

#define LEDG_PIN GPIO_Pin_4
#define LEDG_PORT GPIOA

#define BUZZER_PIN GPIO_Pin_5
#define BUZZER_PORT GPIOB

void testSetup();
void testLoop();
void gyro_signalen();
void calculate_pid();
int map2(int x, int min, int max, int nmin, int nmax, int deadmin, int deadmax);

#endif /* TEST_H_ */
