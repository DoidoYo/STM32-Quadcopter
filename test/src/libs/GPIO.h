/*
 * GPIO.h
 *
 *  Created on: Jan 19, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "core.h"

class GPIO {

public:
	GPIO(GPIO_TypeDef* gpio, uint8_t pin, GPIOMode_TypeDef mode);
	void high();
	void low();
	int read();

private:
	GPIO_TypeDef* _gpio;
	uint8_t _pin;
	GPIOMode_TypeDef _mode;
};


#endif /* GPIO_H_ */
