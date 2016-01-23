

#include "quad/Quadcopter.h"

int main(void) {

	//init
	initDelay();
	initUSART1(9600);

	initQuad();

	while (1) {
		loop();
	}
}
