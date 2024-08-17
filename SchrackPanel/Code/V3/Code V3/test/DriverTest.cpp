#include <Arduino.h>
#include "Driver.h"
#include "PinsAndConsts.h"


void setup() {
	for(uint16_t i = 0; i < 294; i++) {
		Frame[i] = TestFrameHewwoCol3[i];
	}

	DriverSetup();
}

void loop() {
	DriverLoop(Frame);
}