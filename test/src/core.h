/*
 * core.h
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef CORE_H_
#define CORE_H_

#include "stm32f10x.h"

#ifdef __cplusplus

#include "libs/GPIO.h"
//#include "quad/Quadcopter.h"

extern "C" {
#endif

#include "libs/Delay.h"
#include "libs/USART1.h"

#ifdef __cplusplus
}
#endif


//typedef struct vector { float x, y, z; };

#endif /* CORE_H_ */
