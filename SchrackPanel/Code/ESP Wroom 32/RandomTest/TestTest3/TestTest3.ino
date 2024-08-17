
void setup() {
	Serial.begin(115200);
	uint64_t Text[90] = {' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ':', '-', '+', '*', '_', '.', ',', '?', '!', ';', '&', '/', '\\', '|', '(', ')', '[', ']', '{', '}', '<', '>', '=', '$', '#', '^', '~'};
	for(uint8_t i = 0; i <= (sizeof(Text)/sizeof(Text[0]))-1; i++) {
		Serial.printf("%i: ", i);
		Serial.printf("%c: ", Text[i]);
		Serial.printf("%i\n", Text[i]);
	}
}

void loop() {

}