#ifndef _LOCALIZATIONS
	#define _LOCALIZATIONS
	#include <Arduino.h>
	#define Lang 0	// vlaid vlaues: 0 - english; 1 - czech

	#define en 0
	#define cz 1

	#if Lang == cz	// Czech
		#define TEXT_MainMenu "Menu"	// found @ state 3
		#define TEXT_ScrtPt "EstrEgg?"		// found @ state 90
		#define TEXT_Time "Cas"				// found @ state 200
		#define TEXT_Countdown "Odpocet"	// found @ state 210
	#else			// English (default)
		#define TEXT_MainMenu "Menu"	// found @ state 3
		#define TEXT_ScrtPt "EstrEgg?"		// found @ state 90
		#define TEXT_Time "Time"			// found @ state 200
		#define TEXT_Countdown "Countdown"	// found @ state 210
	#endif
#endif