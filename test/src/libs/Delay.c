/*
 * Delay.c
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */


#include "Delay.h"

uint64_t _microCounter;

void initDelay() {
	SysTick_Config(SystemCoreClock / 1000000);
}

uint64_t millis() {
	return _microCounter/1000;
}

uint64_t micros() {
	return _microCounter;
}

void delayMs(int DelayTime) {
	uint64_t delayTimer = 0;
	delayTimer = millis();
	while ((millis() - delayTimer) < (uint32_t) DelayTime);
}

void incrementMicros() {
	_microCounter++;
}
