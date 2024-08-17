#if 0

#include <Arduino.h>
#include "PinsAndConsts.h"


uint8_t Button1State = 0;
uint8_t Button2State = 0;
uint8_t Button3State = 0;
uint8_t Button4State = 0;
uint8_t Button1LastState = 0;
uint8_t Button2LastState = 0;
uint8_t Button3LastState = 0;
uint8_t Button4LastState = 0;
uint8_t Button1CheckedFlag = 0;
uint8_t Button2CheckedFlag = 0;
uint8_t Button3CheckedFlag = 0;
uint8_t Button4CheckedFlag = 0;

TaskHandle_t ButtonsCoreFuncHandle;
void ButtonsCoreFunc(void* parameter) {	// Buttons
	pinMode(PIN_Btn1, INPUT);
	pinMode(PIN_Btn2, INPUT);
	pinMode(PIN_Btn3, INPUT);
	pinMode(PIN_Btn4, INPUT);
	while(1) {
		Button1State = digitalRead(PIN_Btn1);
		Button2State = digitalRead(PIN_Btn2);
		Button3State = digitalRead(PIN_Btn3);
		Button4State = digitalRead(PIN_Btn4);
		if(Button1State != Button1LastState) {
			if(digitalRead(PIN_Btn1) == 0) {
				Button1CheckedFlag = 1;
			}

			Button1LastState = Button1State;
		}

		if(Button2State != Button2LastState) {
			if(digitalRead(PIN_Btn2) == 0) {
				Button2CheckedFlag = 1;
			}
			Button2LastState = Button2State;
		}
		
		if(Button3State != Button3LastState) {
			if(digitalRead(PIN_Btn3) == 0) {
				Button3CheckedFlag = 1;
			}
			Button3LastState = Button3State;
		}
		
		if(Button4State != Button4LastState) {
			if(digitalRead(PIN_Btn4) == 0) {
				Button4CheckedFlag = 1;
			}
			Button4LastState = Button4State;
		}

		vTaskDelay(ButtonsTimeSpace);	// Min button-flag-check delay
	}
}

TaskHandle_t StatesCoreFuncHandle;
void StatesCoreFunc(void* parameter) {	// States
	vTaskDelay(1000);
	while(1) {
		switch(State) {
			case 0:	// State "Err screen"
				if(Button1CheckedFlag == 1) {
					State = 2;	// Go to "Schrack logo"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					Button4CheckedFlag = 0;
				}

				break;
			
			
			case 1:	// State "Bootup anim"
				if(Button1CheckedFlag == 1) {
					State = 2;	//DEBUG
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					Button4CheckedFlag = 0;
				}

				break;

			case 2:	// State "Schrack logo"
				if(Button1CheckedFlag == 1) {
					State = 3;	// Go to "Main memu"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					Button4CheckedFlag = 0;
				}

				break;

			case 3:	// State "Main menu"
				if(Button1CheckedFlag == 1) {
					State = 100;	// Go to "Text cycle"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					State = 200;	// Go to "Time"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					Button4CheckedFlag = 0;
				}

				break;

			case 100:	// State "Text cycle"
				if(Button1CheckedFlag == 1) {
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 3;	// Go to "Main menu"
					Button4CheckedFlag = 0;
				}

				break;

			case 200:	// State "Time"
				if(Button1CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					State = 220;	// Go to "RTC"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 3;	// Go to "Main menu"
					Button4CheckedFlag = 0;
				}

				break;

			case 210:	// State "Countdown"
				if(Button1CheckedFlag == 1) {
					State = 211;	// Go to "Predefined"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					State = 212;	// Go to "Custom"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 200;	// Go to "Time"
					Button4CheckedFlag = 0;
				}

				break;

			case 211:	// State "Predefined"
				if(Button1CheckedFlag == 1) {
					State = 213;	// Go to "Start countdown"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					// Internal "Cycle cursor"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button4CheckedFlag = 0;
				}

				break;

			case 212:	// State "Custom"
				if(Button1CheckedFlag == 1) {
					State = 213;	// Go to "Start countdown"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					// Internal "Move cursor"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					// Internal "Increase digit"
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button4CheckedFlag = 0;
				}

				break;

			case 213:	// State "Start countdown"
				if(Button1CheckedFlag == 1) {
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					State = 215;	// Go to "Pause countdown"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button4CheckedFlag = 0;
				}

				break;

			case 214:	// State "Countdown ended"
				if(Button1CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button4CheckedFlag = 0;
				}

				break;

			case 215:	// State "Pause countdown"
				if(Button1CheckedFlag == 1) {
					State = 213;	// Go to "Start countdown"
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 210;	// Go to "Countdown"
					Button4CheckedFlag = 0;
				}

				break;

			case 220:	// State "RTC"
				if(Button1CheckedFlag == 1) {
					Button1CheckedFlag = 0;
				}

				if(Button2CheckedFlag == 1) {
					Button2CheckedFlag = 0;
				}

				if(Button3CheckedFlag == 1) {
					Button3CheckedFlag = 0;
				}

				if(Button4CheckedFlag == 1) {
					State = 200;	// Go to "Time"
					Button4CheckedFlag = 0;
				}

				break;
			
			default:
				State = 0;	// Go to "Err screen"
				break;
		}

		Serial.println(State);
		vTaskDelay(ButtonsAntiSpamTimeSpace);	// Min state-change delay
	}
	
}


void setup() {
	//Serial.begin(250000);
	Serial.begin(115200);

	// Buttons
	// Driver task			|Task func			|Name				|Stack size	|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(ButtonsCoreFunc,	"ButtonsCoreFunc",	1000,		NULL,					1,			&ButtonsCoreFuncHandle,		1);

	// States
	// Driver task			|Task func			|Name				|Stack size	|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(StatesCoreFunc,		"StatesCoreFunc",	1000,		NULL,					1,			&StatesCoreFuncHandle,		1);

}

void loop() {
	taskYIELD();
}

#endif

//=====================================================

#if 1

#include <Arduino.h>
#include "PinsAndConsts.h"
#include "States.h"
#include "Buttons.h"

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


void setup() {
	Serial.begin(250000);

	// Buttons
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(ButtonsCoreFunc,	"ButtonsCoreFunc",	CONST_ButtonsCoreFuncStackSize,		NULL,					1,			&ButtonsCoreFuncHandle,		1);

	// States
	// Driver task			|Task func			|Name				|Stack size							|Parameter of the task	|Priority	|Task handle				|Core
	xTaskCreatePinnedToCore(StatesCoreFunc,		"StatesCoreFunc",	CONST_StatesCoreFuncStackSize,		NULL,					1,			&StatesCoreFuncHandle,		1);

}

void loop() {
	taskYIELD();
}

#endif