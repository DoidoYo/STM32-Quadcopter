/*
 * L3G4300D.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#include "L3G4200D.h"

L3G4200D::L3G4200D() {
}

void readGyroTask() {
	//gyro.readRaw(raw.x, raw.y, raw.z);
	//readAngle(angle.x,angle.y,angle.z);
}

void L3G4200D::init(I2C *i2c) {
	I2Cx = i2c;
	sendBit(L3G4200D_CTRL_REG1, L3G4200D_CTRL_REG1_ON);
	sendBit(L3G4200D_CTRL_REG4,
	L3G4200D_CTRL_REG4_CONST_UPDATE | L3G4200D_CTRL_REG4_SCALE_500);

	zeroX = 0;
	zeroY = 0;
	zeroZ = 0;

	delayMs(150);
}

void L3G4200D::calibrate() {

	vector v;

	for (int i = 0; i < 200; i++) {
		readRaw(v);

		zeroX += v.x;
		zeroY += v.y;
		zeroZ += v.z;

		delayMs(1);
	}

	zeroX /= 200;
	zeroY /= 200;
	zeroZ /= 200;

}

void L3G4200D::readAngle(vector &raw, vector &angle) {
	readRaw(raw);

	uint8_t data[1];
	getBits(L3G4200D_CTRL_REG4, 1, data);
	uint8_t sensitivity = ((data[0] >> 4) & (0b0011));

	if (sensitivity == (0b00)) {
		angle.x = raw.x / 8.75;
		angle.y = raw.y / 8.75;
		angle.z = raw.z / 8.75;
	} else if (sensitivity == (0b01)) {
		angle.x = raw.x / 17.5;
		angle.y = raw.y / 17.5;
		angle.z = raw.z / 17.5;
	} else if (sensitivity == (0b10)) {
		angle.x = raw.x / 70;
		angle.y = raw.y / 70;
		angle.z = raw.z / 70;
	}
}

void L3G4200D::readAngle(int16_t &x, int16_t &y, int16_t &z) {
	uint8_t data[1];
	getBits(L3G4200D_CTRL_REG4, 1, data);

	readRaw(x, y, z);

	uint8_t sensitivity = ((data[0] >> 4) & (0b11));

	//printf2("sensitivity: %i\n", sensitivity);

	if (sensitivity == (0b00)) {
		x /= 8.75;
		y /= 8.75;
		z /= 8.75;
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
	x = (data[0]) | (data[1] << 8);
	y = (data[2]) | (data[3] << 8);
	z = (data[4]) | (data[5] << 8);
}

void L3G4200D::readRaw(vector &data) {
	int16_t x, y, z;
	readRaw(x, y, z);
	data.x = x + 28;
	data.y = y - 33;
	data.z = z - 31;
}

void L3G4200D::getBits(uint8_t reg, int bytes, uint8_t *data) {
	I2Cx->readBytes(L3G4200D_ADDR, reg, bytes, data);
}

void L3G4200D::sendBit(uint8_t reg, uint8_t data) {
	I2Cx->sendByte(L3G4200D_ADDR, reg, data);
}
