/**
 * max7219 docs - https://www.sparkfun.com/datasheets/Components/General/COM-09622-MAX7219-MAX7221.pdf
 * spi docs - https://www.circuitbasics.com/basics-of-the-spi-communication-protocol/
 * more in-depth spi - file:///%userprofile%/Downloads/Datalogger%20SPI%20mode%20timing.pdf	-	https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwj236m-xKeJAxWog_0HHfvmGKEQFnoECCUQAQ&url=https%3A%2F%2Fforums.parallax.com%2Fdiscussion%2Fdownload%2F50482%26d%3D1195317591&usg=AOvVaw3BeFz73hDrgI94akh_SbBl&opi=89978449
 */

#include <Arduino.h>

// PINS
#define SPI_MOSI 7
#define SPI_CLK  6
#define SPI_CS   5

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
};

max7219Registers maxRegisters;

void spiSetup() {
  pinMode(SPI_CS, OUTPUT);
  pinMode(SPI_CLK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  digitalWrite(SPI_MOSI, 0);
  digitalWrite(SPI_CLK, 0);
  digitalWrite(SPI_CS, 1);
}

void spiSend(uint8_t reg, uint8_t val) {
  digitalWrite(SPI_CS, 0);
  shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, reg);
  shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, val);
  digitalWrite(SPI_CS, 1);
}

void setup() {
  spiSetup();
  spiSend(maxRegisters.shutdown, 1);     // Exit shutdown.
  spiSend(maxRegisters.decodeMode, 0);   // Turn off decode.
  spiSend(maxRegisters.scanLimit, 7);    // Activeate all digits.
  spiSend(maxRegisters.intensity, 0xF);  // Intensity to max.

  /**
	 *  -A-
	 * |   |
	 * F   B
	 *  -G-
	 * E   C
	 * |   |
	 *  -D-  (p)
	 */
  //--------------------------------pABCDEFG

  spiSend(maxRegisters.digit7, 0);
  spiSend(maxRegisters.digit6, 0);
  spiSend(maxRegisters.digit5, 0);
  spiSend(maxRegisters.digit4, 0);
  spiSend(maxRegisters.digit3, 0);
  spiSend(maxRegisters.digit2, 0);
  spiSend(maxRegisters.digit1, 0);
  spiSend(maxRegisters.digit0, 0);

  spiSend(maxRegisters.digit7, 0b01001110);
  spiSend(maxRegisters.digit6, 0b01111111);
  spiSend(maxRegisters.digit5, 0b01110111);

  spiSend(maxRegisters.digit2, 0b01110110);
  spiSend(maxRegisters.digit1, 0b01110000);
  spiSend(maxRegisters.digit0, 0b00110000);
}

void loop() {
  delay(1000);
}
