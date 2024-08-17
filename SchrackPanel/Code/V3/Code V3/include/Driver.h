#ifndef _DRIVER
#define _DRIVER
#include <Arduino.h>
#include "PinsAndConsts.h"


struct ROWS {
	void Clear() {
		for(uint8_t i = 0; i < 7; i++) {	// Go through all rows
			digitalWrite(PINS_Row[i], 0);	// Set all to 0
		}
	}
	
	void Select(uint8_t Row) {
		for(uint8_t i = 0; i < 7; i++) {	// Go through all rows
			if(i == Row) {	// Set one to 1 and others to 0
				digitalWrite(PINS_Row[i], 1);
			} else {
				digitalWrite(PINS_Row[i], 0);
			}
		}
	}
};

struct COLUMNS {
	void Clear() {
		digitalWrite(PIN_Col_Clr, 0);
		vTaskDelay(CONST_PulseLenClear);
		digitalWrite(PIN_Col_Clr, 1);
		vTaskDelay(CONST_PulseLenAfterClear);
	}
	
	void Set(uint8_t SegmentN, uint8_t Bit) {
		digitalWrite(PINS_ColSrA[SegmentN], (Bit == 1? 1: 0));
	}
	
	void Shift() {
		vTaskDelay(CONST_TimeBeforeShift);
		digitalWrite(PIN_Col_Clk, 1);
		vTaskDelay(CONST_PulseLenShift);
		digitalWrite(PIN_Col_Clk, 0);
		vTaskDelay(CONST_TimeAfterShift);
	}
};

ROWS Rows;
COLUMNS Columns;

//==================

 /**
  * @brief Sets up driver
*/
void DriverSetup() {
	// pin modes
	pinMode(PIN_Col_Clk, OUTPUT);
	pinMode(PIN_Col_Clr, OUTPUT);
	for(uint8_t i = 0; i < 7; i++) {
		pinMode(PINS_Row[i], OUTPUT);
	}

	for(uint8_t i = 0; i < 6; i++) {
		pinMode(PINS_ColSrA[i], OUTPUT);
	}

	// pin resets
	digitalWrite(PIN_Col_Clk, 0);
	digitalWrite(PIN_Col_Clr, 0);
	for(uint8_t i = 0; i < 7; i++) {
		digitalWrite(PINS_Row[i], 0);
	}

	for(uint8_t i = 0; i < 6; i++) {
		digitalWrite(PINS_ColSrA[i], 0);
	}
}

#if 0	// use old display mode
/**
 * @brief Prints the frame to display. run this as fast as you can, it has vTaskDelays.
 * @param Frame Frame var
*/
void DriverLoop(uint8_t* Frame) {
	for(uint8_t SegmentY = 0; SegmentY < CONST_Height; SegmentY++) {	// Iterate through rows
		Rows.Clear();		// Deselect rows
		Columns.Clear();	// and clear columns
		for(uint8_t SegmentX = 0; SegmentX < CONST_SegmentWidth; SegmentX++) {	// Iterate through all columns in a segment
			for(uint8_t SegmentN = 0; SegmentN < CONST_Segments; SegmentN++) {	// Iterate through all segments
				Columns.Set(SegmentN, Frame[(SegmentY)+(SegmentX*CONST_Height+SegmentN*CONST_SegmentWidth*CONST_Height)]);
			}
			
			Columns.Shift();
		}
		
		Rows.Select(SegmentY);	// Select row
		vTaskDelay(CONST_TimeBetweenOps);
	}
}

#else	// reverse order of red leds and last two columns
/**
 * @brief Prints the frame to display. run this as fast as you can, it has vTaskDelays.
 * @param Frame Frame var
*/
void DriverLoop(uint8_t* Frame) {
	for(uint8_t SegmentY = 0; SegmentY < CONST_Height; SegmentY++) {	// Iterate through rows
		Rows.Clear();		// Deselect rows
		Columns.Clear();	// and clear columns
		for(uint8_t SegmentX = 0; SegmentX < CONST_SegmentWidth; SegmentX++) {	// Iterate through all columns in a segment
			for(uint8_t SegmentN = 0; SegmentN < CONST_Segments-1; SegmentN++) {	// Iterate through all except the last segment segments (set each segment except the last)
				Columns.Set(SegmentN, Frame[(SegmentY)+(SegmentX*CONST_Height+SegmentN*CONST_SegmentWidth*CONST_Height)]);
			}

			if(SegmentX == 0 || SegmentX == 1) {	// Set the last segment; this thing switches the order of red leds and the two columns in this segment
				Columns.Set(CONST_Segments-1, Frame[(SegmentY)+((SegmentX+2)*CONST_Height+(CONST_Segments-1)*CONST_SegmentWidth*CONST_Height)]);	// First set the reds
			} else if(SegmentX == 2 || SegmentX == 3) {
				Columns.Set(CONST_Segments-1, Frame[(SegmentY)+((SegmentX-2)*CONST_Height+(CONST_Segments-1)*CONST_SegmentWidth*CONST_Height)]);	// Second set the columns
			} else {
				Columns.Set(CONST_Segments-1, Frame[(SegmentY)+((SegmentX)*CONST_Height+(CONST_Segments-1)*CONST_SegmentWidth*CONST_Height)]);	// Set the rest; this should probably be just "set 0", <hj>but clock cycles are cheap</jh>
			}
			
			Columns.Shift();
		}
		
		Rows.Select(SegmentY);	// Select row
		vTaskDelay(CONST_TimeBetweenOps);
	}
}

