

uint8_t Frame[10];

void SetFrame(uint8_t* SF_Frame, int SF_Num) {
	for(uint16_t i = 0; i <= 10-1; i++) {
		SF_Frame[i] = SF_Num;
	}
}


void setup() {
	printf("T1\n");
	SetFrame(Frame, 2);
	printf("T2\n");
	for(uint16_t i = 0; i <= 10-1; i++) {
		printf("%02i\n", Frame[i]);
	}
	
	printf("T3\n");
	SetFrame(Frame, 1);
	printf("T4\n");
	for(uint16_t i = 0; i <= 10-1; i++) {
		printf("%02i\n", Frame[i]);
	}
	
	printf("T5\n");
}

void loop() {

}