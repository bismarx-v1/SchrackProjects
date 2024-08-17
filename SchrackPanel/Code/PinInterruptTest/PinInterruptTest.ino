#include <arduino.h>

struct Button {
	const uint8_t PIN;
	uint32_t numberKeyPresses;
	bool pressed;
};

Button button1 = {18, 0, false};

void IRAM_ATTR isr() {
	//Serial.println(millis());
	Serial.println(digitalRead(button1.PIN));
}

void setup() {
	Serial.begin(115200);
	pinMode(button1.PIN, INPUT_PULLUP);
	attachInterrupt(button1.PIN, isr, CHANGE);
}

void loop() {
	delay(10);
}