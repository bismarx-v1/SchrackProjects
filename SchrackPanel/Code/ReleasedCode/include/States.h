#ifndef _STATES
#define _STATES
#include <Arduino.h>
#include "PinsAndConsts.h"

/**
 * @brief This does nothing. its empty
*/
void StatesSetup() {
}

/**
 * @brief Reacts to button flags and sets the State var as described in the flowchart
 * @warning Remove the "button checked flag = 0" if the button press is needed elsewhere
*/
void StatesLoop() {
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
				//State = 90;	// Go to "The secret part"
				Button3CheckedFlag = 0;
			}

			if(Button4CheckedFlag == 1) {
				Button4CheckedFlag = 0;
			}

			#if CONST_ButtonsHavePullups == 0	// If buttons don't have pullups
				if(digitalRead(PIN_Btn2) == 1 && digitalRead(PIN_Btn3) == 1 && digitalRead(PIN_Btn4) == 1) {
					State = 90;	// Go to "The secret part"
					Button2CheckedFlag = 0;
					Button3CheckedFlag = 0;
					Button4CheckedFlag = 0;
				}
			#else
				if(digitalRead(PIN_Btn2) == 0 && digitalRead(PIN_Btn3) == 0 && digitalRead(PIN_Btn4) == 0) {
					State = 90;	// Go to "The secret part"
					Button2CheckedFlag = 0;
					Button3CheckedFlag = 0;
					Button4CheckedFlag = 0;
				}
			#endif

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
		
		case 90:	// State "The secret part"
			if(Button1CheckedFlag == 1) {
				State = 91;	// Go to "Credits"
				Button1CheckedFlag = 0;
			}

			if(Button2CheckedFlag == 1) {
				State = 92;	// Go to "Texts from dev team+"
				Button2CheckedFlag = 0;
			}

			if(Button3CheckedFlag == 1) {
				//State = 93;	// Go to "[Game name]"	// Game not yet included
				Button3CheckedFlag = 0;
			}

			if(Button4CheckedFlag == 1) {
				State = 3;	// Go to "Main menu"
				Button4CheckedFlag = 0;
			}

			break;

		case 91:	// State "Credits"
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
				State = 90;	// Go to "The secret part"
				Button4CheckedFlag = 0;
			}

			break;

		case 92:	// State "Texts from dev team+"
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
				State = 90;	// Go to "The secret part"
				Button4CheckedFlag = 0;
			}

			break;
		/*
		case 93:	// State "[Game name]"	// No game exists yet
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
				State = 90;	// Go to "The secret part"	// Go to "The secret part"
				Button4CheckedFlag = 0;
			}

			break;
		*/
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
				//State = 220;	// Go to "RTC"
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
				State = 213;	// Go to "Run countdown"
				Button1CheckedFlag = 0;
			}

			if(Button2CheckedFlag == 1) {
				// Internal "Cycle cursor"
				//Button2CheckedFlag = 0;
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
				State = 213;	// Go to "Run countdown"
				Button1CheckedFlag = 0;
			}

			if(Button2CheckedFlag == 1) {
				// Internal "Move cursor"
				//Button2CheckedFlag = 0;
			}

			if(Button3CheckedFlag == 1) {
				// Internal "Increase digit"
				//Button3CheckedFlag = 0;
			}

			if(Button4CheckedFlag == 1) {
				State = 210;	// Go to "Countdown"
				Button4CheckedFlag = 0;
			}

			break;
		
		case 213:	// State "Run countdown"
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
				State = 213;	// Go to "Run countdown"
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
		/*
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
		*/
		default:
			State = 0;	// Go to "Err screen"
			break;
	}

	//Serial.println(State);	// Debug print; prints the current state every loop
	vTaskDelay(ButtonsAntiSpamTimeSpace);	// Min state-change delay
}

#endif