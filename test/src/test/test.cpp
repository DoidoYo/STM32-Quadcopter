/*
 * test.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: Gabriel Fernandes
 */

#include "test.h"

float pid_p_gain_roll = 1.3;      //Gain setting for the roll P-controller (1.3)
float pid_i_gain_roll = 0.05;     //Gain setting for the roll I-controller (0.3)
float pid_d_gain_roll = 15;        //Gain setting for the roll D-controller (15)
int pid_max_roll = 400;             //Maximum output of the PID-controller (+/-)

float pid_p_gain_pitch = pid_p_gain_roll; //Gain setting for the pitch P-controller.
float pid_i_gain_pitch = pid_i_gain_roll; //Gain setting for the pitch I-controller.
float pid_d_gain_pitch = pid_d_gain_roll; //Gain setting for the pitch D-controller.
int pid_max_pitch = pid_max_roll;   //Maximum output of the PID-controller (+/-)

float pid_p_gain_yaw = 4.0;     //Gain setting for the pitch P-controller. //4.0
float pid_i_gain_yaw = 0.02;   //Gain setting for the pitch I-controller. //0.02
float pid_d_gain_yaw = 0.0;           //Gain setting for the pitch D-controller.
int pid_max_yaw = 400;              //Maximum output of the PID-controller (+/-)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring Variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int receiver_input_channel_1, receiver_input_channel_2,
		receiver_input_channel_3, receiver_input_channel_4;
int esc_1, esc_2, esc_3, esc_4;
int throttle, battery_voltage;
unsigned long timer_channel_1, timer_channel_2, timer_channel_3,
		timer_channel_4, esc_timer, esc_loop_timer;
int cal_int, start;
unsigned long loop_timer;
double gyro_pitch, gyro_roll, gyro_yaw;
double gyro_roll_cal, gyro_pitch_cal, gyro_yaw_cal;
uint8_t highByte, lowByte;

float pid_error_temp;
float pid_i_mem_roll, pid_roll_setpoint, gyro_roll_input, pid_output_roll,
		pid_last_roll_d_error;
float pid_i_mem_pitch, pid_pitch_setpoint, gyro_pitch_input, pid_output_pitch,
		pid_last_pitch_d_error;
float pid_i_mem_yaw, pid_yaw_setpoint, gyro_yaw_input, pid_output_yaw,
		pid_last_yaw_d_error;

I2C wire;

GPIO ledG;
GPIO ledR;

#define M1_P GPIO_Pin_1
#define M1_G GPIOA

#define M2_P GPIO_Pin_6
#define M2_G GPIOA

#define M3_P GPIO_Pin_7
#define M3_G GPIOA

#define M4_P GPIO_Pin_0
#define M4_G GPIOB

void testSetup() {
	///////////////////////////////////////////////////////////
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

	//////////////////////////////////////////////////////////
	ledG.init(LEDG_PORT, LEDG_PIN, GPIO_Mode_Out_PP);
	ledR.init(LEDR_PORT, LEDR_PIN, GPIO_Mode_Out_PP);

	initReceiver();

	wire.init(I2C1, 100000);

	wire.sendByte(105, 0x20, 0x0F);
	wire.sendByte(105, 0x23, 0x90);

	delayMs(250);

	//Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
	for (cal_int = 0; cal_int < 2000; cal_int++) { //Take 2000 readings for calibration.
		if (cal_int % 15 == 0) {
			if (GPIO_ReadInputDataBit(LEDG_PORT, LEDG_PIN)) {
				GPIO_ResetBits(LEDG_PORT, LEDG_PIN);
			} else {
				GPIO_SetBits(LEDG_PORT, LEDG_PIN);
			}
		}
		gyro_signalen();                                 //Read the gyro output.
		gyro_roll_cal += gyro_roll;            //Ad roll value to gyro_roll_cal.
		gyro_pitch_cal += gyro_pitch;        //Ad pitch value to gyro_pitch_cal.
		gyro_yaw_cal += gyro_yaw;                //Ad yaw value to gyro_yaw_cal.

		GPIO_SetBits(M1_G, M1_P);
		GPIO_SetBits(M2_G, M2_P);
		GPIO_SetBits(M3_G, M3_P);
		GPIO_SetBits(M4_G, M4_P);
		delayUs(1000);
		GPIO_ResetBits(M1_G, M1_P);
		GPIO_ResetBits(M2_G, M2_P);
		GPIO_ResetBits(M3_G, M3_P);
		GPIO_ResetBits(M4_G, M4_P);
		delayMs(3);
	}

	//Now that we have 2000 measures, we need to devide by 2000 to get the average gyro offset.
	gyro_roll_cal /= 2000;                      //Divide the roll total by 2000.
	gyro_pitch_cal /= 2000;                    //Divide the pitch total by 2000.
	gyro_yaw_cal /= 2000;                        //Divide the yaw total by 2000.

}

