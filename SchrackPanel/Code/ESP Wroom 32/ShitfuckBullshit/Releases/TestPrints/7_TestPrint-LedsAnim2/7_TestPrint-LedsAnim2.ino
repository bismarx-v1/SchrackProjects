/*
* Led anim from top left to bot right then the same way but off
*/

#include "Driver.h"
#include "GraphicsLedLoad.h"

/***************\
* TestFrames	*
\***************/
uint8_t Frame[336];

/***********\
* Functions	*
\***********/

/*******\
* Main	*
\*******/

TaskHandle_t Task1;
TaskHandle_t Task2;

/***************\
* Core0 code	*	// <Graphics>
\***************/
void codeForTask1( void * parameter) {
	Graphics_Setup();
	for(;;) {	// <Task1 loop>
		Graphics_Loop(Frame);
		vTaskDelay(1000);
	};			// </Task1 loop>
}
/***************\
* /Core0 code	*	// </Graphics>
\***************/

/***************\
* Core1 code	*	// <Driver>
\***************/
void codeForTask2( void * parameter) {
	Driver_Setup();
	for(;;) {	// <Task2 loop>
		Driver_PrintFrame(Frame);
		vTaskDelay(0.01);	// Delay between frames
	};			// </Task2 loop>
}
/***************\
* /Core1 code	*	// </Driver>
\***************/

void setup() {
	Serial.begin(115200);
	
	
	/*
	* Pin Task1 to core
	*/
	xTaskCreatePinnedToCore(
		codeForTask1,	/* Task func*/
		"Task_1",		/* Name*/
		8000,			/* Stack size*/
		NULL,			/* Parameter of the task*/
		1,				/* Priority*/
		&Task1,			/* Task handle*/
		0				/* Core*/
	);
	
	/*
	* Pin Task2 to core
	*/
	xTaskCreatePinnedToCore(
		codeForTask2,	/* Task func*/
		"Task_2",		/* Name*/
		8000,			/* Stack size*/
		NULL,			/* Parameter of the task*/
		1,				/* Priority*/
		&Task2,			/* Task handle*/
		1				/* Core*/
	);
}

void loop() {
	//Serial.printf("MainLoop: %i\n", xPortGetCoreID());
	delay(100);
}