#endif
#endif

// ===================================================
// The rest are, what appears to be, old tests and useless stuff
// Things might actually break if someone uses thease with the panel
// ===================================================

#if 0

const float CONST_PulseLenClear		= 0.0000001;
const float CONST_PulseLenAfterClear	= 0.0000001;

const float CONST_TimeBeforeShift		= 0.0000001;
const float CONST_PulseLenShift		= 0.0000001;
const float CONST_TimeAfterShift		= 0.0000001;

const float CONST_TimeBetweenOps		= 1; // 1 works fine; 0.5

const uint8_t DelayMode 		= 0;	// 0 = delay(), !0 = vTaskDelay()

const uint8_t Frame1[56] = {
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 1, 1, 0, 0
};

uint8_t Frame[56];
const uint8_t Frame2[56] = {
	1, 1, 0, 0, 0, 0, 1, 1,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 0, 0, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 1, 1
};

//   /////////////////
//  //    Funcs    //
// /////////////////

/*
* A delay that can switch between task delay and normal delay
* Returns
* 	- void
* Requires
* 	- mode
* 		 - uint8_t 
* 			 - 0 - delay
* 			 - !0 - vTaskDelay
* 	- time
* 		- float
* 			- in ms
*/
void BetterDelay(uint8_t mode, float time) {
	if(mode == 0) {
		delay(time);
	} else {
		vTaskDelay(time);
	}
}

/*
* Clears column shift registers
* Returns
* 	- void
* Requires
*	- void
*/
void Col_Clear() {
	digitalWrite(Col_Clr, 0);
	
	BetterDelay(DelayMode, CONST_PulseLenClear);
	
	digitalWrite(Col_Clr, 1);
	
	BetterDelay(DelayMode, CONST_PulseLenAfterClear);
}

/*
* Set the value that will be shifted into the Qa
* Returns
* 	- void
* Requires
* 	- SrNum
* 		- uint8_t
* 			-  reg num
* 			- (0-5) inclusive inclusive
* 	- NewBit
* 		- uint8_t
* 			-  new bit to be shifted in the Qa
* 			- (0-1) inclusive inclusive
*/
void Col_Set(uint8_t SrNum, uint8_t NewBit) {
	digitalWrite(ColSrAPins[SrNum], NewBit);	
}

/*
* Shift all 6 column shift registers
* Returns
* 	- void
* Requires
* 	- void
*/
void Col_Shift() {
	BetterDelay(DelayMode, CONST_TimeBeforeShift);
	
	digitalWrite(Col_Clk, 1);
	
	BetterDelay(DelayMode, CONST_PulseLenShift);
	
	digitalWrite(Col_Clk, 0);
	
	BetterDelay(DelayMode, CONST_TimeAfterShift);
}



/*
* Func for selecting a row
* Returns
* 	- void
* Requires
* 	- Row
* 		- uint8_t 
* 			- (0-6) inclusive inclusive
* 				- other values deselect all rows
*/
void SelectRow(uint8_t Row) {			// So you want to select a row
	for(uint8_t i = 0; i <= 7-1; i++) {	// You iterate througt all possible rows
		if(i == Row) {					// If the current row is the one you want to be set
			digitalWrite(RowPins[i], 1);			// You set it
		} else {						// Else
			digitalWrite(RowPins[i], 0);			//You reset it
		}
	}
}


//   /////////////////
//  //    Setup    //
// /////////////////
void setup() {
	for(uint8_t i = 0; i <= 56-1; i++) {
		Frame[i] = Frame1[i];
	}

	pinMode(Col_Clk, OUTPUT);
	pinMode(Col_Clr, OUTPUT);

	for(uint8_t i = 0; i <= 7-1; i++) {
		pinMode(RowPins[i], OUTPUT);
		if(i < 6) {
			pinMode(ColSrAPins[i], OUTPUT);
		}
	}



}

