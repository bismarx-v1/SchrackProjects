#ifndef _STATE_TO_FRAME
#define _STATE_TO_FRAME
#include <Arduino.h>
#include "PinsAndConsts.h"
#include "Graphics.h"
#include "STFFuncs.h"

uint16_t StateLast = 65535;
uint8_t RedLedState = 0;	// State of the red leds for ex. err screen
uint32_t TrigLast = 0;	// Time of last toggle
uint16_t StateTick = 0;	// Max val = 65535
uint32_t CtdEndTrigTime = 0;	// Time the "countdown end" was triggered

/**
 * @brief Setup for StateToFrame and Graphics
*/
void StateToFrameSetup() {
	GraphicsSetup();
}

uint16_t i;	// x3 this works. i used this because i don't think the esp would release the memory after the for and would creae a memory leak
			// this is really stupid put i'll leave it. it works

/**
 * @brief Uses a global var called State to decide what to render
*/
void StateToFrameLoop() {
	switch(State) {
	case 0:	// State "Err screen"
		if(State != StateLast) {
			StateLast = State;
			RedLedState = 1;	// Works as a toggle for red leds
			TrigLast = millis()-1000;
			for(i = 0; i < 294; i++) {
				Frame[i] = TestFrameErr[i];
			}
		}


		if(TrigLast+1000 < millis()) {
			TrigLast = millis();
			SetReds(Frame, RedLedState);
			RedLedState = 1-RedLedState;
		}
	
		break;

	case 1:	// State "Bootup anim"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			memset(Frame, 0, CONST_Pixels);	// Clear the display
		}

		if(StateTick < 65535) {

			BootAnim(StateTick, Frame);
			BootAnimIncreaseTickLable1:
			StateTick++;
			if(FlipMatrix(StateTick, CONST_Width, CONST_Height) >= 294) {
				goto BootAnimIncreaseTickLable1;
			}

			BootAnim(StateTick, Frame);
			BootAnimIncreaseTickLable2:
			StateTick++;
			if(FlipMatrix(StateTick, CONST_Width, CONST_Height) >= 294) {
				goto BootAnimIncreaseTickLable2;
			}

			if(StateTick >= 336) {
				StateTick = 65535;
			}

		} else {
			vTaskDelay(1000);	// Hold the last frame longer
			State = 2;
			StateTick = 0;
		}
		
		break;

	case 2:	// State "Schrack logo"
		if(State != StateLast) {
			StateLast = State;
			CurrentText = "SCHRACK";
			GraphicsLoop(Frame, 2, 1, "SCHRACK", 0);
		}

		break;

	case 3:	// State "Main menu"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			PreviousText = CurrentText;
			CurrentText = TEXT_MainMenu;
		}

		if(StateTick < 65535) {
			StateTick = TextEnterAnim(Frame, CurrentText, 0, StateTick);
		}
		
		break;

	case 90:	// State "The secret part"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			PreviousText = CurrentText;
			CurrentText = TEXT_ScrtPt;
		}

		if(StateTick < 65535) {
			StateTick = TextEnterAnim(Frame, TEXT_ScrtPt, 0, StateTick);
		}
		
		break;

	case 91:	// State "Credits"
		if(State != StateLast) {
			StateLast = State;
			CycleTextsIndexInTextArray = 0;		// Reset; from STFF
			CycleTextsOffset = 0;		// Reset; from STFF
			CycleTextsLast = millis();	// Reset; from STFF
			CycleTextsReset(CreditText, &Text);
		}

		CycleTexts(CreditText, CreditTextLength, CreditTextPeriod, &CycleTextsIndexInTextArray, &CycleTextsLast, &CycleTextsOffset, &Text);

		break;

	case 92:	// State "Texts from dev team+"
		if(State != StateLast) {
			StateLast = State;
			CycleTextsIndexInTextArray = 0;		// Reset; from STFF
			CycleTextsOffset = 0;		// Reset; from STFF
			CycleTextsLast = millis();	// Reset; from STFF
			CycleTextsReset(DevText, &Text);
		}

		CycleTexts(DevText, DevTextLength, DevTextPeriod, &CycleTextsIndexInTextArray, &CycleTextsLast, &CycleTextsOffset, &Text);

		break;

	case 93:	// State "[Game name]"	// not yet added
		if(State != StateLast) {
			StateLast = State;
			
		}




		break;
	
	case 100:	// State "Text cycle"
		if(State != StateLast) {
			StateLast = State;
			CycleTextsIndexInTextArray = 0;		// Reset; from STFF
			CycleTextsOffset = 0;		// Reset; from STFF
			CycleTextsLast = millis();	// Reset; from STFF
			CycleTexts2Reset(TextsToCycle, TextsToCycleLength);	// DEBUG/TEST
			//CycleTextsReset(TextsToCycle, &Text);		// DEBUG/Backup plan
		}

		CycleTexts2(TextsToCycle, TextsToCycleLength);	// DEBUG/TEST
		//CycleTexts(TextsToCycle, TextsToCycleLength, CycleTextChangePeriod, &CycleTextsIndexInTextArray, &CycleTextsLast, &CycleTextsOffset, &Text);		// DEBUG/Backup plan

		break;
	
	case 200:	// State "Time"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			PreviousText = CurrentText;
			CurrentText = TEXT_Time;
		}

		if(StateTick < 65535) {
			StateTick = TextEnterAnim(Frame, TEXT_Time, 0, StateTick);
		}
		
		break;
	
	case 210:	// State "Countdown"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			PreviousText = CurrentText;
			CurrentText = TEXT_Countdown;
		}

		if(StateTick < 65535) {
			StateTick = TextEnterAnim(Frame, TEXT_Countdown, 0, StateTick);
		}
		
		break;
	
	case 211:	// State "Predefined"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			IndexInPredefinedTimeArray = 0;	// reset; from STFF
			IndexInPredefinedTimeArrayLast = 1;	// reset; from STFF
		}

		SetTimePredefined(&Button2CheckedFlag);	// Button2 cycles time
		break;
	
	case 212:	// State "Custom"
		if(State != StateLast) {
			StateLast = State;
			CursorIndex = 7;	// Reset; from StFF
			SetTimeCustomChangedFlag = 1;	// Reset; from StFF
			CounterRemTime = "00:00:00";	// Reset
		}

		SetTimeCustom(&Button2CheckedFlag, &Button3CheckedFlag);
		break;

	case 213:	// State "Run countdown"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			CounterRemTimeLast = millis();
			CountdownRunFirstCheck();
		}

		StateTick = CountdownRun();
		if(StateTick == 65535) {
			State = 214;
			StateTick = 0;
		}

		break;

	case 214:	// State "End countdown"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
			CtdEndTrigTime = millis();
		}

		if(StateTick < 65535) {
			StateTick = CountdownEnd(StateTick, CtdEndTrigTime);
		}

		break;

	case 215:	// State "Pause countdown"
		if(State != StateLast) {
			StateLast = State;
			StateTick = 0;
		}

		if(StateTick < 65535) {
			StateTick = CountdownPause(StateTick);
		}

		break;

	/*case 220:	// State "RTC"	// not yet added
		if(State != StateLast) {
			StateLast = State;
		}

		break;*/
	
	default:
		State = 0;
		break;
	}

	vTaskDelay(STFPeriod);
}

#endif