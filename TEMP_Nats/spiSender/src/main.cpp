#include <Arduino.h>

#define MOSI 27
#define CLK	 26
#define CS	 25

void setup() {
	pinMode(MOSI, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(CS, OUTPUT);
	digitalWrite(CS, 1);

	Serial.begin(115200);
}

void loop() {
	if(Serial.available() > 1) {
		uint8_t reg = Serial.read();
		uint8_t val = Serial.read();

		Serial.print(reg, HEX);
		Serial.print("\t");
		Serial.println(val, HEX);

		digitalWrite(CS, 0);
		shiftOut(MOSI, CLK, LSBFIRST, reg);
		shiftOut(MOSI, CLK, LSBFIRST, val);
		digitalWrite(CS, 1);
	}
}
