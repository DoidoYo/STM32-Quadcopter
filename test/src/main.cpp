#include "quad/Quadcopter.h"

#include "quad/TaskManager.h"

#include "quad/test.h"

I2C i2c;

BMP180 baro;
L3G4200D gyro;

GPIO LED_G;
GPIO LED_R;
GPIO LED_SYS;
GPIO BUZZER;
GPIO BUTTON ;

vector angleRaw, angleScaled;
vector accelRaw, accelScaled;
vector headingRaw, headingScaled;
long pressure;

int main(void) {

	//populate variables
	i2c = I2C(I2C1, 400000);

	baro = BMP180(&i2c, 1);
	gyro = L3G4200D(&i2c);

	LED_G = GPIO(LEDG_PORT, LEDG_PIN, GPIO_Mode_Out_PP);
	LED_R = GPIO(LEDR_PORT, LEDR_PIN, GPIO_Mode_Out_PP);
	LED_SYS = GPIO(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP);
	BUZZER = GPIO(BUZZER_PORT, BUZZER_PIN, GPIO_Mode_Out_PP);
	BUTTON = GPIO(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_IPU);

	//init tasks
	TaskManager::addTask(&TASK_readGyro, 100);
	TaskManager::addTask(&TASK_readBaro, 40);

	//init
	initMillis();
	initUSART1(9600);

	baro.init();
	gyro.init();

	while (1) {
		TaskManager::run();
	}
}
