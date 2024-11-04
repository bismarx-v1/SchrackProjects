/**
 * Code showing a use case of the "millis()" function, Nya
 * - Natani
*/

#include <Arduino.h>


#define	FREQ01	500	// Minimal delay, in ms,  between triggers, Nya
uint64_t LastTriggered01 = 0;	// Var that stores the last time the thng triggered, Nya


void setup() {
}

void loop() {
	if(LastTriggered01 + FREQ01 <= millis() || LastTriggered01 == 0) {	// Check if enough time passed already, Nya (the "LastTriggered01 == 0" part just makes it so that the code can execute without waiting the initial %FREQ01% milliseconds)
		LastTriggered01 = millis();
		// Do something, Nya (just any code that needs to be triggered with atleast %FREQ01% delay between triggeres)
	}

	delay(5);	// This just simulates some other code doing stuff. 5ms is quite a long time for a piece of code, though, Nya
}
