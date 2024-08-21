#include <Arduino.h>
#include "Graphics.h"
#include "PinsAndConsts.h"

//========================================================
/**
 * @brief Prints the frame to console
 * @param Frame Frame var
*/
void PrintFrameToConsole(uint8_t* Frame) {
	Serial.print("#  ");
	for(uint8_t x = 0; x < CONST_Width; x++) {
		Serial.printf("%02i|", x);
	}

	Serial.print("  #\n");
	
	for(uint8_t y = 0; y < CONST_Height; y++) {
		Serial.printf("%02i|", y);
		for(uint8_t x = 0; x < CONST_Width; x++) {
			switch (Frame[y+x*7]) {
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

		Serial.printf("|%02i\n", y);
	}
	
	Serial.print("#  ");
	for(uint8_t x = 0; x < CONST_Width; x++) {
		Serial.printf("%02i|", x);
	}

	Serial.print("  #\n\n");
}
//========================================================

void setup() {
	//Serial.begin(115200);
	//Serial.begin(230400);
	Serial.begin(250000);
	delay(2000);
	GraphicsSetup();
	String Text;
	/*Text = "33:33:33";
	GraphicsLoop(Frame, 1, 1, Text, 0);
	PrintFrameToConsole(Frame);*/
	Text = "U R Cute. ";	// add a space after the text for a better text brake
	/*GraphicsLoop(Frame, 3, 1, Text, 6*0);
	PrintFrameToConsole(Frame);*/
	/*
	for(uint16_t i = 0; i < (Text.length()*6)+1; i++) {
		GraphicsLoop(Frame, 3, 1, Text, i);
		PrintFrameToConsole(Frame);
	}
	*/

	for(uint16_t i = 0; i < (Text.length()*6*2)+1; i++) {
		GraphicsLoop(Frame, 3, 1, Text, i);
		PrintFrameToConsole(Frame);
	}
	
	while (1) {
		delay(1000);
	}
}

void loop() {

}