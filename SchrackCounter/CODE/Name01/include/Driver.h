#include <Arduino.h>
#include "Consts.h"

#ifndef DRIVER_H
#define DRIVER_H


/**
 * 
*/
void spiSetup() {

}

/**
 * Sets up the driver
*/
void displaySetup() {
	spiTransfer(addr, OP_SHUTDOWN, 0x1);
	spiTransfer(addr, OP_INTENSITY, 0xf);	// (min, max) - (0x0, 0xf)
	spiTransfer(addr, OP_DECODEMODE, 0x0);	// No decode
	spiTransfer(addr, OP_SCANLIMIT, 0x7);
}

/*
	SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    //if(numDevices<=0 || numDevices>8 )
    //    numDevices=8;
    //maxDevices=numDevices;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    SPI_MOSI=dataPin;
    for(int i=0;i<64;i++) 
        status[i]=0x00;
    for(int i=0;i<maxDevices;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        setScanLimit(i,7);
        //decode is done in source
        spiTransfer(i,OP_DECODEMODE,0);
        clearDisplay(i);
        //we go into shutdown-mode on startup
        shutdown(i,true);
    }
*/




/**
 * @brief Displays a number on a display
 * @param display Display to display to (D_TIME, D_SCORE)
 * @param num Number to display
*/
void displaySetNum(uint8_t display, uint16_t num) {
	uint8_t digits = 0;	// Num of digits on one display
	const uint8_t* p_digitRegArr;	// Pointer to the pin regidter array

	if(display = D_TIME) {	// How many digits does the display have and which registers to use
		digits = 4;
		p_digitRegArr = OP_DIGITS_TIME_REV;
	} else if(display = D_SCORE) {
		digits = 3;
		p_digitRegArr = OP_DIGITS_SCORE_REV;
	} else {
		return;
	}

	for(uint8_t i = 0; i < digits; i++) {
		uint8_t digitDp;
		if(display = D_TIME && i == 2) {	// Time has a decimal point
			digitDp = 0b10000000;
		} else {
			digitDp = 0b0;
		}

		spiTransfer(addr, p_digitRegArr[i], NUMBER_CODES[num % 10] | digitDp);	// Write to driver
		num = num / 10; // Shift the num
	}
}

#endif
