#include "core.h"

//#include "quad/Quadcopter.h"

#include "test/test.h"

int main(void) {

	initDelay();
	initUSART1(115200);

	testSetup();

	while(1) {
		testLoop();
	}

}

/*
uint32_t tim;

int esc_1, esc_2, esc_3, esc_4;
int throttle, battery_voltage;
unsigned long timer_channel_1, timer_channel_2, timer_channel_3,
		timer_channel_4, esc_timer, esc_loop_timer, loop_timer;

#define M1_P GPIO_Pin_1
#define M1_G GPIOA

#define M2_P GPIO_Pin_6
#define M2_G GPIOA

#define M3_P GPIO_Pin_7
#define M3_G GPIOA

#define M4_P GPIO_Pin_0
#define M4_G GPIOB

int main(void) {

	//init
	initDelay();
	initUSART1(115200);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef init;

	//motor 4 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_0;
	init.GPIO_Mode = GPIO_Mode_Out_PP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &init);

	//motor 3 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &init);

	//motor 2 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &init);

	//motor 1 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &init);

	//initQuad();
	//testSetup();

	tim = millis();

	while (1) {
		//loop();

		if ((millis() - tim) < 4000) {
			esc_1 = 2000;
			esc_2 = 2000;
			esc_3 = 2000;
			esc_4 = 2000;
		} else {
			esc_1 = 1000;
			esc_2 = 1000;
			esc_3 = 1000;
			esc_4 = 1000;
		}

		while (micros() - loop_timer < 4000)
			;                                 //We wait until 4000us are passed.
		loop_timer = micros();                //Set the timer for the next loop.

		GPIO_SetBits(M1_G, M1_P);
		GPIO_SetBits(M2_G, M2_P);
		GPIO_SetBits(M3_G, M3_P);
		GPIO_SetBits(M4_G, M4_P);

		timer_channel_1 = esc_1 + loop_timer; //Calculate the time of the faling edge of the esc-1 pulse.
		timer_channel_2 = esc_2 + loop_timer; //Calculate the time of the faling edge of the esc-2 pulse.
		timer_channel_3 = esc_3 + loop_timer; //Calculate the time of the faling edge of the esc-3 pulse.
		timer_channel_4 = esc_4 + loop_timer; //Calculate the time of the faling edge of the esc-4 pulse.

		while (GPIO_ReadInputDataBit(M1_G, M1_P)
				| GPIO_ReadInputDataBit(M2_G, M2_P)
				| GPIO_ReadInputDataBit(M3_G, M3_P)
				| GPIO_ReadInputDataBit(M4_G, M4_P)) { //Stay in this loop until output 4,5,6 and 7 are low.
			esc_loop_timer = micros();                  //Read the current time.
			if (timer_channel_1 <= esc_loop_timer)
				GPIO_ResetBits(M1_G, M1_P);
			if (timer_channel_2 <= esc_loop_timer)
				GPIO_ResetBits(M2_G, M2_P);
			if (timer_channel_3 <= esc_loop_timer)
				GPIO_ResetBits(M3_G, M3_P);
			if (timer_channel_4 <= esc_loop_timer)
				GPIO_ResetBits(M4_G, M4_P);
		}

		//testLoop();

		//delayMs(100);
	}
}
*/
