/**
 * In version 2 all functions just change a variable that is sent later at once.
 */
#include <Arduino.h>
#include "schrackButtonPins.h"


#ifndef _DRIVER_H
  #define _DRIVER_H

struct max7219Registers {
  const uint8_t digit0     = 0x1;
  const uint8_t digit1     = 0x2;
  const uint8_t digit2     = 0x3;
  const uint8_t digit3     = 0x4;
  const uint8_t digit4     = 0x5;
  const uint8_t digit5     = 0x6;
  const uint8_t digit6     = 0x7;
  const uint8_t digit7     = 0x8;
  const uint8_t decodeMode = 0x9;
  const uint8_t intensity  = 0xA;
  const uint8_t scanLimit  = 0xB;
  const uint8_t shutdown   = 0xC;
  const uint8_t testMode   = 0xF;

  const uint8_t digitArray[8] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7};

  // This array maps the registers to the digits in the segment array. Index is array index, number is digit register.
  const uint8_t digitIndexArray[8] = {0,4,6,2,1,5,7,3};

  /**
	 *  -A-
	 * |   |
	 * F   B
	 *  -G-
	 * E   C
	 * |   |
	 *  -D-  (p)
	 */
  // pABCDEFG
  const uint8_t numberCharArray[10] = {0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011,
                                       0b01011011, 0b01011111, 0b01110000, 0b01111111, 0b01111011};
  const uint8_t negativeSign        = 0b00000001;
};


struct thingDisplay {
private:
  void displayDigit(uint8_t pos, uint8_t num, uint8_t decimalPoint);

public:
  max7219Registers maxRegisters;
  typedef uint8_t screen;
  const screen SCR_TIME = 0;
  const screen SCR_SCORE = 1;

  uint8_t          segmentsArrayObjGlobal[8];  // Array with all segment values to to send.

  void setup();
  void spiSend(uint8_t reg, uint8_t val);
  void clear();
  void clearSpi();
  void display(screen scr, uint16_t number);
  void displaySegments(const uint8_t segments[8]);
  void push();
};
thingDisplay display;


/**
 * @brief Sets up the SPI pins and the display.
 */
void thingDisplay::setup() {
  pinMode(GPIO_SPI_CS_DRIVER, OUTPUT);
  pinMode(GPIO_SPI_CLK, OUTPUT);
  pinMode(GPIO_SPI_MOSI, OUTPUT);
  digitalWrite(GPIO_SPI_MOSI, 0);
  digitalWrite(GPIO_SPI_CLK, 0);
  digitalWrite(GPIO_SPI_CS_DRIVER, 1);

  spiSend(maxRegisters.shutdown, 1);     // Exit shutdown.
  spiSend(maxRegisters.testMode, 0);     // Tet mode off.
  spiSend(maxRegisters.decodeMode, 0);   // Turn off decode.
  spiSend(maxRegisters.scanLimit, 7);    // Activeate all digits.
  spiSend(maxRegisters.intensity, 0xF);  // Intensity to max.
  clearSpi();
}


/**
 * @brief Sends 2 bytes to a device.
 * @param reg Register to send to.
 * @param val Value to send.
 */
void thingDisplay::spiSend(uint8_t reg, uint8_t val) {
  digitalWrite(GPIO_SPI_CS_DRIVER, 0);
  shiftOut(GPIO_SPI_MOSI, GPIO_SPI_CLK, MSBFIRST, reg);
  shiftOut(GPIO_SPI_MOSI, GPIO_SPI_CLK, MSBFIRST, val);
  digitalWrite(GPIO_SPI_CS_DRIVER, 1);
}


/**
 * @brief Clears the displays.
 */
void thingDisplay::clear() {
  memset(segmentsArrayObjGlobal, 0, 8);
}


/**
 * @brief Clears the displays. Sends commands through spi instead of editing the segment array.
 */
void thingDisplay::clearSpi() {
  spiSend(maxRegisters.digit7, 0);
  spiSend(maxRegisters.digit6, 0);
  spiSend(maxRegisters.digit5, 0);
  spiSend(maxRegisters.digit4, 0);
  spiSend(maxRegisters.digit3, 0);
  spiSend(maxRegisters.digit2, 0);
  spiSend(maxRegisters.digit1, 0);
  spiSend(maxRegisters.digit0, 0);
}


/**
 * @brief Displays a number on a position.
 * @param pos Pos on the display.
 * @param num Num to display. Displays only the last digit. 0x7fff to clear display.
 * @param decimalPoint PDecimal point (0|1) - (no dP|dP).
 */
void thingDisplay::displayDigit(uint8_t pos, uint8_t num, uint8_t decimalPoint) {
  if(pos > 7 || pos < 0) {
    return;
  }

  if(decimalPoint != 1) {
    decimalPoint = 0;
  }

  //segmentsArrayObjGlobal[maxRegisters.digitIndexArray[pos]] = maxRegisters.numberCharArray[num % 10] | 0b10000000 * decimalPoint;
  segmentsArrayObjGlobal[pos] = maxRegisters.numberCharArray[num % 10] | 0b10000000 * decimalPoint;
}


/**
 * @brief Displays a number on the displays
 * @param mode (0|1) - (mm:ss:ss|hh:mm:ss).
 * @param number Number to display.
 */
void thingDisplay::display(screen scr, uint16_t number) {
  if(scr != 0 && scr != 1) {
    log_e("Wrong display.");
  }

  if(number > 0x270f && scr == SCR_TIME) {  // Check limit.
    log_e("Wrong!");
    return;
  }

  if(number > 0x3e7 && scr == SCR_SCORE) {  // Check limit.
    log_e("Wrong!");
    return;
  }

  if(scr == SCR_TIME) {
    displayDigit(3, number % 10, 0);
    displayDigit(2, number % 100 / 10, 0);
    displayDigit(1, number % 1000 / 100, 0);
    displayDigit(0, number / 1000, 0);
  } else {
    displayDigit(6, number % 10, 0);
    displayDigit(5, number % 100 / 10, 0);
    displayDigit(4, number / 100, 0);
  }
}


/**
 * @param segments Array of the segments.
 * @brief Writes a lot of stuff to the driver.
 */
void thingDisplay::displaySegments(const uint8_t segments[8]) {
  for(uint8_t i = 0; i < 8; i++) {
    segmentsArrayObjGlobal[i] = segments[i];
  }
}


/**
 * @brief Sends the segment values out.
 */
void thingDisplay::push() {
  for(uint8_t i = 0; i < 8; i++) {
    spiSend(maxRegisters.digitArray[maxRegisters.digitIndexArray[i]], segmentsArrayObjGlobal[i]);
  }
}

#endif
