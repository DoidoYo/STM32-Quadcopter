/*
 * Delay.h
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f10x.h"

 void initDelay();
 void delayMs(int i);
 void delayUs(int i);
 uint64_t millis();
 uint64_t micros();
 void incrementMicros();

#endif /* DELAY_H_ */
