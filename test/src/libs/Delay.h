/*
 * Delay.h
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f10x.h"

 void initMillis();
 void delay(int i);
 uint32_t millis();
 void incrementMillis();

#endif /* DELAY_H_ */