void testLoop() {
	gyro_signalen();
	gyro_roll_input = (gyro_roll_input * 0.8) + ((gyro_roll / 57.14286) * 0.2); //Gyro pid input is deg/sec.
	gyro_pitch_input = (gyro_pitch_input * 0.8)
			+ ((gyro_pitch / 57.14286) * 0.2);      //Gyro pid input is deg/sec.
	gyro_yaw_input = (gyro_yaw_input * 0.8) + ((gyro_yaw / 57.14286) * 0.2); //Gyro pid input is deg/sec.

	throttle = input[RX_THROTTLE];
	pid_yaw_setpoint = map2(input[RX_YAW], STICK_MIN, STICK_MAX,
			-RATE_YAW_SCALE, RATE_YAW_SCALE, STICK_DEAD_MIN,
			STICK_DEAD_MAX);
	pid_roll_setpoint = map2(input[RX_ROLL], STICK_MIN, STICK_MAX,
			-RATE_ROLL_SCALE, RATE_ROLL_SCALE, STICK_DEAD_MIN,
			STICK_DEAD_MAX);
	pid_pitch_setpoint = map2(input[RX_PITCH], STICK_MIN, STICK_MAX,
			-RATE_PITCH_SCALE, RATE_PITCH_SCALE, STICK_DEAD_MIN,
			STICK_DEAD_MAX);

	//printf2("x,y,z: %i \t %i \t %i \n", (int)gyro_roll_input, (int)gyro_pitch_input, (int)gyro_yaw_input);

	calculate_pid();

	if (throttle > 1200) {                             //The motors are started.
		if (throttle > 1800)
			throttle = 1800; //We need some room to keep full control at full throttle.
		esc_1 = throttle + pid_output_pitch + pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 1 (front-right - CCW)
		esc_2 = throttle - pid_output_pitch + pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 2 (rear-right - CW)
		esc_3 = throttle - pid_output_pitch - pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 3 (rear-left - CCW)
		esc_4 = throttle + pid_output_pitch - pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 4 (front-left - CW)

		if (esc_1 < 1200)
			esc_1 = 1200;                             //Keep the motors running.
		if (esc_2 < 1200)
			esc_2 = 1200;                             //Keep the motors running.
		if (esc_3 < 1200)
			esc_3 = 1200;                             //Keep the motors running.
		if (esc_4 < 1200)
			esc_4 = 1200;                             //Keep the motors running.

		if (esc_1 > 2000)
			esc_1 = 2000;                     //Limit the esc-1 pulse to 2000us.
		if (esc_2 > 2000)
			esc_2 = 2000;                     //Limit the esc-2 pulse to 2000us.
		if (esc_3 > 2000)
			esc_3 = 2000;                     //Limit the esc-3 pulse to 2000us.
		if (esc_4 > 2000)
			esc_4 = 2000;                     //Limit the esc-4 pulse to 2000us.
	} else {
		esc_1 = 1000;         //If start is not 2 keep a 1000us pulse for ess-1.
		esc_2 = 1000;         //If start is not 2 keep a 1000us pulse for ess-2.
		esc_3 = 1000;         //If start is not 2 keep a 1000us pulse for ess-3.
		esc_4 = 1000;         //If start is not 2 keep a 1000us pulse for ess-4.
	}

	//All the information for controlling the motor's is available.
	//The refresh rate is 250Hz. That means the esc's need there pulse every 4ms.
	while (micros() - loop_timer < 4000)
		;                                     //We wait until 4000us are passed.
	loop_timer = micros();                    //Set the timer for the next loop.

	GPIO_SetBits(M1_G, M1_P);
	GPIO_SetBits(M2_G, M2_P);
	GPIO_SetBits(M3_G, M3_P);
	GPIO_SetBits(M4_G, M4_P);

	timer_channel_1 = esc_1 + loop_timer; //Calculate the time of the faling edge of the esc-1 pulse.
	timer_channel_2 = esc_2 + loop_timer; //Calculate the time of the faling edge of the esc-2 pulse.
	timer_channel_3 = esc_3 + loop_timer; //Calculate the time of the faling edge of the esc-3 pulse.
	timer_channel_4 = esc_4 + loop_timer; //Calculate the time of the faling edge of the esc-4 pulse.

	while (GPIO_ReadInputDataBit(M1_G,M1_P) | GPIO_ReadInputDataBit(M2_G,M2_P) | GPIO_ReadInputDataBit(M3_G,M3_P) | GPIO_ReadInputDataBit(M4_G,M4_P)) {  //Stay in this loop until output 4,5,6 and 7 are low.
		esc_loop_timer = micros();                      //Read the current time.
		if (timer_channel_1 <= esc_loop_timer)
			GPIO_ResetBits(M1_G, M1_P);
		if (timer_channel_2 <= esc_loop_timer)
			GPIO_ResetBits(M2_G, M2_P);
		if (timer_channel_3 <= esc_loop_timer)
			GPIO_ResetBits(M3_G, M3_P);
		if (timer_channel_4 <= esc_loop_timer)
			GPIO_ResetBits(M4_G, M4_P);
	}

}

