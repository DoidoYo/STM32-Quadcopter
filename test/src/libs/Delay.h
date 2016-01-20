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

 /*void initMillis() {
	SysTick_Config(SystemCoreClock / 1000);
}

uint32_t millis() {
	return _millisCounter;
}

void delay(int DelayTime) {
	uint32_t delayTimer = 0;
	delayTimer = millis();
	while ((millis() - delayTimer) < (uint32_t) DelayTime);
}

void incrementMillis() {
	_millisCounter++;
}*/
