#ifndef _STATE_TO_FRAME_FUNCS
#define _STATE_TO_FRAME_FUNCS
#include <Arduino.h>
#include "Graphics.h"

uint16_t Tick2;	// Used in "Anim_Boot"

/**
 * @brief Sets pixel on position of Tick(left to right then top to bottom) to 1 if it isn't a part of text "SCHRACK"
 * @param Tick Stage of the anim. max is 335
 * @param Frame The frame to make the change to
*/
void BootAnim(uint16_t Tick, uint8_t* Frame) {
	if(Tick < 336) {
		//=(BG4-MOD(BG4,$C$4))/$C$4+$C$5*(MOD(BG4,$C$4))

		//Tick2 = (Tick-Tick%CONST_WidthReal)/CONST_WidthReal+CONST_Height*(Tick%CONST_WidthReal);
		//Tick2 = (Tick-Tick%CONST_Height)/CONST_Height+CONST_WidthReal*(Tick%CONST_Height);
		//Tick2 = (Tick-Tick%CONST_Height)/CONST_Height+CONST_Width*(Tick%CONST_Height);
		//Tick2 = Tick;
		//if(0 == CharSet[(((Tick2-Tick2%CONST_Height)/CONST_Height)%CONST_CharSegWidth*CONST_Height)+(Tick2%CONST_Height)+(CharToCharIndex(String("SCHRACK")[(((Tick2-Tick2%CONST_Height)/CONST_Height)-((Tick2-Tick2%CONST_Height)/CONST_Height)%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_CharSegWidth*CONST_Height)]) {
			//Frame[Tick2] = 1;
		//}
		//Frame[Tick2] = 1-TestFrameHewwoCol3[Tick2];
		//Frame[Tick2] = 1;
		// BOOT ANIM #1 SCRAPPED
		// MAKE SOMETHING SIMPLER
		// <3 -Nat

		//Tick2 = (Tick-Tick%$C$4)/$C$4+$C$5*(Tick%$C$4)
		//FrameX = ((Tick-Tick%CONST_Height)/CONST_Height)
		//FrameY = (Tick%CONST_Height)

		if(0 == CharSet[(((FlipMatrix(Tick, CONST_Width, CONST_Height)-FlipMatrix(Tick, CONST_Width, CONST_Height)%CONST_Height)/CONST_Height)%CONST_CharSegWidth*CONST_Height)+(FlipMatrix(Tick, CONST_Width, CONST_Height)%CONST_Height)+(CharToCharIndex(String("SCHRACK")[(((FlipMatrix(Tick, CONST_Width, CONST_Height)-FlipMatrix(Tick, CONST_Width, CONST_Height)%CONST_Height)/CONST_Height)-((FlipMatrix(Tick, CONST_Width, CONST_Height)-FlipMatrix(Tick, CONST_Width, CONST_Height)%CONST_Height)/CONST_Height)%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_CharSegWidth*CONST_Height)]) {
			Frame[FlipMatrix(Tick, CONST_Width, CONST_Height)] = 1;
		}
	}
}

uint8_t LongText;	// If the text is longer than 7 chars
uint8_t NewTextLength;

/**
 * @brief This func should provide a transition anim between 2 texts
 * @param OldText Text you go from
 * @param NewText Text you go to
 * @param Tick Variable with the offset
 * @returns 0 when finished, else 1
*/
uint8_t TextTransAnim(uint8_t* Frame, String OldText, String NewText, uint16_t* Tick) {
	//GraphicsLoop(Frame, 3, 0, OldText+". "+OldText+". "+NewText, Tick[0]);	// Render text
	if(OldText[OldText.length()-1] != ' ') {
		if(7-OldText.length() > 0) {
			LongText = 0;
			NewTextLength = 0;
			for(uint8_t i = 0; i < 7-OldText.length()+1; i++) {
				OldText = OldText + " ";
			}
		}
	}

	if(NewText.length() > 7) {
		LongText = 1;
		NewTextLength = NewText.length();
		NewText = NewText + ". " + NewText;
	}

	OldText = OldText + " >";
	GraphicsLoop(Frame, 3, 0, OldText+NewText, Tick[0]);	// Render text
	// if(Tick[0]+1 >= (((OldText+". "+OldText+". ").length())*6)) {	// If the next offset is 0 for the new text
	if(Tick[0]+1 >= ((OldText.length())*6+1) + LongText*(NewTextLength+2)*6+1) {	// If the next offset is 0 for the new text
		return(0);
	} else {
		Tick[0]++;
		return(1);
	}
}

/**
 * @brief Renders continuation a piece of text ariving from the right
 * @param Frame The frame to make the change to
 * @param Text Input text
 * @param EndHighLight Decides if the highlight should be on at the end
 * @param Tick Stage of the anim
 * @returns Value of next tick
*/
uint16_t TextEnterAnim(uint8_t* Frame, String Text, uint8_t EndHighlight, uint16_t Tick) {
	if(Text.length() <= 7) {	// Text is short, it just arrives
		if(TextTransAnim(Frame, PreviousText, Text, &Tick)) {
			//TextTransAnim(Frame, PreviousText, Text, &Tick);
			//GraphicsLoop(Frame, 3, 0, "       "+Text, Tick);
			//return(Tick+1);
			return(Tick);
		} else {
			GraphicsLoop(Frame, 2, EndHighlight, Text, 0);
			return(65535);
		}
	} else {	// Texts is long, it passes through once
		/*if(Tick < (Text.length()+7+2)*6) {
			GraphicsLoop(Frame, 3, 0, "       "+Text+". "+Text, Tick);
			return(Tick+1);
		}*/
		if(TextTransAnim(Frame, PreviousText, Text, &Tick)) {
			return(Tick);
		}else {
			GraphicsLoop(Frame, 2, EndHighlight, Text, 0);
			return(65535);
		}
	}
}

String SetText(const String* TextsArray, uint8_t CycleTextsIndex) {
	if(TextsArray[CycleTextsIndex].length() > 2) {
		return(TextsArray[CycleTextsIndex]+". "+TextsArray[CycleTextsIndex]+". ");
	} else if(TextsArray[CycleTextsIndex].length() > 1) {
		return(TextsArray[CycleTextsIndex]+". "+TextsArray[CycleTextsIndex]+". "+TextsArray[CycleTextsIndex]+". ");
	} else {
		return(TextsArray[CycleTextsIndex]+". "+TextsArray[CycleTextsIndex]+". "+TextsArray[CycleTextsIndex]+". "+TextsArray[CycleTextsIndex]+". ");
	}
}

uint8_t CycleTextsIndexInTextArray = 0;
uint16_t CycleTextsOffset = 0;
uint32_t CycleTextsLast = 0;
String Text = "";

/**
 * @brief 
 * @param TextsArray Array of texts to display
 * @param TextsArrayLength Length of the array
 * @param TextChangePeriod Array with times how long should each text stay
*/
void CycleTextsReset(const String* TextsArray, String* Text) {
	Text[0] = SetText(TextsArray, 0);
}

/**
 * @brief Cycles inputed texts
 * @param TextsArray Array of texts to display
 * @param TextsArrayLength Length of the array
 * @param TextChangePeriod Array with times how long should each text stay
*/
void CycleTexts(const String* TextsArray, uint8_t TextsArrayLength, const uint16_t* TextChangePeriod, uint8_t* IndexInTextArray, uint32_t* TextChangedLast, uint16_t* CycleTextsOffset, String* Text) {
	//Serial.print(Text[0]); Serial.print("; "); Serial.print(IndexInTextArray[0]);  Serial.print("; "); Serial.print(CycleTextsOffset[0]); Serial.print("; "); Serial.print((TextsArray[IndexInTextArray[0]].length()+2)*6); Serial.print("; "); Serial.print(TextChangedLast[0]+TextChangePeriod[IndexInTextArray[0]]); Serial.print("; "); Serial.println(millis());	// Debug print
	GraphicsLoop(Frame, 3, 0, Text[0], CycleTextsOffset[0]);
	CurrentText = "       ";

	if(TextChangedLast[0]+TextChangePeriod[IndexInTextArray[0]] < millis()) {
		TextChangedLast[0] = millis();
		CycleTextsOffset[0] = 0;

		if(IndexInTextArray[0]+1 >= TextsArrayLength) {
			IndexInTextArray[0] = 0;
		} else {
			IndexInTextArray[0]++;
		}

		Text[0] = SetText(TextsArray, IndexInTextArray[0]);

	} else {
		if(CycleTextsOffset[0]+1 >= (TextsArray[IndexInTextArray[0]].length()+2)*6) {
			CycleTextsOffset[0] = 0;
		} else {
			CycleTextsOffset[0]++;
		}
	}
}

uint8_t IndexInPredefinedTimeArray = 0;	// Index in the "predefined time" array to display
uint8_t IndexInPredefinedTimeArrayLast = 1;	// Last time displayed; used for optimalization (not drawing the same text over and over)

/**
 * @brief Display and set "CounterRemTime" time based on a predefined list; displayed time changes with a button flag
 * @param ButtonCheckedFlag Button that will change the time
*/
void SetTimePredefined(uint8_t* ButtonCheckedFlag) {
	if(ButtonCheckedFlag[0] == 1) {	// If button pressed
		if(IndexInPredefinedTimeArray+1 >= PredefinedTimeToCycleLength) {	// If the index will owerflow
			IndexInPredefinedTimeArray = 0;	// Reset
		} else {
			IndexInPredefinedTimeArray++;	// Move to next time
		}

		ButtonCheckedFlag[0] = 0;	// Reset the flag
	}
	
	if(IndexInPredefinedTimeArray != IndexInPredefinedTimeArrayLast) {	// If time changed, update Frame
		CounterRemTime = PredefinedTimeToCycle[IndexInPredefinedTimeArray];	// Set remaining time to the desired time
		GraphicsLoop(Frame, 1, 0, CounterRemTime, 0);	// Display the time
		CurrentText = "       ";
		IndexInPredefinedTimeArrayLast = IndexInPredefinedTimeArray;
	}
}

uint32_t CountdownPauseAnimToggleLast = 0;	// Last time the anim changed
const uint16_t CountdownPauseAnimToggleTimeArray[2] = {20000, 10000};	// How long before anim change

/**
 * @brief A small anim for when the countdown is paused
 * @param Tick Stage of the anim
 * @returns Value to set Tick to next
*/
uint16_t CountdownPause(uint16_t Tick) {
	if(Tick == 0) {
		GraphicsLoop(Frame, 1, 0, CounterRemTime, 0);	// Remaining time
	} else if(Tick == 1) {
		GraphicsLoop(Frame, 1, 0, "Pa:us:ed", 0);	// "Paused" text
	}

	CurrentText = "       ";

	if(CountdownPauseAnimToggleLast + CountdownPauseAnimToggleTimeArray[Tick] < millis()) {	// If time to change to new text
		Tick = 1-Tick;
	}

	return(Tick);
}

uint8_t BzrState = 0;

/**
 * @brief A small anim for when the countdown ends; turns on tthe buzzer for 5secs
 * @param Tick Stage of the anim
 * @param TimeTriggred Time that the buzzer will be based on
 * @returns Value to set Tick to next
*/
uint16_t CountdownEnd(uint16_t Tick, uint32_t TimeTriggred) {
	if(Tick == 0) {
		GraphicsLoop(Frame, 1, 0, CounterRemTime, 0);	// colons on
	} else if(Tick == 1) {
		GraphicsLoop(Frame, 1, 1, "00 00 00", 0);	// colons off
		//GraphicsLoop(Frame, 1, 0, String(CounterRemTime[0] + CounterRemTime[1] + String(" ") + CounterRemTime[4] + CounterRemTime[5] + String(" ") + CounterRemTime[7] + CounterRemTime[8]), 0);	// colons off; for any end time
	}

	CurrentText = "       ";

	if(CountdownPauseAnimToggleLast + CountdownPauseAnimToggleTimeArray[Tick] < millis()) {	// If time to change to new text
		Tick = 1-Tick;
	}

	if(millis() < TimeTriggred + 5000) {
		if(BzrState != 1) {
			#if BUZZER_ENABLED
			digitalWrite(PIN_Bzr, 1);
			#endif
			BzrState = 1;
		}
	} else {
		if(BzrState != 0) {
			#if BUZZER_ENABLED
			digitalWrite(PIN_Bzr, 0);
			#endif
			BzrState = 0;
		}
	}

	return(Tick);
}

uint32_t CounterRemTimeLast = 0; // Last time the time decreased

/**
 * @brief This function should decrese time by 1
 * @param Time Time variable
 * @warning "00:00:00" will overflow into "99:59:59"
*/
String TimeDecrease(String Time) {		// Frst method i thought of. enjoy
	if(Time[7] > '0') {				// Check if ##:##:#N can be decreased
		Time[7] = Time[7] - 1;		// It can, decrease it
	} else {						// It can't
		Time[7] = '9';				// Set it back to max (9)
		if(Time[6] > '0') {				// Check if ##:##:N# can be decreased
			Time[6] = Time[6] - 1;		// It can, decrease it
		} else {						// It can't
			Time[6] = '5';				// Set it back to max (5)
			if(Time[4] > '0') {				// Check if ##:#N:## can be decreased
				Time[4] = Time[4] - 1;		// It can, decrease it
			} else {						// It can't
				Time[4] = '9';				// Set it back to max (9)
				if(Time[3] > '0') {				// Check if ##:N#:## can be decreased
					Time[3] = Time[3] - 1;		// It can, decrease it
				} else {						// It can't
					Time[3] = '5';				// Set it back to max (5)
					if(Time[1] > '0') {				// Check if #N:##:## can be decreased
						Time[1] = Time[1] - 1;		// It can, decrease it
					} else {						// It can't
						Time[1] = '9';				// Set it back to max (9)
						if(Time[0] > '0') {				// Check if N#:##:## can be decreased
							Time[0] = Time[0] - 1;		// It can, decrease it
						} else {						// It can't
							Time[0] = '9';				// Set it back to max (9) anyway.
						}
					}
				}
			}
		}
	}

	return(Time);
}

/**
 * @brief Checks for "wrong" rem times
*/
void CountdownRunFirstCheck() {
	if(CounterRemTime == String("00:00:00")) {
		State = 214;
	} else if(CounterRemTime == String("99:99:98")) {
		State = 0;
	}
}

uint32_t CounterRemTimeLastLast = 0;
uint8_t CountdownRunForLoopI1 = 0;

/**
 * @brief Displays remaining time each second and decreases it
 * @returns Value to set Tick to next
*/
uint16_t CountdownRun() {
	GraphicsLoop(Frame, 1, 0, CounterRemTime, 0);
	CurrentText = "       ";
	if(CounterRemTimeLast + 1000 <= millis()) {
		CounterRemTimeLastLast = CounterRemTimeLast;
		CounterRemTimeLast = millis();
		for(CountdownRunForLoopI1 = 0; CountdownRunForLoopI1 < round(CounterRemTimeLast/1000) - round(CounterRemTimeLastLast/1000); CountdownRunForLoopI1++) {
			CounterRemTime = TimeDecrease(CounterRemTime);
			if(CounterRemTime == String("00:00:00")) {
				return(65535);
			}
		}
	}

	return(0);
}

uint8_t CursorIndex = 7;
uint8_t SetTimeCustomChangedFlag = 1;
const uint8_t CursorOffset[8] = {0, 42, 0, 98, 140, 0, 196, 238};
uint8_t SetTimeCustomForLoopI1 = 0;
uint8_t SetTimeCustomToggleCursorHighlight = 0;
uint32_t SetTimeCustomToggleCursorHighlightChangedLast = 0;

/**
 * 
*/
void SetTimeCustom(uint8_t* MoveCursorFlagPointer, uint8_t* CycleDigitFlagPointer) {
	if(SetTimeCustomChangedFlag = 1) {
		SetTimeCustomChangedFlag = 0;
		GraphicsLoop(Frame, 1, 0, CounterRemTime, 0);
		CurrentText = "       ";
		if(SetTimeCustomToggleCursorHighlight == 1) {
			for(SetTimeCustomForLoopI1 = 0; SetTimeCustomForLoopI1 < 42; SetTimeCustomForLoopI1++) {
				Frame[CursorOffset[CursorIndex]+SetTimeCustomForLoopI1] = 1-Frame[CursorOffset[CursorIndex]+SetTimeCustomForLoopI1];
			}

			if(CursorIndex != 0) {
				for(SetTimeCustomForLoopI1 = 0; SetTimeCustomForLoopI1 < 7; SetTimeCustomForLoopI1++) {
					Frame[CursorOffset[CursorIndex]+SetTimeCustomForLoopI1-7] = 1-Frame[CursorOffset[CursorIndex]+SetTimeCustomForLoopI1-7];
				}
			}
		}
	}

	if(SetTimeCustomToggleCursorHighlightChangedLast+500 <= millis()) {
		SetTimeCustomToggleCursorHighlightChangedLast = millis();
		SetTimeCustomToggleCursorHighlight = 1-SetTimeCustomToggleCursorHighlight;
	}

	if(MoveCursorFlagPointer[0] == 1) {
		/*// This moves the cursor the other way(right)
		if(CursorIndex == 0 || CursorIndex == 3 || CursorIndex == 5 || CursorIndex == 6) {
			CursorIndex++;
		} else if(CursorIndex == 1 || CursorIndex == 4) {
			CursorIndex = CursorIndex+2;
		} else {
			CursorIndex = 0;
		}*/
		// This moves the cursor left
		if(CursorIndex == 7 || CursorIndex == 4 || CursorIndex == 1) {
			CursorIndex--;
		} else if(CursorIndex == 6 || CursorIndex == 3) {
			CursorIndex = CursorIndex-2;
		} else {
			CursorIndex = 7;
		}
		
		MoveCursorFlagPointer[0] = 0;
	}

	if(CycleDigitFlagPointer[0] == 1) {
		SetTimeCustomChangedFlag = 1;
		if(CounterRemTime[CursorIndex] >= '9') {
			CounterRemTime[CursorIndex] = '0';
		} else {
			CounterRemTime[CursorIndex] = CounterRemTime[CursorIndex]+1;
		}

		CycleDigitFlagPointer[0] = 0;
	}

}


#endif