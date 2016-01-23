/*
 * Motors.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include "Quadcopter.h"

#define M_FR 0
#define M_FL 1
#define M_BL 2
#define M_BR 3

extern uint16_t motors[4];

void initMotors();
void updateMotors();


#endif /* MOTORS_H_ */
