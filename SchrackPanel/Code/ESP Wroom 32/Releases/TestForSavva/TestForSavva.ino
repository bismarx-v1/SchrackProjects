#include "LibFilepathFix.h"

void setup() {
	Driver_PinModes();
	Driver_DefaultStates();
	ClearAllSr();
	
	AddCursorRows();
	digitalWrite(Pin_SrCols_A[0], 1);
	BetterDelay(DelayVersion, 10);
	for(uint8_t i = 0; i <= 8-1; i++) {
		ShiftColumns();
	}
}

void loop() {

}

/*
* Expected output
* ==================================================================================================================
* 	Sr_Rows		=	Sr_Cols0_00_07	Sr_Cols1_08_15	Sr_Cols2_16_23	Sr_Cols3_24_31	Sr_Cols4_32_39	Sr_Cols5_40_47
* 	Qa	1		=	Qa	1			Qa	0			Qa	0			Qa	0			Qa	0			Qa	0
* 	Qb	0		=	Qb	0			Qb	0			Qb	0			Qb	0			Qb	0			Qb	0
* 	Qc	0		=	Qc	0			Qc	0			Qc	0			Qc	0			Qc	0			Qc	0
* 	Qd	0		=	Qd	0			Qd	0			Qd	0			Qd	0			Qd	0			Qd	0
* 	Qe	0		=	Qe	0			Qe	0			Qe	0			Qe	0			Qe	0			Qe	0
* 	Qf	0		=	Qf	0			Qf	0			Qf	0			Qf	0			Qf	0			Qf	0
* 	Qg	0		=	Qg	0			Qg	0			Qg	0			Qg	0			Qg	0			Qg	0
* 	Qh	0		=	Qh	0			Qh	0			Qh	0			Qh	0			Qh	0			Qh	0
* ==================================================================================================================
*/