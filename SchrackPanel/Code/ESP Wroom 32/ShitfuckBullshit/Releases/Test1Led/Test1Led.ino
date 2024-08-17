//...///////////////////////
//..//    Pin defines    //
//.///////////////////////

// OUTPUTS
const uint8_t Pin_SrRows_A		= 34;	// Row SR input to be shifted in the SR QA out
const uint8_t Pin_SrRows_Clr	= 15;	// Row SR clear/enable; 0 = clear, 1 = enable
const uint8_t Pin_SrRows_Clk	= 2;	// Row SR shift
const uint8_t Pin_EnRowMos		= 27;	// Pin that enables row mosfets

const uint8_t Pin_SrCols_A[6]	= {32, 33, 25, 26, 14, 35};	// Col SR input...; {[0-7], [8-15], [16-23], [24-31], [32-39], [40-41+Reds]}; outputs 45, 46, 47 aren't connected
const uint8_t Pin_SrCols_Clr	= 13;							// Col SR clear/enable; 0 = clear, 1 = enable
const uint8_t Pin_SrCols_Clk	= 12;							// Row SR shift

const uint8_t Pin_Buzzer		= 21;	// Buzzer(active) enable pin

// INPUTS
const uint8_t Pin_Buttons[4] = {16, 17, 5, 18};	// Button pins; {[Btn1], [Btn2], [Btn3], [Btn4]}

/*
* Pin values last checked - 30.3.12024 @ 18:04
*/

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
	ShiftCols();
}


// Init
void InitDriver() {
	SetPinmodes();		// Sets pinModes
	DefaultStates();	// Sets default states
	// ClearAllSR();		// Clears Sr(s)
	
}

void PrintFrameToPanel(uint8_t* Frame) {
	AddCursorRows();	// Add cursor to first row
	for(uint8_t y = 0; y <= 7-1; y++) {
		for(uint8_t x = 0; x <= 48-1; x++) {
			WriteCols((x-x%8)/8, Frame[x+y*48]);
		}
		
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
			//Frame[x+y*48] = TestFrameHewwoCol3[x+y*42];
			Frame[x+y*48] = 0;
		}
		
		for(uint8_t x = 0; x <= 3-1; x++) {
			Frame[x+y*48+42] = 0;
		}
	}
	
	ClearAllSr();
	AddCursorRows();
	WriteCols(0, 1);
	
}

//.../////////////////////
//..//    void Loop    //
//./////////////////////
void loop() {

}