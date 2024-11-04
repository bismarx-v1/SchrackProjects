/**
 * This is the main program for the panel. I am sorry for anyone who has to fix this (excluding future me).
 * 
 * Here the program splits of into 4 separate tasks as seen bellow
 * Stuff changes a bit if the output is hardware of a console but only in the driver
 * 
 * Dunno what i made nor how i made it, but it tastes like spaghetti.
 * Chess Battle Advanced
*/

/**
 * TODO
 *  - Fix
 * 		- Add RTC																														(requires a wifi AP with a predefined name and pass)
 * 		- Add VW font support																											(requieres a whole rewrite)
 * 		- Check for all "// DEBUG"s
 * 		- Everything breaks when leaving(btn4) a menu with a long text(>7chars)
 * 
 * 	- Efficiency
 * 		- Try to slow down StateToFrame task																							(it's fine like this, and the esp is powerfull enough that i don't care now)
 * 		- Try to speed up Buttons task																									(not really needed, it's fast enough)
 * 
 * 	- Consistency
 * 		- Check how long can the program run, theory is 49.710269618055555days, 1193.0464708333334hours or 4294967295 millisseconds		(looking at the whole thing, We think the hardware won't stand uptime longer than a day)
 * 		- Test countdown accuracy																										(eh, good enough. no deviation We could measure in a 10min test)
*/

//  CCCC H   H EEEEE  CCCC K   K      OOO  U   U TTTTT PPPP  U   U TTTTT
// C     H   H E     C     K  K      O   O U   U   T   P   P U   U   T  
// C     H   H E     C     K K       O   O U   U   T   P   P U   U   T  
// C     HHHHH EEEEE C     KK        O   O U   U   T   PPPP  U   U   T  
// C     H   H E     C     K K       O   O U   U   T   P     U   U   T  
// C     H   H E     C     K  K      O   O U   U   T   P     U   U   T  
//  CCCC H   H EEEEE  CCCC K   K      OOO   UUU    T   P      UUU    T  

#define OUTPUT_IS_HARDWARE 1	// Switches between printing to panel or serial console @ 230400 b/s

#include <Arduino.h>
#include "PinsAndConsts.h"
#include "Graphics.h"
#include "Driver.h"
#include "Buttons.h"
#include "States.h"
#include "StateToFrame.h"


TaskHandle_t ButtonsCoreFuncHandle;
void ButtonsCoreFunc(void* parameter) {	// Buttons
	ButtonsSetup();
	while(1) {
		ButtonsLoop();
	}
}

TaskHandle_t StatesCoreFuncHandle;
void StatesCoreFunc(void* parameter) {	// States
	StatesSetup();
	while(1) {
		StatesLoop();
	}
}

#if OUTPUT_IS_HARDWARE	// printing to panel
TaskHandle_t GraphicsCoreFuncHandle;
void GraphicsCoreFunc(void* parameter) {	// Graphics
	StateToFrameSetup();
	while(1) {
		StateToFrameLoop();
	}
}

TaskHandle_t DriverCoreFuncHandle;
void DriverCoreFunc(void* parameter) {	// Driver
	DriverSetup();
	while(1) {
		DriverLoop(Frame);
	}
}

#else	// printing to console
TaskHandle_t GraphicsCoreFuncHandle;
void GraphicsCoreFunc(void* parameter) {	// Graphics
	StateToFrameSetup();
	delay(2000);
	while(1) {
		StateToFrameLoop();
	}
}

TaskHandle_t DriverCoreFuncHandle;
void DriverCoreFunc(void* parameter) {	// Driver
	//Serial.begin(115200);
	Serial.begin(230400);
	//Serial.begin(250000);
	delay(2000);
	Serial.println("BEGIN");
	while(1) {
		PrintToConsole(Frame);
		vTaskDelay(200);	// display refresh delay
	}
}

#endif



void setup() {
	// Buttons
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(ButtonsCoreFunc,	"ButtonsCoreFunc",	CONST_ButtonsCoreFuncStackSize,		NULL,					1,			&ButtonsCoreFuncHandle,		1);

	// States
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(StatesCoreFunc,		"StatesCoreFunc",	CONST_StatesCoreFuncStackSize,		NULL,					1,			&StatesCoreFuncHandle,		1);

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