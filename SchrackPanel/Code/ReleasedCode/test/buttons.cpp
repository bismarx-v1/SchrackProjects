#if 1

#include <Arduino.h>
#include "PinsAndConsts.h"
#include "Buttons.h"

TaskHandle_t ButtonsCoreFuncHandle;
void ButtonsCoreFunc(void* parameter) {	// Buttons
	ButtonsSetup();
	while(1) {
		ButtonsLoop();
	}
}

void setup() {
	Serial.begin(250000);

	// Buttons
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle			|Core
	xTaskCreatePinnedToCore(ButtonsCoreFunc,	"ButtonsCoreFunc",	CONST_ButtonsCoreFuncStackSize,		NULL,					1,			&ButtonsCoreFuncHandle,	1);

}

void loop() {
	if(Button1CheckedFlag == 1) {
		Serial.print("B 1: ");
		Serial.println(Button1State);
		Button1CheckedFlag = 0;
	}

	if(Button2CheckedFlag == 1) {
		Serial.print("B 2: ");
		Serial.println(Button2State);
		Button2CheckedFlag = 0;
	}
	
	if(Button3CheckedFlag == 1) {
		Serial.print("B 3: ");
		Serial.println(Button3State);
		Button3CheckedFlag = 0;
	}
	
	if(Button4CheckedFlag == 1) {
		Serial.print("B 4: ");
		Serial.println(Button4State);
		Button4CheckedFlag = 0;
	}

	vTaskDelay(90);
}

#endif