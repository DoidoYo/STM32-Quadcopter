/*
 * Input.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#include "Input.h"

uint32_t input[6];
uint64_t pulseStart[6];

void initReceiver() {

	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef init;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	//D24 - (PA11)
	init.GPIO_Pin = GPIO_Pin_11;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);

	exti.EXTI_Line = EXTI_Line11;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D27 - (PA8)///////////////////////////////
	init.GPIO_Pin = GPIO_Pin_8;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);

	exti.EXTI_Line = EXTI_Line8;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D28 - (PB15)/////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_15;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);

	exti.EXTI_Line = EXTI_Line15;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D29 - (PB14)////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_14;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

	exti.EXTI_Line = EXTI_Line14;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D30 - (PB13)/////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_13;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

	exti.EXTI_Line = EXTI_Line13;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D31 - (PB12)///////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_12;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

	exti.EXTI_Line = EXTI_Line12;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

}

void checkReceiverConnection() {
	for (int i = 0; i < 6; i++) {
		if ((micros() - pulseStart[i]) > 21000) {
			input[i] = 0;
		}
	}
}

extern "C" {

void EXTI9_5_IRQHandler() {

	if (EXTI_GetITStatus(EXTI_Line8) == SET) {

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)) {
			pulseStart[RX_AUX1] = micros();
		} else {
			input[RX_AUX1] = micros() - pulseStart[RX_AUX1];
			//printf2("Aux1 %i\n", input[RX_AUX1]);
		}

		EXTI_ClearITPendingBit(EXTI_Line8);
	}

}

void EXTI15_10_IRQHandler() {

	if (EXTI_GetITStatus(EXTI_Line11) == SET) {

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)) {
			pulseStart[RX_AUX2] = micros();
		} else {
			input[RX_AUX2] = micros() - pulseStart[RX_AUX2];
			//printf2("Aux2 %i\n", input[RX_AUX2]);
		}

		EXTI_ClearITPendingBit(EXTI_Line11);
	}

	if (EXTI_GetITStatus(EXTI_Line12) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)) {
			pulseStart[RX_YAW] = micros();
		} else {
			input[RX_YAW] = micros() - pulseStart[RX_YAW];
			//printf2("Yaw %i\n", input[RX_YAW]);
		}

		EXTI_ClearITPendingBit(EXTI_Line12);
	}

	if (EXTI_GetITStatus(EXTI_Line13) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)) {
			pulseStart[RX_THROTTLE] = micros();
		} else {
			input[RX_THROTTLE] = micros() - pulseStart[RX_THROTTLE];
			//printf2("Throttle %i\n", input[RX_THROTTLE]);
		}

		EXTI_ClearITPendingBit(EXTI_Line13);
	}

	if (EXTI_GetITStatus(EXTI_Line14) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)) {
			pulseStart[RX_PITCH] = micros();
		} else {
			input[RX_PITCH] = micros() - pulseStart[RX_PITCH];
			//printf2("Pitch %i\n", input[RX_PITCH]);
		}

		EXTI_ClearITPendingBit(EXTI_Line14);
	}

	if (EXTI_GetITStatus(EXTI_Line15) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)) {
			pulseStart[RX_ROLL] = micros();
		} else {
			input[RX_ROLL] = micros() - pulseStart[RX_ROLL];
			//printf2("Roll %i\n", input[RX_ROLL]);
		}

		EXTI_ClearITPendingBit(EXTI_Line15);
	}

}
}

