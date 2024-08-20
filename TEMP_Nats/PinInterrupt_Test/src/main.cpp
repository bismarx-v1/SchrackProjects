/**
 * Code untested, i don't have a condensator for debouncing the button
 * This is made to be unbreakable and as fast as possible (on the detection side)
 * Everything you see here is just how i do it, not how it's done right
*/

#include <Arduino.h>

#define PIN_BUTTON1 4

uint8_t Button1PressedFlag = 0;		// How many times has the button been pressed since the last check
uint32_t Button1PressedTimes = 0;	// How many times was the button pressed
uint8_t Button1ChangeFlag = 0;		// Used for checking if the button triggered
uint64_t Button1PressedLast = 0;	// Used for calculating the time between presses

/**
 * @brief A callback function for a button
 * it's a Interrupt Service Routine, therefore it must
 * 	- not return anything
 * 	- be as fast as possible
 * the "IRAM_ATTR" makes it so it's stored in RAM instead of flash, and that makes it load faster
*/
void IRAM_ATTR callback_func() {
	Button1PressedFlag++;
}

void setup() {
	Serial.begin(230400);
	/**
	 * MODE can be
	 * 	- LOW
	 * 	- HIGH
	 * 	- CHANGE
	 * 	- FALLING
	 * 	- RISING
	*/
	attachInterrupt(PIN_BUTTON1, callback_func, FALLING);
}


void loop() {
	Button1Check:	// A label for the button 1 check. quite self-explanatory
	if(Button1PressedFlag > 0) {	// Check it there are any uncounted presses
		Button1PressedTimes++;
		Button1PressedFlag--;	// Doing things this way prevents bugs. the interrupt could happen at any moment
		Button1ChangeFlag = 1;	// Set flag
		goto Button1Check;	// Check again. if this were a function, this could be replaced with recursion
	}

	if(Button1ChangeFlag == 1) {
		Button1ChangeFlag = 0;	// Resed flag
		Serial.printf("Presses:\t%i;\tTime since last press:\t%i\n", Button1PressedTimes, millis()-Button1PressedLast);	// Print data to serial monitor. this should be the slowest thing here
		Button1PressedLast = millis();
	}
}