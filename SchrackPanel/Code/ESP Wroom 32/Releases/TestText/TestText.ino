/*
* Should print a static text
*/

#include "LibFilepathFix.h"

//...///////////////////
//..//    Timings    //
//.///////////////////
const float DefaultStatesDelay	= 500;
const float ClearPulseLength	= 500;
const float ShiftPulseLength	= 500;
const float RowDispalyLength	= 500;

//...////////////////////////////
//..//    Driver functions    //
//.////////////////////////////
// Defaults
void SetPinmodes() {
	pinMode(Pin_SrRows_A	, OUTPUT);
	pinMode(Pin_SrRows_Clr	, OUTPUT);
	pinMode(Pin_SrRows_Clk	, OUTPUT);
	
	pinMode(Pin_EnRowMos	, OUTPUT);
	for(uint8_t i = 0; i <= 6-1; i++) {
		pinMode(Pin_SrCols_A[i], OUTPUT);
	}
	
	pinMode(Pin_SrCols_Clr	, OUTPUT);
	pinMode(Pin_SrCols_Clk	, OUTPUT);
	
	pinMode(Pin_Buzzer		, OUTPUT);
	
	for(uint8_t i = 0; i <= 4-1; i++){
		pinMode(Pin_Buttons[i], INPUT);
	}
}

void DefaultStates() {
	digitalWrite(Pin_SrRows_A, 0);
	digitalWrite(Pin_SrRows_Clr, 0);
	digitalWrite(Pin_SrRows_Clk, 0);
	digitalWrite(Pin_EnRowMos, 0);
	for(uint8_t i = 0; i <= 6-1; i++) {
		digitalWrite(Pin_SrCols_A[i], 0);
	}
	
	digitalWrite(Pin_SrCols_Clr, 0);
	digitalWrite(Pin_SrCols_Clk, 0);
	
	digitalWrite(Pin_Buzzer, 0);
	delay(DefaultStatesDelay);
}

// Clear(s)
void ClearRows() {
	digitalWrite(Pin_SrRows_Clr, 0);
	delay(ClearPulseLength);
	digitalWrite(Pin_SrRows_Clr, 1);
	delay(ClearPulseLength);
}

void ClearCols() {
	digitalWrite(Pin_SrCols_Clr, 0);
	delay(ClearPulseLength);
	digitalWrite(Pin_SrCols_Clr, 1);
	delay(ClearPulseLength);
}

void ClearAllSr() {
	digitalWrite(Pin_SrCols_Clr, 0);
	digitalWrite(Pin_SrRows_Clr, 0);
	delay(ClearPulseLength);
	digitalWrite(Pin_SrCols_Clr, 1);
	digitalWrite(Pin_SrRows_Clr, 1);
	delay(ClearPulseLength);
}

// Shift(s)
void ShiftRows() {
	digitalWrite(Pin_EnRowMos, 0);
	digitalWrite(Pin_SrRows_Clk, 1);
	delay(ShiftPulseLength);
	digitalWrite(Pin_SrRows_Clk, 0);
	digitalWrite(Pin_EnRowMos, 1);
	delay(ShiftPulseLength);
}

void ShiftCols() {
	digitalWrite(Pin_SrCols_Clk, 1);
	delay(ShiftPulseLength);
	digitalWrite(Pin_SrCols_Clk, 0);
	delay(ShiftPulseLength);
}

// AddCursor/First shift
void AddCursorRows() {
	digitalWrite(Pin_SrRows_A, 1);
	ShiftRows();
	digitalWrite(Pin_SrRows_A, 0);
}

// Write new column
void WriteCols(uint8_t SReg, uint8_t Val) {
	digitalWrite(Pin_SrCols_A[SReg], Val);
}


// Init
void InitDriver() {
	SetPinmodes();		// Sets pinModes
	DefaultStates();	// Sets default states
	
}

void PrintFrameToPanel(uint8_t* Frame) {
	ClearAllSr();
	AddCursorRows();	// Add cursor to first row
	for(uint8_t y = 0; y <= 7-1; y++) {
		for(uint8_t x = 0; x <= 48-1; x++) {
			WriteCols((x-x%8)/8, Frame[x+y*48]);
		}
		
		ShiftCols();
		delay(RowDispalyLength);
		ClearCols();
		ShiftRows();
	}
}

//...////////////////////////
//..//    Frame define    //
//.////////////////////////
uint8_t Frame[336];
const uint8_t TestFrameHewwoCol3[294] = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0};





//...//////////////////////
//..//    void Setup    //
//.//////////////////////
void setup() {
	InitDriver();
	
	for(uint16_t i = 0; i <= 336-1; i++) {
		Frame[i] = 0;
	}
	
	for(uint8_t y = 0; y <= 7-1; y++) {
		for(uint8_t x = 0; x <= 42-1; x++) {
			Frame[x+y*48] = TestFrameHewwoCol3[x+y*42];
		}
		
		for(uint8_t x = 0; x <= 3-1; x++) {
			Frame[x+y*48+42] = 0;
		}
	}
}

//.../////////////////////
//..//    void Loop    //
//./////////////////////
void loop() {
	PrintFrameToPanel(Frame);

}