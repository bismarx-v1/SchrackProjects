/*
* Row outputs NOT are inverted - ADDED ALREADY, pls don't fix Nat
* 	-Nat
*/

uint8_t			DelayVersion		= 0;	// 0 = delay(), !0 = vTaskDelay()
const float 	PulseLenClear		= 0.0000001;	// length of clear pulse
const float 	PulseLenShift		= 0.0000001;	// length of shift pulse
const float 	ShowRowTime			= 1;	// time between showing and hiding a row
const float 	TimeBeforeRShift	= 0.0000001;	// time before shifting a set value to register


void Driver_PinModes() {
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

void Driver_DefaultStates() {
	digitalWrite(Pin_SrRows_A	, 0);
	digitalWrite(Pin_SrRows_Clr	, 0);
	digitalWrite(Pin_SrRows_Clk	, 0);
	
	digitalWrite(Pin_EnRowMos	, 0);
	for(uint8_t i = 0; i <= 6-1; i++) {
		digitalWrite(Pin_SrCols_A[i], 0);
	}
	
	digitalWrite(Pin_SrCols_Clr	, 0);
	digitalWrite(Pin_SrCols_Clk	, 0);
	
	digitalWrite(Pin_Buzzer		, 0);
}

void BetterDelay(uint8_t mode, float time) {
	if(mode == 0) {
		delay(time);
	} else {
		vTaskDelay(time);
	}
}


void ClearAllSr() {
	digitalWrite(Pin_SrRows_Clr, 0);
	digitalWrite(Pin_SrCols_Clr, 0);
	
	BetterDelay(DelayVersion, PulseLenClear);
	
	digitalWrite(Pin_SrRows_Clr, 1);
	digitalWrite(Pin_SrCols_Clr, 1);
	
	BetterDelay(DelayVersion, PulseLenClear);
}

void ClearColumns() {
	digitalWrite(Pin_SrCols_Clr, 0);
	
	BetterDelay(DelayVersion, PulseLenClear);
	
	digitalWrite(Pin_SrCols_Clr, 1);
	
	BetterDelay(DelayVersion, PulseLenClear);
}

void ShiftRows() {
	digitalWrite(Pin_SrRows_Clk, 1);
	
	BetterDelay(DelayVersion, PulseLenShift);
	
	digitalWrite(Pin_SrRows_Clk, 0);
	
	BetterDelay(DelayVersion, PulseLenShift);
}

void ShiftColumns() {
	digitalWrite(Pin_SrCols_Clk, 1);
	
	BetterDelay(DelayVersion, PulseLenShift);
	
	digitalWrite(Pin_SrCols_Clk, 0);
	
	BetterDelay(DelayVersion, PulseLenShift);
}

void AddCursorRows() {
	digitalWrite(Pin_SrRows_A, 1);
	BetterDelay(DelayVersion, TimeBeforeRShift);
	ShiftRows();
	digitalWrite(Pin_SrRows_A, 0);
}


void Driver_PrintFrameToPanel(uint8_t* Frame) {
	ClearAllSr();
	for(uint8_t Row = 0; Row <= 7-1; Row++) {
		digitalWrite(Pin_EnRowMos, 0);	// Disable rows
		if(Row == 0) {
			AddCursorRows();			// Set cursor to first row
		} else {
			ShiftRows();				// Shift cursor to next row
		}
		for(uint8_t XOffset = 0; XOffset <= 8-1; XOffset++) {	// Load whole row
			for(uint8_t RegI = 0; RegI <= 6-1; RegI++){
				digitalWrite(Pin_SrCols_A[RegI], Frame[XOffset+RegI*8+Row*48]);
			}
			
			BetterDelay(DelayVersion, TimeBeforeRShift);
			ShiftColumns();
		}
		
		digitalWrite(Pin_EnRowMos, 1);	// Enable rows
		BetterDelay(DelayVersion, ShowRowTime);
		ClearColumns();
	}
}

