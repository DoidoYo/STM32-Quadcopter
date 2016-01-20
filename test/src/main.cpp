
#include "core.h"

#include "libs/I2C.h"
#include "libs/BMP180.h"
#include "libs/L3G4200D.h"

I2C i2c(I2C1, 100000);

BMP180 bmp(&i2c,3);
L3G4200D gyro(&i2c);

GPIO led(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP);

int main(void) {

	initMillis();
	initUSART1(9600);

	bmp.init();
	gyro.init();

	led.high();

	while (1) {
		long P;
		int16_t x,y,z;
		bmp.getPressure(P);
		gyro.readAngle(x,y,z);

	}
}

