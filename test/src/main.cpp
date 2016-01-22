#include "quad/Quadcopter.h"

#include "quad/TaskManager.h"

#include "quad/test.h"

int main(void) {

	//populate variables
	i2c.init(I2C1, 400000);

	baro.init(&i2c, 3);
	gyro.init(&i2c);

	LED_G.init(LEDG_PORT, LEDG_PIN, GPIO_Mode_Out_PP);
	LED_R.init(LEDR_PORT, LEDR_PIN, GPIO_Mode_Out_PP);
	LED_SYS.init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP);
	BUZZER.init(BUZZER_PORT, BUZZER_PIN, GPIO_Mode_Out_PP);
	BUTTON.init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_IPU);

	//init tasks
	TaskManager::addTask(&TASK_readGyro, 100);
	TaskManager::addTask(&TASK_readBaro, 40);

	//init
	initMillis();
	initUSART1(9600);

	while (1) {
		TaskManager::run();
	}
}
