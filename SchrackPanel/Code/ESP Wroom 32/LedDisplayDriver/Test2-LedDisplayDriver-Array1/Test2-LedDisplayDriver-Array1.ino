//...///////////////////////
//..//    Pin defines    //
//.///////////////////////
unsigned const char Pin_Row[3] = {1, 2, 3};				// pin numbers for row pins; 1st number is 0b001, 2nd is 0b010,...
unsigned const char Pin_Column[6] = {1, 2, 3, 4, 5, 6}; 	// pin numbers for shift registers; 1st number is 1-8 column register, 2nd is 9-16 column register,...
unsigned const char Pin_SRShift = 1;						// pin number for shift pin
unsigned const char Pin_SRClear = 1;						// pin number for clear pin
unsigned const char Pin_Reds = 1;						// pin number for red LEDs
unsigned const char Pin_RowsEnable = 1;					// pin number for red LEDs

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