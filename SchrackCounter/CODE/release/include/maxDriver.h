#include <Arduino.h>
#include "pins.h"


#ifndef DRIVER_H
  #define DRIVER_H

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

  #ifdef IS_PROTOTYPE_LAYOUT

    #if IS_PROTOTYPE_LAYOUT == 1
  const uint8_t digitArray[8] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7};  // Prototype.
    #else
  //const uint8_t digitArray[8] = {digit0, digit4, digit6, digit2, digit3, digit1, digit5, digit7};  // Release.
  const uint8_t digitArray[8] = {digit3, digit7, digit5, digit1, digit2, digit6, digit4, digit0};  // Release.
    #endif
  #endif

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
  void clearDisplays();
  void displayNum(uint8_t pos, uint8_t num, uint8_t decimalPoint);

public:
  max7219Registers maxRegisters;
  const uint8_t    DIS_TIME  = 0;
  const uint8_t    DIS_SCORE = 1;

  void spiSend(uint8_t reg, uint8_t val);
  void displaySetup();
  void display(uint8_t screen, int16_t number, int8_t decimalPos);
};
thingDisplay display;


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
 * @param decimalPoint Position of the decimal point. -1 is no DP.
 */
void thingDisplay::displayNum(uint8_t pos, uint8_t num, uint8_t decimalPoint) {
  if(pos > 7 || pos < 0) {
    return;
  }

  if(decimalPoint > 1) {
    decimalPoint = 1;
  }


  spiSend(maxRegisters.digitArray[pos], maxRegisters.numberCharArray[num % 10] + 0b10000000 * decimalPoint);
}

/**
 * @brief Sets up the SPI pins and the display.
 */
void thingDisplay::displaySetup() {
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
 * @brief Displays a number on the displays
 * @param screen Screen to display to. (DIS_TIME|DIS_SCORE)
 * @param number Number to display. Limits depend on the display. (time:5999>n>-599|score:999>n>-99)
 * @param decimalPos Pos of the decimal point. -1 is no DP.
 */
void thingDisplay::display(uint8_t screen, int16_t number, int8_t decimalPos) {
  static int16_t numberTimeLast    = -600;
  static int16_t numberScoreLast   = -100;
  static int8_t  numberDpTimeLast  = -2;
  static int8_t  numberDpScoreLast = -2;
  if(screen == DIS_TIME) {                                      // Time display.
    if((number > 5999 && number != 0x7fff) || number < -599) {  // Check number boundries.
      return;
    }

    if(decimalPos > 4 || decimalPos < -1) {  // Check decimal point boundries.
      return;
    }

    if(decimalPos == -1) {
      decimalPos = 0xff;
    }

    if(number == numberTimeLast && decimalPos == numberDpTimeLast) {
      return;
    }
    numberTimeLast   = number;
    numberDpTimeLast = decimalPos;

    for(uint8_t i = 4; i < 8; i++) {
      spiSend(maxRegisters.digitArray[i], 0);
    }  // Clear.

    if(number != 0x7fff) {  // Normal num.

      // print(str((n-n%60)/60) + ":" + str(n%60))
      uint16_t numberRestOfDigitsLeft  = (number - number % 60) / 60;
      uint8_t  numberRestOfDigitsRight = number % 60;

      uint8_t posOnDisplay = 4;
      for(posOnDisplay; posOnDisplay < 8; posOnDisplay++) {
        if(posOnDisplay < 6) {
          displayNum(posOnDisplay, numberRestOfDigitsRight % 10, (decimalPos + 4 == posOnDisplay ? 1 : 0));
          numberRestOfDigitsRight = numberRestOfDigitsRight / 10;
        } else {
          displayNum(posOnDisplay, numberRestOfDigitsLeft % 10, (decimalPos + 4 == posOnDisplay ? 1 : 0));
          numberRestOfDigitsLeft = numberRestOfDigitsLeft / 10;
        }
      }
    } else {  // Clear display.
      for(uint8_t i = 4; i < 8; i++) {
        spiSend(maxRegisters.digitArray[i], (decimalPos + 4 == i ? 0b10000000 : 0));  // THIS
      }
    }


  } else if(screen == DIS_SCORE) {                            // Score display.
    if((number > 999 && number != 0x7fff) || number < -99) {  // Check number boundries.
      return;
    }

    if(decimalPos > 3 || decimalPos < -1) {  // Check decimal point boundries.
      return;
    }

    if(decimalPos == -1) {
      decimalPos = 0xff;
    }

    if(number == numberScoreLast && decimalPos == numberDpScoreLast) {
      return;
    }
    numberScoreLast   = number;
    numberDpScoreLast = decimalPos;

    for(uint8_t i = 0; i < 3; i++) {
      spiSend(maxRegisters.digitArray[i], 0);
    }  // Clear.
    uint16_t numberRestOfDigits = number;

    uint8_t posOnDisplay = 1;
    for(posOnDisplay; posOnDisplay < 4; posOnDisplay++) {
      displayNum(posOnDisplay, numberRestOfDigits % 10, (decimalPos == posOnDisplay ? 1 : 0));
      numberRestOfDigits = numberRestOfDigits / 10;
    }

    /*if(number < 0) {
      spiSend(maxRegisters.digitArray[posOnDisplay], maxRegisters.negativeSign);
    }*/


  } else {  // Not a display.
    return;
  }
}
#endif
