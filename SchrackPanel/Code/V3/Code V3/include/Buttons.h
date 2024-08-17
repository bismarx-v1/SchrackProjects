#ifndef _BUTTONS
#define _BUTTONS
#include <Arduino.h>
#include "PinsAndConsts.h"

uint8_t Button1State = 1;	// default high because pullups
uint8_t Button2State = 1;
uint8_t Button3State = 1;
uint8_t Button4State = 1;
uint8_t Button1LastState = 1;
uint8_t Button2LastState = 1;
uint8_t Button3LastState = 1;
uint8_t Button4LastState = 1;

/**
 * Setsup buttons. just pinmodes
*/
void ButtonsSetup() {
	pinMode(PIN_Btn1, INPUT);
	pinMode(PIN_Btn2, INPUT);
	pinMode(PIN_Btn3, INPUT);
	pinMode(PIN_Btn4, INPUT);
}

/**
 * Periodically checks for button inputs and sets their flags
*/
void ButtonsLoop() {
	Button1State = digitalRead(PIN_Btn1);
	Button2State = digitalRead(PIN_Btn2);
	Button3State = digitalRead(PIN_Btn3);
	Button4State = digitalRead(PIN_Btn4);
	if(Button1State != Button1LastState) {
		//if(digitalRead(PIN_Btn1) == 0) {	// This was here before but it seems bit too useless
		#if CONST_ButtonsHavePullups == 0	// If buttons don't have pullups
			if(Button1State == 1) {
				Button1CheckedFlag = 1;
			}
		#else
			if(Button1State == 0) {
				Button1CheckedFlag = 1;
			}
		#endif

		Button1LastState = Button1State;
	}

	if(Button2State != Button2LastState) {
		//if(digitalRead(PIN_Btn2) == 0) {
		#if CONST_ButtonsHavePullups == 0	// If buttons don't have pullups
			if(Button2State == 1) {
				Button2CheckedFlag = 1;
			}
		#else
			if(Button2State == 0) {
				Button2CheckedFlag = 1;
			}
		#endif

		Button2LastState = Button2State;
	}
	
	if(Button3State != Button3LastState) {
		//if(digitalRead(PIN_Btn3) == 0) {
		#if CONST_ButtonsHavePullups == 0	// If buttons don't have pullups
			if(Button3State == 1) {
				Button3CheckedFlag = 1;
			}
		#else
			if(Button3State == 0) {
				Button3CheckedFlag = 1;
			}
		#endif

		Button3LastState = Button3State;
	}
	
	if(Button4State != Button4LastState) {
		//if(digitalRead(PIN_Btn4) == 0) {
		#if CONST_ButtonsHavePullups == 0	// If buttons don't have pullups
			if(Button4State == 1) {
				Button4CheckedFlag = 1;
			}
		#else
			if(Button4State == 0) {
				Button4CheckedFlag = 1;
			}
		#endif
		
		Button4LastState = Button4State;
	}

	vTaskDelay(ButtonsTimeSpace);	// Min button-flag-check delay
}

#endif