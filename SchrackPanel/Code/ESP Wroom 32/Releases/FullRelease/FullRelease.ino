#include "LibFilepathFix.h"

uint8_t Frame[336];

TaskHandle_t Driver;
void codeForDriver( void * parameter) {
	Driver_PrintFrameToPanel(Frame);
	taskYIELD();
}

TaskHandle_t Graphics;
void codeForGrph( void * parameter) {
	
	taskYIELD();
}

void setup() {
	Driver_PinModes();
	Driver_DefaultStates();
	DelayVersion = 1;
	
	// Driver task			|Task func		|Name		|Stack size	|Parameter of the task	|Priority	|Task handle	|Core
	xTaskCreatePinnedToCore(codeForDriver,	"Driver",	8000,		NULL,					1,			&Driver,		0);
	
	// Graphics task		|Task func		|Name		|Stack size	|Parameter of the task	|Priority	|Task handle	|Core
	xTaskCreatePinnedToCore(codeForGrph,	"Graphics",	8000,		NULL,					1,			&Graphics,		0);
	
}

void loop () {
	taskYIELD();
}