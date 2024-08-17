#include <Arduino.h>
#include "PinsAndConsts.h"
#include "Driver.h"
#include "Graphics.h"


TaskHandle_t GraphicsCoreFuncHandle;
void GraphicsCoreFunc(void* parameter) {	// Graphics
	GraphicsSetup();
	String Text;
	#if 0
	Text = "33:33:33";
	GraphicsLoop(Frame, 1, 1, Text, 0);
	while(1) {
		taskYIELD();
	}

	#else
	//Text = "Pilsner Urquell. ";	// add a space after the text for a better text brake
	Text = "██████. ";	// add a space after the text for a better text brake
	while(1) {
		for(uint16_t i = 0; i < (Text.length()*6)+1; i++) {
			GraphicsLoop(Frame, 3, 1, Text, i);
			vTaskDelay(400);
		}

	}
	#endif
}

TaskHandle_t DriverCoreFuncHandle;
void DriverCoreFunc(void* parameter) {	// Driver
	DriverSetup();
	while(1) {
		DriverLoop(Frame);
	}
}

void setup() {
	// Graphics
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(GraphicsCoreFunc,	"GraphicsCoreFunc",	CONST_GraphicsCoreFuncStackSize,	NULL,					1,			&GraphicsCoreFuncHandle,	1);

	// Driver
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(DriverCoreFunc,		"DriverCoreFunc",	CONST_DriverCoreFuncStackSize,		NULL,					1,			&DriverCoreFuncHandle,		1);
}

void loop() {
	taskYIELD();
}