void gyro_signalen() {
	uint8_t data[6];
	wire.readBytes(105, 168, 6, data);

	gyro_roll = (float)((int16_t)((data[1] << 8) | data[0])); //Multiply highByte by 256 (shift left by 8) and ad lowByte
	if (cal_int == 2000)
		gyro_roll -= gyro_roll_cal;      //Only compensate after the calibration

	gyro_pitch = (float)((int16_t)((data[3] << 8) | data[2])); //Multiply highByte by 256 (shift left by 8) and ad lowByte                                       //Invert axis
	if (cal_int == 2000)
		gyro_pitch -= gyro_pitch_cal;    //Only compensate after the calibration

	gyro_yaw = (float)((int16_t)((data[5] << 8) | data[4])); //Multiply highByte by 256 (shift left by 8) and ad lowByte
	gyro_yaw *= -1;                                              //Invert axis
	if (cal_int == 2000)
		gyro_yaw -= gyro_yaw_cal;        //Only compensate after the calibration
}

void calculate_pid() {
	//Roll calculations
	pid_error_temp = gyro_roll_input - pid_roll_setpoint;
	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
	if (pid_i_mem_roll > pid_max_roll)
		pid_i_mem_roll = pid_max_roll;
	else if (pid_i_mem_roll < pid_max_roll * -1)
		pid_i_mem_roll = pid_max_roll * -1;

	pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll
			+ pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
	if (pid_output_roll > pid_max_roll)
		pid_output_roll = pid_max_roll;
	else if (pid_output_roll < pid_max_roll * -1)
		pid_output_roll = pid_max_roll * -1;

	pid_last_roll_d_error = pid_error_temp;

	//Pitch calculations
	pid_error_temp = gyro_pitch_input - pid_pitch_setpoint;
	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
	if (pid_i_mem_pitch > pid_max_pitch)
		pid_i_mem_pitch = pid_max_pitch;
	else if (pid_i_mem_pitch < pid_max_pitch * -1)
		pid_i_mem_pitch = pid_max_pitch * -1;

	pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch
			+ pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
	if (pid_output_pitch > pid_max_pitch)
		pid_output_pitch = pid_max_pitch;
	else if (pid_output_pitch < pid_max_pitch * -1)
		pid_output_pitch = pid_max_pitch * -1;

	pid_last_pitch_d_error = pid_error_temp;

	//Yaw calculations
	pid_error_temp = gyro_yaw_input - pid_yaw_setpoint;
	pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
	if (pid_i_mem_yaw > pid_max_yaw)
		pid_i_mem_yaw = pid_max_yaw;
	else if (pid_i_mem_yaw < pid_max_yaw * -1)
		pid_i_mem_yaw = pid_max_yaw * -1;

	pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw
			+ pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
	if (pid_output_yaw > pid_max_yaw)
		pid_output_yaw = pid_max_yaw;
	else if (pid_output_yaw < pid_max_yaw * -1)
		pid_output_yaw = pid_max_yaw * -1;

	pid_last_yaw_d_error = pid_error_temp;
}

int map2(int x, int min, int max, int nmin, int nmax, int deadmin,
		int deadmax) {

	int nx = 0;

	if (x < deadmax && x > deadmin) {
		return (nmin + nmax) / 2;
	}
	nx = (x - min) * (nmax - nmin) / (max - min) + nmin;
	if (nx > nmax) {
		return nmax;
	}
	if (nx < nmin) {
		return nmin;
	}
	return nx;
}
