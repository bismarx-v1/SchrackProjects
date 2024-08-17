//...///////////////////////
//..//    Pin defines    //
//.///////////////////////

// OUTPUTS
const uint8_t Pin_SrRows_A		= 23;	// Row SR input to be shifted in the SR QA out
const uint8_t Pin_SrRows_Clr	= 15;	// Row SR clear/enable; 0 = clear, 1 = enable
const uint8_t Pin_SrRows_Clk	= 02;	// Row SR shift
const uint8_t Pin_EnRowMos		= 27;	// Pin that enables row mosfets

const uint8_t Pin_SrCols_A[6]	= {32, 33, 25, 26, 14, 22};	// Col SR input...; {[0-7], [8-15], [16-23], [24-31], [32-39], [40-41+Reds]}; outputs 45, 46, 47 aren't connected
const uint8_t Pin_SrCols_Clr	= 13;							// Col SR clear/enable; 0 = clear, 1 = enable
const uint8_t Pin_SrCols_Clk	= 12;							// Row SR shift

const uint8_t Pin_Buzzer		= 21;	// Buzzer(active) enable pin

// INPUTS
const uint8_t Pin_Buttons[4] = {16, 17, 5, 18};	// Button pins; {[Btn1], [Btn2], [Btn3], [Btn4]}

/*
* Pin values last checked - 2.4.12024 @ 22:07
*/