//   ////////////////
//  //    Loop    //
// ////////////////
void loop() {
	for(uint8_t y = 0; y <= 7-1; y++) {
		if(y >= 0 & y < 0) {
			continue;
		}
		
		SelectRow(7);
		Col_Clear();
		
		for(uint8_t x = 0; x <= 8-1; x++) {
			Col_Set(0, Frame[x+y*8]);
			Col_Shift();
		}
		
		//BetterDelay(DelayMode, 1000);
		SelectRow(y);
		//BetterDelay(DelayMode, 3000);
		BetterDelay(DelayMode, CONST_TimeBetweenOps);
	}
	
	if(millis() >= lastMillis + 500) {
		lastMillis = millis();
		if(FrameCycle == 0) {
			FrameCycle = 1;
			for(uint8_t i = 0; i <= 56-1; i++) {
				Frame[i] = Frame1[i];
			}
		} else {
			FrameCycle = 0;
			for(uint8_t i = 0; i <= 56-1; i++) {
				Frame[i] = Frame2[i];
			}
		}
	}
}
#endif

// ===================================================

#if 0
#include <Arduino.h>
#include "PinsAndConsts.h"

const float CONST_PulseLenClear		= 0.0000001;
const float CONST_PulseLenAfterClear	= 0.0000001;

const float CONST_TimeBeforeShift		= 0.0000001;
const float CONST_PulseLenShift		= 0.0000001;
const float CONST_TimeAfterShift		= 0.0000001;

const float CONST_TimeBetweenOps		= 1; // 1 works fine; 0.5

const uint8_t DelayMode 		= 0;	// 0 = delay(), !0 = vTaskDelay()

const uint8_t Frame1[56] = {
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 1, 1, 0, 0
};

uint8_t Frame[56];
const uint8_t Frame2[56] = {
	1, 1, 0, 0, 0, 0, 1, 1,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 0, 0, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 1, 1
};


/*
* Clears column shift registers
* Returns
* 	- void
* Requires
*	- void
*/
void Col_Clear() {
	digitalWrite(PIN_Col_Clr, 0);
	
	vTaskDelay(CONST_PulseLenClear);
	
	digitalWrite(PIN_Col_Clr, 1);
	
	vTaskDelay(CONST_PulseLenAfterClear);
}

/*
* Set the value that will be shifted into the Qa
* Returns
* 	- void
* Requires
* 	- SrNum
* 		- uint8_t
* 			-  reg num
* 			- (0-5) inclusive inclusive
* 	- NewBit
* 		- uint8_t
* 			-  new bit to be shifted in the Qa
* 			- (0-1) inclusive inclusive
*/
void Col_Set(uint8_t SrNum, uint8_t NewBit) {
	digitalWrite(ColSrAPins[SrNum], NewBit);
}

/*
* Shift all 6 column shift registers
* Returns
* 	- void
* Requires
* 	- void
*/
void Col_Shift() {
	vTaskDelay(CONST_TimeBeforeShift);
	
	digitalWrite(Col_Clk, 1);
	
	vTaskDelay(CONST_PulseLenShift);
	
	digitalWrite(Col_Clk, 0);
	
	vTaskDelay(CONST_TimeAfterShift);
}



/*
* Func for selecting a row
* Returns
* 	- void
* Requires
* 	- Row
* 		- uint8_t 
* 			- (0-6) inclusive inclusive
* 				- other values deselect all rows
*/
void SelectRow(uint8_t Row) {			// So you want to select a row
	for(uint8_t i = 0; i <= 7-1; i++) {	// You iterate througt all possible rows
		if(i == Row) {					// If the current row is the one you want to be set
			digitalWrite(RowPins[i], 1);			// You set it
		} else {						// Else
			digitalWrite(RowPins[i], 0);			//You reset it
		}
	}
}

//   /////////////////
//  //    Setup    //
// /////////////////
void setup() {
	for(uint8_t i = 0; i <= 56-1; i++) {
		Frame[i] = Frame1[i];
	}

	pinMode(Col_Clk, OUTPUT);
	pinMode(Col_Clr, OUTPUT);

	for(uint8_t i = 0; i <= 7-1; i++) {
		pinMode(RowPins[i], OUTPUT);
		if(i < 6) {
			pinMode(ColSrAPins[i], OUTPUT);
		}
	}
}

//   ////////////////
//  //    Loop    //
// ////////////////
void loop() {
	for(uint8_t y = 0; y <= 7-1; y++) {
		SelectRow(7);
		Col_Clear();
		
		for(uint8_t x = 0; x <= 8-1; x++) {
			Col_Set(0, Frame[x+y*8]);
			Col_Shift();
		}
		
		//vTaskDelay(1000);
		SelectRow(y);
		//vTaskDelay(3000);
		vTaskDelay(CONST_TimeBetweenOps);
	}
	
	if(millis() >= lastMillis + 500) {
		lastMillis = millis();
		if(FrameCycle == 0) {
			FrameCycle = 1;
			for(uint8_t i = 0; i <= 56-1; i++) {
				Frame[i] = Frame1[i];
			}
		} else {
			FrameCycle = 0;
			for(uint8_t i = 0; i <= 56-1; i++) {
				Frame[i] = Frame2[i];
			}
		}
	}
}
#endif