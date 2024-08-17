// <TimeToPixels>
void TimeToPixels(uint8_t* TTP_Time, uint8_t* TTP_Frame) {
	uint8_t* TTP_RetFrame;
	TTP_RetFrame = new uint8_t[336];
	
	for(uint16_t i = 0; i <= 336-1; i++) {
		TTP_RetFrame[i] = 2;
	}
	
	for(uint16_t i = 0; i <= 336-1; i++) {
		TTP_Frame[i] = TTP_RetFrame[i];
	}
	
	return;	// DEBUG
}