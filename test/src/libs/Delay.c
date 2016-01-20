/*
 * Delay.c
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */


#include "Delay.h"

uint32_t _millisCounter;

void initMillis() {
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
}
