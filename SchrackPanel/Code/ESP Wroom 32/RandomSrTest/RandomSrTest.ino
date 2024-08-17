#define Breadboard 1

#if Breadboard	// Breadboard
	const uint8_t Row_Clk	= 12;
	const uint8_t Row_Clr	= 14;
	const uint8_t Row_InA	= 27;
	
	const uint8_t Col0_Clk	= 26;
	const uint8_t Col0_Clr	= 25;
	const uint8_t Col0_InA	= 33;
	const uint8_t Row_En	= 0;
#else	//Pcb
	const uint8_t Row_Clk	= 02;
	const uint8_t Row_Clr	= 15;
	const uint8_t Row_InA	= 23;
	const uint8_t Row_En	= 27;
	
	const uint8_t Col0_Clk	= 12;
	const uint8_t Col0_Clr	= 13;
	const uint8_t Col0_InA	= 32;
#endif

const float PulseLenClear	= 0.0000001;
const float PulseLenShift	= 0.0000001;
const float TimeBeforeShift	= 0.0000001;
const float TimeBetweenOps	= 500;

void Row_Clear() {
	if(Breadboard != 0) {
		digitalWrite(Row_En, 0);
		delay(TimeBeforeShift);
	}
	
	digitalWrite(Row_Clr, 0);
	
	delay(PulseLenClear);
	
	digitalWrite(Row_Clr, 1);
	
	delay(PulseLenClear);
	if(Breadboard != 0) {
		digitalWrite(Row_En, 1);
		delay(TimeBeforeShift);
	}
}

void Row_Shift(uint8_t NewBit) {
	if(Breadboard != 0) {
		digitalWrite(Row_En, 0);
		delay(TimeBeforeShift);
	}
	digitalWrite(Row_InA, NewBit);
	
	delay(TimeBeforeShift);
	
	digitalWrite(Row_Clk, 1);
	
	delay(PulseLenShift);
	
	digitalWrite(Row_Clk, 0);
	
	delay(PulseLenShift);
	if(Breadboard != 0) {
		digitalWrite(Row_En, 1);
		delay(TimeBeforeShift);
	}
}

void Col0_Clear() {
	digitalWrite(Col0_Clr, 0);
	
	delay(PulseLenClear);
	
	digitalWrite(Col0_Clr, 1);
	
	delay(PulseLenClear);
}

void Col0_Shift(uint8_t NewBit) {
	digitalWrite(Col0_InA, NewBit);
	
	delay(TimeBeforeShift);
	
	digitalWrite(Col0_Clk, 1);
	
	delay(PulseLenShift);
	
	digitalWrite(Col0_Clk, 0);
	
	delay(PulseLenShift);
}


void setup() {
	pinMode(Row_Clk, OUTPUT);
	pinMode(Row_Clr, OUTPUT);
	pinMode(Row_InA, OUTPUT);
	pinMode(Col0_Clk, OUTPUT);
	pinMode(Col0_Clr, OUTPUT);
	pinMode(Col0_InA, OUTPUT);
	if(Breadboard != 0) {
		pinMode(Row_En, OUTPUT);
	}
	
	digitalWrite(Row_Clk, 0);
	digitalWrite(Row_Clr, 0);
	digitalWrite(Row_InA, 0);
	digitalWrite(Col0_Clk, 0);
	digitalWrite(Col0_Clr, 0);
	digitalWrite(Col0_InA, 0);
	if(Breadboard != 0) {
		digitalWrite(Row_En, 1);
	}
	
	Row_Clear();
	Col0_Clear();
	Col0_Shift(1);
	Row_Shift(1);
	delay(TimeBetweenOps);
}

void loop() {

	Col0_Clear();
	delay(TimeBetweenOps);
	for(uint8_t y = 0; y <= 6-1; y++) {
		for(uint8_t x = 0; x <= 7-1; x++) {
			Col0_Shift(0);
			delay(TimeBetweenOps);
		}
		
		Col0_Shift(1);
		delay(TimeBetweenOps);
		Row_Shift(0);
		if(y != 5) {
			delay(TimeBetweenOps);
		} else {
			Row_Shift(1);
		}
	}
}