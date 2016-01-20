
#include "core.h"

#include "libs/I2C.h"
#include "sensors/BMP180.h"
#include "sensors/L3G4200D.h"

//init i2c
I2C i2c(I2C1, 100000);

//init sensors
BMP180 bmp(&i2c,3);
L3G4200D gyro(&i2c);

//////////
GPIO ledB(GPIOB, LEDB_PIN, GPIO_Mode_Out_PP);
GPIO ledR(GPIOB, LEDR_PIN, GPIO_Mode_Out_PP);
GPIO buzzer(GPIOB, BUZZER_PIN, GPIO_Mode_Out_PP);

GPIO button(GPIOB, BUTTON_PIN, GPIO_Mode_IPU);

int main(void) {

	initMillis();
	initUSART1(9600);

	bmp.init();
	gyro.init();

	while (1) {
		long P;
		int16_t x,y,z;
		bmp.getPressure(P);
		gyro.readAngle(x,y,z);

	}
}

