void Graphics_Setup() {
	int8_t Direction = 1;
	uint16_t Progress = 0;
	
}

void Graphics_Loop(uint8_t* Frame) {
	Frame[Progress] = Direction;
	if(Progress == sizeOf(Frame)-1) {	// If end - last
		Direction = -1;
	}
	
	if(Progress == 0) {					// If end - first
		Direction = 1;
	}
	
	Progress = Progress + Direction;
	
}