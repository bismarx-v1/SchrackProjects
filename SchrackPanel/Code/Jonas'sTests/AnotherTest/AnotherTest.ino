/*
* - programs
* 	- test 2 changing images
*/

//DEBUG
uint32_t lastMillis = 0;
uint8_t FrameCycle = 0;

//   ////////////////
//  //    Defs    //
// ////////////////
#define Row_0		27
#define Row_1		26
#define Row_2		25
#define Row_3		33
#define Row_4		32
#define Row_5		14
#define Row_6		12

#define Col_Clk		21
#define Col_Clr		19

#define Col0_InA	18
#define Col1_InA	23
#define Col2_InA	22
#define Col3_InA	17
#define Col4_InA	16
#define Col5_InA	4

const uint8_t ColSrAPins[6] = {Col0_InA, Col1_InA, Col2_InA, Col3_InA, Col4_InA, Col5_InA};
const uint8_t RowPins[7] = {Row_0, Row_1, Row_2, Row_3, Row_4, Row_5, Row_6};

//   ////////////////////
//  //    Settings    //
// ////////////////////
const float PulseLenClear		= 0.0000001;
const float PulseLenAfterClear	= 0.0000001;

const float TimeBeforeShift		= 0.0000001;
const float PulseLenShift		= 0.0000001;
const float TimeAfterShift		= 0.0000001;

const float TimeBetweenOps		= 1; // 1 works fine; 0.5

const uint8_t DelayMode 		= 0;	// 0 = delay(), !0 = vTaskDelay()

const uint8_t TestFrame1[56] = {
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 1, 1, 0, 0
};

uint8_t TestFrame[56];
const uint8_t TestFrame2[56] = {
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
	
	BetterDelay(DelayMode, PulseLenClear);
	
	digitalWrite(Col_Clr, 1);
	
	BetterDelay(DelayMode, PulseLenAfterClear);
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
	BetterDelay(DelayMode, TimeBeforeShift);
	
	digitalWrite(Col_Clk, 1);
	
	BetterDelay(DelayMode, PulseLenShift);
	
	digitalWrite(Col_Clk, 0);
	
	BetterDelay(DelayMode, TimeAfterShift);
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
		TestFrame[i] = TestFrame1[i];
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
			Col_Set(0, TestFrame[x+y*8]);
			Col_Shift();
		}
		
		//BetterDelay(DelayMode, 1000);
		SelectRow(y);
		//BetterDelay(DelayMode, 3000);
		BetterDelay(DelayMode, TimeBetweenOps);
	}
	
	if(millis() >= lastMillis + 500) {
		lastMillis = millis();
		if(FrameCycle == 0) {
			FrameCycle = 1;
			for(uint8_t i = 0; i <= 56-1; i++) {
				TestFrame[i] = TestFrame1[i];
			}
		} else {
			FrameCycle = 0;
			for(uint8_t i = 0; i <= 56-1; i++) {
				TestFrame[i] = TestFrame2[i];
			}
		}
	}
}