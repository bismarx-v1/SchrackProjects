//...///////////////////////
//..//    Pin defines    //
//.///////////////////////

// OUTPUTS
const uint8_t Pin_SrRows_A = 34;	// Row SR input to be shifted in the SR QA out
const uint8_t Pin_SrRows_Clr = 15;	// Row SR clear/enable; 0 = clear, 1 = enable
const uint8_t Pin_SrRows_Clk = 2;	// Row SR shift
const uint8_t Pin_EnRowMos = 27;	// Pin that enables row mosfets

const uint8_t Pin_SrCols_A[6] = {32, 33, 25, 26, 14, 35};	// Col SR input...; {[0-7], [8-15], [16-23], [24-31], [32-39], [40-41+Reds]}; outputs 45, 46, 47 aren't connected
const uint8_t Pin_SrCols_Clr = 13;							// Col SR clear/enable; 0 = clear, 1 = enable
const uint8_t Pin_SrCols_Clk = 12;							// Row SR shift

const uint8_t Pin_Buzzer = 21;	// Buzzer(active) enable pin

// INPUTS
const uint8_t Pin_Buttons[4] = {16, 17, 5, 18};	// Button pins; {[Btn1], [Btn2], [Btn3], [Btn4]}

/*
* Pin values last checked - 30.3.12024 @ 18:04
*/

//...///////////////////////
//..//    Pin defines    //
//.///////////////////////

void Driver_Setup() {
	/***********\
	* PinModes	*
	\***********/
	// Outputs
	pinMode(Pin_SrRows_A, OUTPUT);		// Row SR inputs
	pinMode(Pin_SrRows_Clr, OUTPUT);	// Row SR *clear
	pinMode(Pin_SrRows_Clk, OUTPUT);	// Row SR clock
	pinMode(Pin_EnRowMos, OUTPUT);		// Row mos(s) enable
	
	for(uint8_t i = 0; i <= 6-1; i++) {	// 6 Column SR inputs
		pinMode(Pin_SrCols_A[i], INPUT);
	}
	
	pinMode(Pin_SrCols_Clr, OUTPUT);	// Column SR *clear
	pinMode(Pin_SrCols_Clk, OUTPUT);	// Column SR clock
	
	pinMode(Pin_Buzzer, OUTPUT);		// Buzzer enable
	
	// Inputs
	for(uint8_t i = 0; i <= 4-1; i++) {	// 4 Buttons
		pinMode(Pin_Buttons[i], INPUT);
	}
	
	/*******************\
	* Default states	*
	\*******************/
	digitalWrite(Pin_SrRows_A, 0);		// Write row SR inputs 0
	digitalWrite(Pin_SrRows_Clr, 0);	// Write row SR *clear 0
	digitalWrite(Pin_SrRows_Clk, 0);	// Write row SR clock 0
	digitalWrite(Pin_EnRowMos, 1);		// Write row mos(s) enable 1
	
	for(uint8_t i = 0; i <= 6-1; i++) {	// Write 6 column SR inputs 0
		digitalWrite(Pin_SrCols_A[i], 0);
	}
	digitalWrite(Pin_SrCols_Clr, 0);	// Write column SR *clear 0
	digitalWrite(Pin_SrCols_Clk, 0);	// Write Column SR clock 0
	
	digitalWrite(Pin_Buzzer, 0);		// Write buzzer enable 0
	
	vTaskDelay(0.00000002);
	digitalWrite(Pin_SrRows_Clr, 1);	// Write row SR *clear 1
	digitalWrite(Pin_SrCols_Clr, 1);	// Write column SR *clear 1
}

void Driver_PrintFrame(uint8_t* Frame) {
	digitalWrite(Pin_SrRows_Clr, 0);	// <Reset>	// <Clear>	// Clear rows - pulse start
	digitalWrite(Pin_SrCols_Clr, 0);							// Clear cols - pulse start
	vTaskDelay(0.00000002);										// Pulse length
	digitalWrite(Pin_SrRows_Clr, 1);							// Clear rows - pulse end
	digitalWrite(Pin_SrCols_Clr, 1);				//</Clear>	// Clear cols - pulse end
	
	digitalWrite(Pin_SrRows_A, 1);					// <Add Crr>	// Add row carriage - pulse start
	vTaskDelay(0.00000002);											// Pulse length
	digitalWrite(Pin_SrRows_A, 0);					// </Add Crr>	// Add row carriage - pulse end
	
	digitalWrite(Pin_EnRowMos, 0);					// <Shift row>	// Disable rows
	digitalWrite(Pin_SrRows_Clk, 1);								// Shift row - pulse start
	vTaskDelay(0.00000002);											// Pulse length
	digitalWrite(Pin_SrRows_Clk, 0);								// Shift row - pulse end
	digitalWrite(Pin_EnRowMos, 1);		// </Reset>	// </Shift row>	// Enable rows
	 
	
	vTaskDelay(0.00000002);
	for(uint8_t y = 0; y <= 7-1; y++) {	// Row
		for(uint8_t x = 0; x <= 8-1; x++) {	// Offset in SR
			for(uint8_t Sr = 0; Sr <= 6-1; Sr++) {	// Row SR
				if(Frame[x+y*48+Sr*8] == 1) {	// If pixel value == 1
					digitalWrite(Pin_SrCols_A[Sr], 1);	// Write 1
				} else {
					digitalWrite(Pin_SrCols_A[Sr], 0);	// Write 0
				}
			}
			
			digitalWrite(Pin_SrCols_Clk, 1);	// <Shift col>	// Shift col - pulse start
			vTaskDelay(0.00000002);								// Pulse length
			digitalWrite(Pin_SrCols_Clk, 0);	// </Shift col>	// Shift col - pulse end
			
		}
		
		digitalWrite(Pin_EnRowMos, 0);	// <Shift row>	// Disable rows
		digitalWrite(Pin_SrRows_Clk, 1);				// Shift row - pulse start
		vTaskDelay(0.00000002);							// Pulse length
		digitalWrite(Pin_SrRows_Clk, 0);				// Shift row - pulse end
		digitalWrite(Pin_EnRowMos, 1);	// </Shift row>	// Enable rows
	}
}