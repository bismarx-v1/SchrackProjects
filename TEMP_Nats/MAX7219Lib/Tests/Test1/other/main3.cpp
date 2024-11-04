#include <Arduino.h>


#define DIN 5
#define CS  17
#define CLK 16


void shift(uint8_t val) {
	digitalWrite(DIN, val);
	delay(0.0005);	// 500ns
	digitalWrite(CLK, 1);
	delay(0.0005);	// 500ns
	digitalWrite(CLK, 0);
	delay(0.0005);	// 500ns
}

void shift16(uint16_t data) {
	for(u_int8_t i = 0; i < 16; i++) {
		shift(data && 1 << i);
	}
}


void setup() {
	pinMode(DIN, OUTPUT);
	pinMode(CS, OUTPUT);
	pinMode(CLK, OUTPUT);
	delay(200);

	digitalWrite(DIN, 0);
	digitalWrite(CS, 1);
	digitalWrite(CLK, 0);
	delay(200);

	digitalWrite(CS, 0);
	delay(0.0005);	// 500ns
	//        fedcba9876543210
	shift16(0b0000110000000001);	// Shutdown - normal
	shift16(0b0000100100000000);	// Decode - no
	shift16(0b0000101000001111);	// Intensity - full
	shift16(0b0000111100000000);	// Test - no
	shift16(0b0000101100000000);	// Scan limit(digits) - 1
	//                pABCDEFG
	shift16(0b0000000100000001);	// Digit0
	digitalWrite(CS, 1);
}

void loop() {

}