/*
 * Quadcopter.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel's PC
 */


#include "Quadcopter.h"

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
