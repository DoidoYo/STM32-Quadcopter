/*
 * L3G4300D.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */


#include "L3G4200D.h"

L3G4200D::L3G4200D(I2C *i2c) {
	I2Cx = i2c;
}

void L3G4200D::init() {
	sendBit(L3G4200D_CTRL_REG1, L3G4200D_CTRL_REG1_ON);
	sendBit(L3G4200D_CTRL_REG4, L3G4200D_CTRL_REG4_CONST_UPDATE | L3G4200D_CTRL_REG4_SCALE_500);
}

void L3G4200D::readAngle(int16_t &x, int16_t &y, int16_t &z) {
	uint8_t data[1];
	getBits(L3G4200D_CTRL_REG4, 1 ,data);

	readRaw(x,y,z);

	uint8_t sensitivity = ((data[0]>>4) & (0b11));

	//printf2("sensitivity: %i\n", sensitivity);

	if(sensitivity == (0b00)) {
		x /=8.75;
		y /=8.75;
		z /=8.75;
	} else if (sensitivity == (0b01)) {
		x /= 17.5;
		y /= 17.5;
		z /= 17.5;
	} else if (sensitivity == (0b10)) {
		x /= 70;
		y /= 70;
		z /= 70;
	}

}

/*void L3G4200D::readAngle(vector *v) {
	int16_t x,y,z;
	readAngle(x,y,z);
	v->x = x;
	v->y = y;
	v->z = z;
}*/

void L3G4200D::readRaw(int16_t &x, int16_t &y, int16_t &z) {
	uint8_t data[6];
	getBits(L3G4200D_OUT_INCREMENT, 6, data);
	x = (data[0]) | (data[1]<<8);
	y = (data[2]) | (data[3]<<8);
	z = (data[4]) | (data[5]<<8);
}

void L3G4200D::readRaw(int16_t *data) {
	int16_t x,y,z;
	readRaw(x,y,z);
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

void L3G4200D::getBits(uint8_t reg, int bytes, uint8_t *data) {
	I2Cx->readBytes(L3G4200D_ADDR, reg,bytes,data);
}

void L3G4200D::sendBit(uint8_t reg, uint8_t data) {
	I2Cx->sendByte(L3G4200D_ADDR, reg, data);
}
