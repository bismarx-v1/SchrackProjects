
void setup() {
	Serial.begin(115200);
	for(uint8_t i = 0; i <= 255-1; i++) {
		Serial.printf("%i: ", i);
		Serial.printf("%c: ", i);
		Serial.printf("%i\n", i);
	}
}

void loop() {

}