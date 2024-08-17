void Graphics_Setup() {
	int8_t Direction = 1;
	uint16_t Progress = 0;
	uint8_t Colour = 1;
	
}

void Graphics_Loop(uint8_t* Frame) {
	Frame[Progress] = Colour;
	if(Progress == sizeOf(Frame)-1) {	// If end - last
		Progress = 0;
		Colour = 1 - Colour;
	}
	
	Progress = Progress + Direction;
	
}