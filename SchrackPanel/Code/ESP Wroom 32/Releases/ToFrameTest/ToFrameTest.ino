#include "LibFilepathFix.h"

uint8_t Frame[336];

// <PrintToConsole>
void PrintToConsole(uint8_t* PTC_Frame, uint16_t PTC_SizeOfFrame) {
	for(uint8_t y = 0; y <= 7-1; y++) {
		Serial.printf("%02i|", y);
		for(uint8_t x = 0; x <= (PTC_SizeOfFrame/7)-1; x++) {
			uint8_t PTC_Pixel = PTC_Frame[y*(PTC_SizeOfFrame/7)+x];
			switch(PTC_Pixel) {
				case 1:
					Serial.printf("@@@");
					break;
				case 0:
					Serial.printf("^^^");
					break;
				default:
					Serial.printf("...");
					break;
			}
		}
		
		Serial.printf("\n");
	}
	
	Serial.printf("   ");
	for(uint8_t x = 0; x <= 48-1; x++) {
		Serial.printf("%02i%c", x, (x<=41?'|':'!'));
	}
	
	Serial.printf("\n\n");
}
// </PrintToConsole>

// <PrintToConsoleTwice>
void PrintToConsoleTwice(uint8_t* PTC_Frame, uint16_t PTC_SizeOfFrame, uint8_t* PTC_Frame2, uint16_t PTC_SizeOfFrame2) {
	for(uint8_t y = 0; y <= 7-1; y++) {
		Serial.printf("%02i|", y);
		for(uint8_t x = 0; x <= (PTC_SizeOfFrame/7)-1; x++) {
			uint8_t PTC_Pixel = PTC_Frame[y*(PTC_SizeOfFrame/7)+x];
			switch(PTC_Pixel) {
				case 1:
					Serial.printf("@@@");
					break;
				case 0:
					Serial.printf("^^^");
					break;
				default:
					Serial.printf("...");
					break;
			}
		}
		
		Serial.printf("\t");
		for(uint8_t x = 0; x <= (PTC_SizeOfFrame2/7)-1; x++) {
			uint8_t PTC_Pixel = PTC_Frame2[y*(PTC_SizeOfFrame2/7)+x];
			switch(PTC_Pixel) {
				case 1:
					Serial.printf("@@@");
					break;
				case 0:
					Serial.printf("^^^");
					break;
				default:
					Serial.printf("...");
					break;
			}
		}
		
		Serial.printf("\n");
	}
	
	Serial.printf("   ");
	for(uint8_t x = 0; x <= (PTC_SizeOfFrame/7)-1; x++) {
		Serial.printf("%02i%c", x, (x<=41?'|':'!'));
	}
	
	Serial.printf("\t");
	for(uint8_t x = 0; x <= (PTC_SizeOfFrame2/7)-1; x++) {
		Serial.printf("%02i%c", x, (x<=41?'|':'!'));
	}
	
	Serial.printf("\n\n");
}
// </PrintToConsoleTwice>

char Text1[16] = "Schrack Technic";
char Text2[23] = "Get Ready. Get Schrack";
char Text3[21] = "Schrack for Students";
char Text4[7] = "ARODEM";

void setup() {
	Serial.begin(115200);
	for(uint16_t i = 0; i <= 336-1; i++) {
		Frame[i] = 2;
	}
	
	SetHighlight(Frame, 1);
	
	/*
	for(uint8_t i = 0; i <= 9*sizeof(Text4)-1; i++) {
		DynamicCharTextToFrame(Frame, Text4, i%(sizeof(Text4)+1), sizeof(Text4));
		PrintToConsole(Frame, sizeof(Frame));
	}
	*/
	
	
	for(uint8_t i = 0; i <= 6*sizeof(Text4)-1; i++) {
		BetterDynamicCharTextToFrame(Frame, Text4, i%(sizeof(Text4)*6+1), sizeof(Text4), sizeof(Frame));	// HERE; hi Nati. pls fix. thx, prevoius Nat
		PrintToConsole(Frame, sizeof(Frame));
		//PrintToConsoleTwice(Frame, sizeof(Frame), BDCTFTempFrame, sizeof(BDCTFTempFrame));
	}
	
	
}

void loop() {
}