/*
 * TaskManager.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */


#include "TaskManager.h"

int TaskManager::counter = 0;
uint32_t TaskManager::_hz[TASK_SIZE] = {};
uint32_t TaskManager::_timer[TASK_SIZE] = {};
Function TaskManager::_function[TASK_SIZE] = {};

void TaskManager::addTask(Function function, int hz) {
	_function[counter] = function;
	_hz[counter] = (1000/hz);
	counter++;
}

void TaskManager::run() {

	for(int i=0;i<counter;i++) {

		uint32_t time = millis();
		if((time - _timer[i]) >= _hz[i]) {

			//printf2("inside");

			(*_function[i])();
			_timer[i] = time;
		}

	}

}
