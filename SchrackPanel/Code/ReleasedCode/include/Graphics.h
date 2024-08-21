#ifndef _GRAPHICS
#define _GRAPHICS
#include <Arduino.h>
#include "PinsAndConsts.h"

uint8_t CharToCharIndex(char CTCI_Char) {
	if(CTCI_Char==32) {	// if ' '
		return(0);
	} else if((CTCI_Char>=48)&&(CTCI_Char<=57)) {	// if [Num]
		return(CTCI_Char-47);
	} else if((CTCI_Char>=65)&&(CTCI_Char<=90)) {	// if [UpperCase]
		return(CTCI_Char-54);
	} else if((CTCI_Char>=97)&&(CTCI_Char<=122)) {	// if [LowerCase]
		return(CTCI_Char-60);
	} else if(CTCI_Char==58) {	// if ':'
		return(63);
	} else if(CTCI_Char==45) {	// if '-'
		return(64);
	} else if(CTCI_Char==43) {	// if '+'
		return(65);
	} else if(CTCI_Char==42) {	// if '*'
		return(66);
	} else if(CTCI_Char==95) {	// if '_'
		return(67);
	} else if(CTCI_Char==46) {	// if '.'
		return(68);
	} else if(CTCI_Char==44) {	// if ','
		return(69);
	} else if(CTCI_Char==63) {	// if '?'
		return(70);
	} else if(CTCI_Char==33) {	// if '!'
		return(71);
	} else if(CTCI_Char==59) {	// if ';'
		return(72);
	} else if(CTCI_Char==38) {	// if '&', used for '█'
		return(73);
	} else if(CTCI_Char==47) {	// if '/'
		return(74);
	} else if(CTCI_Char==92) {	// if '\\'
		return(75);
	} else if(CTCI_Char==124) {	// if '|'
		return(76);
	} else if(CTCI_Char==40) {	// if '('
		return(77);
	} else if(CTCI_Char==41) {	// if ')'
		return(78);
	} else if(CTCI_Char==91) {	// if '['
		return(79);
	} else if(CTCI_Char==93) {	// if ']'
		return(80);
	} else if(CTCI_Char==123) {	// if '{'
		return(81);
	} else if(CTCI_Char==125) {	// if '}'
		return(82);
	} else if(CTCI_Char==60) {	// if '<'
		return(83);
	} else if(CTCI_Char==62) {	// if '>'
		return(84);
	} else if(CTCI_Char==61) {	// if '='
		return(85);
	} else if(CTCI_Char==36) {	// if '$', used for not equals
		return(86);
	} else if(CTCI_Char==35) {	// if '#'
		return(87);
	} else if(CTCI_Char==94) {	// if '^'
		return(88);
	} else if(CTCI_Char==126) {	// if '~'
		return(89);
	} else if(CTCI_Char==39) {	// if '''
		return(90);
	} else {
		return(0);
	}
}

/**
 * @brief Sets the red leds part of frame
 * @param Frmae frame to change
 * @param RedLeds value to set to red leds. 1 or 0
*/
void SetReds(uint8_t* Frame, uint8_t RedLeds) {
	for(uint8_t y = 0; y < CONST_Height; y++) {	// red leds stuff
		for(uint8_t x = 0; x < 2; x++) {
			Frame[y+(x+42)*CONST_Height] = RedLeds;
		}

		for(uint8_t x = 0; x < 4; x++) {
			Frame[y+(x+44)*CONST_Height] = 2;
		}
	}
}

//========================================================
uint8_t PrintToConsoleX = 0;
uint8_t PrintToConsoleY = 0;

/**
 * @brief Prints the frame to console
 * @param Frame Frame var
*/
void PrintToConsole(uint8_t* Frame) {
	Serial.print("#  ");	// Print top left corner marker
	for(PrintToConsoleX = 0; PrintToConsoleX < CONST_Width; PrintToConsoleX++) {	// Print horizontal pos marker
		Serial.printf("%02i|", PrintToConsoleX);
	}

	Serial.print("  #\n");	// Print top right corner marker
	
	for(PrintToConsoleY = 0; PrintToConsoleY < CONST_Height; PrintToConsoleY++) {	// Print the data
		Serial.printf("%02i|", PrintToConsoleY);	// Left vertical pos marker
		for(PrintToConsoleX = 0; PrintToConsoleX < CONST_Width; PrintToConsoleX++) {
			switch (Frame[PrintToConsoleY+PrintToConsoleX*CONST_Height]) {	// Print the pixel
				case 0:
					Serial.print("...");
					break;

				case 1:
					Serial.print("@@@");
					break;
				
				default:
					Serial.print("^^^");
					break;
			}
		}

		Serial.printf("|%02i\n", PrintToConsoleY);	// Right vertical pos marker
	}
	
	Serial.print("#  ");	// Print bot left corner marker
	for(PrintToConsoleX = 0; PrintToConsoleX < CONST_Width; PrintToConsoleX++) {	// Print horizontal pos marker
		Serial.printf("%02i|", PrintToConsoleX);
	}

	Serial.print("  #\n\n");	// Print bot right corner marker
}

//========================================================

/**
 * @brief Sets up frame
*/
void GraphicsSetup() {
	memset(Frame, 2, CONST_Pixels);
}

