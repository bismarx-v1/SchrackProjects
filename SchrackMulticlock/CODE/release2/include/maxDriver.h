#include <Arduino.h>
#include "schrackStopwatchPins.h"


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

  const uint8_t digitArray[8] = {digit6, digit2, digit3, digit7, digit5, digit1, digit4, digit0};  // Release.

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

  void setup();
  void spiSend(uint8_t reg, uint8_t val);
  void clearDisplays();
  void display(uint8_t mode, uint32_t number);
  void displaySegments(const uint8_t segments[8]);
};
thingDisplay display;


/**
 * @brief Sets up the SPI pins and the display.
 */
void thingDisplay::setup() {
  pinMode(GPIO_SPI_SEL_MAX, OUTPUT);
  pinMode(GPIO_SPI_CLOCK, OUTPUT);
  pinMode(GPIO_SPI_MOSI, OUTPUT);
  digitalWrite(GPIO_SPI_MOSI, 0);
  digitalWrite(GPIO_SPI_CLOCK, 0);
  digitalWrite(GPIO_SPI_SEL_MAX, 1);

  spiSend(maxRegisters.shutdown, 1);     // Exit shutdown.
  spiSend(maxRegisters.testMode, 0);     // Tet mode off.
  spiSend(maxRegisters.decodeMode, 0);   // Turn off decode.
  spiSend(maxRegisters.scanLimit, 7);    // Activeate all digits.
  spiSend(maxRegisters.intensity, 0xF);  // Intensity to max.
  clearDisplays();
}


/**
 * @brief Sends 2 bytes to a device.
 * @param reg Register to send to.
 * @param val Value to send.
 */
void thingDisplay::spiSend(uint8_t reg, uint8_t val) {
  digitalWrite(GPIO_SPI_SEL_MAX, 0);
  shiftOut(GPIO_SPI_MOSI, GPIO_SPI_CLOCK, MSBFIRST, reg);
  shiftOut(GPIO_SPI_MOSI, GPIO_SPI_CLOCK, MSBFIRST, val);
  digitalWrite(GPIO_SPI_SEL_MAX, 1);
}


/**
 * @brief Clears the displays.
 */
void thingDisplay::clearDisplays() {
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

  if(decimalPoint > 1) {
    decimalPoint = 1;
  } else {
    decimalPoint = 0;
  }

  spiSend(maxRegisters.digitArray[pos], maxRegisters.numberCharArray[num % 10] + 0b10000000 * decimalPoint);
}


/**
 * @brief Displays a number on the displays
 * @param mode (0|1) - (mm:ss:ss|hh:mm:ss).
 * @param number Number to display.
 */
void thingDisplay::display(uint8_t mode, uint32_t number) {
  static uint32_t numberPrev = -1;

  if(mode != 0 && mode != 1) {  // Check limit.
    log_e("Wrong!");
    return;
  }

  if(number > 0x14996FF) {  // Check limit.
    log_e("Wrong!");
    return;
  }

  if(number == numberPrev) {  // Same as last.
    return;
  }

  numberPrev = number;

  uint32_t remainingNum = number;  // This is bad.

  uint8_t  tMs          = remainingNum % 100;
  remainingNum -= tMs;

  uint8_t tS = remainingNum / 100 % 60;
  remainingNum -= tS;

  uint8_t tM = remainingNum / 6000 % 60;
  remainingNum -= tM;

  uint8_t tH = remainingNum / 360000;

  if(mode != 1) {
    displayDigit(0, tMs % 10, 0);  // ???
    displayDigit(1, tMs / 10, 0);
    displayDigit(2, tS % 10, 0);
    displayDigit(3, tS / 10, 0);
    displayDigit(4, tM % 10, 0);
    displayDigit(5, tM / 10, 0);
  } else {
    displayDigit(0, tS % 10, 0);
    displayDigit(1, tS / 10, 0);
    displayDigit(2, tM % 10, 0);
    displayDigit(3, tM / 10, 0);
    displayDigit(4, tH % 10, 0);
    displayDigit(5, tH / 10, 0);
  }
}


/**
 * @param segments Array of the segments.
 * @brief Writes a lot of stuff to the driver.
 */
void thingDisplay::displaySegments(const uint8_t segments[8]) {
  for(uint8_t i = 0; i < 8; i++) {
    spiSend(maxRegisters.digitArray[i], segments[i]);
  }
}

#endif