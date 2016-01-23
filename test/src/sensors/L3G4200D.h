/*
 * L3G4300D.h
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef L3G4200D_H_
#define L3G4200D_H_

#include "core.h"
#include "libs/I2C.h"

#define L3G4200D_WHO_AM_I      0x0F

#define L3G4200D_ADDR (0xD2 >> 1)

#define L3G4200D_CTRL_REG1     0x20
#define L3G4200D_CTRL_REG4     0x23
#define L3G4200D_OUT_TEMP      0x26
#define L3G4200D_STATUS_REG    0x27

#define L3G4200D_CTRL_REG4_CONST_UPDATE 0b10000000
#define L3G4200D_CTRL_REG4_SCALE_250 0x00
#define L3G4200D_CTRL_REG4_SCALE_500 0b00010000
#define L3G4200D_CTRL_REG4_SCALE_2000 0b00100000

#define L3G4200D_CTRL_REG1_ON 0b00001111

#define L3G4200D_OUT_INCREMENT 0x28 | (1<<7)
#define L3G4200D_OUT_X_L       0x28
#define L3G4200D_OUT_X_H       0x29
#define L3G4200D_OUT_Y_L       0x2A
#define L3G4200D_OUT_Y_H       0x2B
#define L3G4200D_OUT_Z_L       0x2C
#define L3G4200D_OUT_Z_H       0x2D


struct vector {
	float x, y, z;
};

class L3G4200D {

public:
	L3G4200D();
	void init(I2C *i2c);
	void calibrate();
	void readRaw(int16_t &x, int16_t &y, int16_t &z);
	//void readRaw(float &x, float &y, float &z);
	void readRaw(vector &data);
	void readAngle(int16_t &x, int16_t &y, int16_t &z);
	void readAngle(vector &raw,vector &angle);
	//void readAngle(vector *v);

private:
	I2C *I2Cx;

	float zeroX, zeroY, zeroZ;

	void getBits(uint8_t reg, int bytes, uint8_t *data);
	void sendBit(uint8_t reg, uint8_t data);

};

#endif /* L3G4300D_H_ */
