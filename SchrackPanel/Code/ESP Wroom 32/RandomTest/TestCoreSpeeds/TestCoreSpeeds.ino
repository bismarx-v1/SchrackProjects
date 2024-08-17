
TaskHandle_t Task1;
TaskHandle_t Task2;

/***************\
* Core1 code	*
\***************/
void codeForTask1( void * parameter) {
	Serial.printf("Task1: %i\n", xPortGetCoreID());
	uint64_t TomeNow = 0;
	uint64_t TomeLast = 0;
	for(;;) {	// <Task1 loop>
		
	};			// </Task1 loop>
}
/***************\
* /Core1 code	*
\***************/

/***************\
* Core0 code	*
\***************/
void codeForTask2( void * parameter) {
	uint64_t TomeNow = 0;
	uint64_t TomeLast = 0;
	Serial.printf("Task2: %i\n", xPortGetCoreID());
	for(;;) {	// <Task2 loop>
		
	};			// </Task2 loop>
}
/***************\
* /Core0 code	*
\***************/

void setup() {
	Serial.begin(115200);
	Serial.printf("Setup: %i\n", xPortGetCoreID());
	
	/*
	* Pin Task1 to core
	*/
	xTaskCreatePinnedToCore(
		codeForTask1,	/* Task func*/
		"Task_1",		/* Name*/
		4000,			/* Stack size*/
		NULL,			/* Parameter of the task*/
		1,				/* Priority*/
		&Task1,			/* Task handle*/
		1				/* Core*/
	);
	
	/*
	* Pin Task2 to core
	*/
	xTaskCreatePinnedToCore(
		codeForTask2,	/* Task func*/
		"Task_2",		/* Name*/
		4000,			/* Stack size*/
		NULL,			/* Parameter of the task*/
		1,				/* Priority*/
		&Task2,			/* Task handle*/
		0				/* Core*/
	);
}

void loop() {
	delay(10);
}
