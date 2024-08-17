//...///////////////////////
//..//    Pin defines    //
//.///////////////////////
unsigned const char Pin_Row[3] = {12, 14, 27};					// pin numbers for row pins; 1st number is 0b001, 2nd is 0b010,...
unsigned const char Pin_Column[6] = {15, 16, 17, 5, 18, 19}; 	// pin numbers for shift registers; 1st number is 0-7 column register, 2nd is 8-15 column register,...
unsigned const char Pin_SRShift = 2;							// pin number for shift pin
unsigned const char Pin_SRClear = 0;							// pin number for clear pin
// red LEDs are toggled by the last column
unsigned const char Pin_RowsEnable = 4;							// pin number for red LEDs
unsigned const char Pin_BuzzerEnable = 26;						// pin number for buzzer


char Frame[336];


void setup() {
	for(char i=0; i<=2; i++) {	// <PinModes>
		pinMode(Pin_Row[i], OUTPUT);
	}
	
	for(char i=0; i<=5; i++) {
		pinMode(Pin_Column[i], OUTPUT);
	}
	
	pinMode(Pin_SRShift, OUTPUT);
	pinMode(Pin_SRClear, OUTPUT);
	pinMode(Pin_Reds, OUTPUT);
	pinMode(Pin_RowsEnable, OUTPUT);	// </PinModes>
	
	digitalWrite(Pin_SRClear, 1);
}

//...///////////////////////////////
//..//    LED display drivers    //
//.///////////////////////////////
void loop() {
	for(char RowLoopI = 0; RowLoopI <= 6; RowLoopI++) {	// selects the current row
		for(char ColLoopI = 0; ColLoopI <= 7; ColLoopI++) {	// loads the shift registers
			for(char SRI = 0; SRI <= 6; SRI++) {	// setting the column pins
				digitalWrite(Pin_Column[SRI], Frame[RowLoopI*35+ColLoopI+8*SRI]);
			}
			
			digitalWrite(Pin_SRShift, 1);	// <shift>
			delay(0.0002);
			digitalWrite(Pin_SRShift, 0);	// </shift>
		}
		
		switch(RowLoopI) { // open the right row
			case 0:
				digitalWrite(Pin_Row[0], 1);
				digitalWrite(Pin_Row[1], 0);
				digitalWrite(Pin_Row[2], 0);
				break;
			case 1:
				digitalWrite(Pin_Row[0], 0);
				digitalWrite(Pin_Row[1], 1);
				digitalWrite(Pin_Row[2], 0);
				break;
			case 2:
				digitalWrite(Pin_Row[0], 1);
				digitalWrite(Pin_Row[1], 1);
				digitalWrite(Pin_Row[2], 0);
				break;
			case 3:
				digitalWrite(Pin_Row[0], 0);
				digitalWrite(Pin_Row[1], 0);
				digitalWrite(Pin_Row[2], 1);
				break;
			case 4:
				digitalWrite(Pin_Row[0], 1);
				digitalWrite(Pin_Row[1], 0);
				digitalWrite(Pin_Row[2], 1);
				break;
			case 5:
				digitalWrite(Pin_Row[0], 0);
				digitalWrite(Pin_Row[1], 1);
				digitalWrite(Pin_Row[2], 1);
				break;
			case 6:
				digitalWrite(Pin_Row[0], 1);
				digitalWrite(Pin_Row[1], 1);
				digitalWrite(Pin_Row[2], 1);
				break;
		}
		
		digitalWrite(Pin_RowsEnable, 1);	// <EnableRows>
		delay(0.002);
		digitalWrite(Pin_RowsEnable, 0);	// </EnableRows>
	}
}