/**
 * @brief
 * @param Frame Frame array
 * @param DisplayMode 0 - test print --staic; 1 - time --HOffset; 2 - text --static; 3 - text --HOffset;
 * @param RedLeds whether to show the red highlight around the first letter
 * @param Text Text; for time pass hh:mm:ss
 * @param OffsetH n of col(s) to offset the text left
*/
void GraphicsLoop(uint8_t* Frame, uint8_t DisplayMode, uint8_t RedLeds, String Text, int32_t OffsetH) {
	memset(Frame, 2, CONST_Pixels);
	switch(DisplayMode) {
		case 0:	// Test print
			for(uint8_t y = 0; y < CONST_Height; y++) {
				for(uint8_t x = 0; x < CONST_Width-6; x++) {
					Frame[y+x*CONST_Height] = TestFrameHewwoCol3[y+x*CONST_Height];
				}
			}
			break;
		
		case 1:	// Time
			for(uint8_t FrameX = 0; FrameX < CONST_Width; FrameX++) {
				if((FrameX == 12 || FrameX == 16) || (FrameX == 30 || FrameX == 34)) {
					OffsetH += 2;
				}

				if((FrameX == 12 || FrameX == 13 || FrameX == 16 || FrameX == 17) || (FrameX == 30 || FrameX == 31 || FrameX == 34 || FrameX == 35)) {
					continue;
				}

				for(uint8_t FrameY = 0; FrameY < CONST_Height; FrameY++) {
					if(Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)] == 2) {
						Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)] = CharSet[(FrameY)+(FrameX%6*CONST_Height)+(CharToCharIndex(Text[(FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_Height*CONST_CharSegWidth)];
					}
				}

				// Debug prints
				//Serial.printf("FrameX=%02i,\tOffsetH=%02i\n", FrameX, OffsetH);
				//PrintToConsole(Frame);
			}
			break;
		
		case 2:	// Text
			for(uint8_t FrameX = 0; FrameX < CONST_Width-6; FrameX++) {
				for(uint8_t FrameY = 0; FrameY < CONST_Height; FrameY++) {
					//Serial.printf("Fy:%02i\tFx:%02i\tCy:%02i\tCx:%02i\tCi:%02i\tC:%i\n", FrameY, FrameX, FrameY, FrameX%5, (FrameX-FrameX%8)/8, CharSet[FrameY*5+FrameX%5]);	// Debug print for useful values
					if((FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth<Text.length()) {
						Frame[FrameY+FrameX*CONST_Height] = CharSet[(FrameX%CONST_CharSegWidth*CONST_Height)+(FrameY)+(CharToCharIndex(Text[(FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_CharSegWidth*CONST_Height)];
					}
				}
			}
			break;
		
		case 3:	// H offset text
			//OffsetH = OffsetH%342;
			//OffsetH = OffsetH%60;
			OffsetH = OffsetH%(Text.length()*6);	// Offset owerflow protection?

			for(uint16_t FrameX = 0; FrameX < Text.length()*6; FrameX++) {
				for(uint8_t FrameY = 0; FrameY < CONST_Height; FrameY++) {
					//Serial.printf("WriteTo: %03i,\tPrevVal: %i,\tVal: %i\n", (FrameY)+((FrameX-OffsetH)*CONST_Height), Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)], CharSet[(FrameY)+(FrameX%6*CONST_Height)+(CharToCharIndex(Text[(FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_Height*CONST_CharSegWidth)]);	// Debug print with useful values
					if((FrameY)+((FrameX-OffsetH)*CONST_Height) < CONST_Pixels) {
						if(Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)] == 2) {
							if((FrameY)+((FrameX-OffsetH)*CONST_Height) >= 0 && (FrameY)+((FrameX-OffsetH)*CONST_Height) < CONST_Pixels) {
								Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)] = CharSet[(FrameY)+(FrameX%6*CONST_Height)+(CharToCharIndex(Text[(FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_Height*CONST_CharSegWidth)];
							}
						}
					}
				}
			}

			/*Serial.print(OffsetH);
			Serial.print("\t");
			Serial.println(-(Text.length()*6 - OffsetH));*/
			/*											// This whole part seems useless?. we deffinetly had a reason for adding it, but nothing says why.
			OffsetH = -(Text.length()*6 - OffsetH);

			for(uint16_t FrameX = 0; FrameX < Text.length()*6; FrameX++) {
				for(uint8_t FrameY = 0; FrameY < CONST_Height; FrameY++) {
					//Serial.printf("WriteTo: %03i,\tPrevVal: %i,\tVal: %i\n", (FrameY)+((FrameX-OffsetH)*CONST_Height), Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)], CharSet[(FrameY)+(FrameX%6*CONST_Height)+(CharToCharIndex(Text[(FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_Height*CONST_CharSegWidth)]);
					if((FrameY)+((FrameX-OffsetH)*CONST_Height) < CONST_Pixels) {
						if(Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)] == 2) {
							Frame[(FrameY)+((FrameX-OffsetH)*CONST_Height)] = CharSet[(FrameY)+(FrameX%6*CONST_Height)+(CharToCharIndex(Text[(FrameX-FrameX%CONST_CharSegWidth)/CONST_CharSegWidth])*CONST_Height*CONST_CharSegWidth)];
						}
					}
				}
			}*/											// End of the seemingly useless part
			
			break;
		
		default:	// Error print
			for(uint8_t y = 0; y < CONST_Height; y++) {
				for(uint8_t x = 0; x < CONST_Width-CONST_CharSegWidth; x++) {
					Frame[y+x*CONST_Height] = TestFrameErr[y+x*CONST_Height];
				}
			}
			break;
	}

	SetReds(Frame, RedLeds);
}

#endif