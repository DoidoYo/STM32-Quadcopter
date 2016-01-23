/*
 * Input.h
 *
 *  Created on: Jan 19, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "Quadcopter.h"

#define RX_THROTTLE 1
#define RX_YAW 0
#define RX_ROLL 3
#define RX_PITCH 2
#define RX_AUX1 4
#define RX_AUX2 5

extern "C" {
void EXTI9_5_IRQHandler();
void EXTI15_10_IRQHandler();
}

	void initReceiver();
	void checkReceiverConnection();

	//static uint64_t pulseStart[6];
	extern uint32_t input[6];


#endif /* INPUT_H_ */
