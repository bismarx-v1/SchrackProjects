/**
 * Test.
 */

/**
 * write more comments
 * figure out what is COnsts.h
 * make buttons work
 */

/**
 * PROTOTYPE BOARD USES DIFFERENT PINS
 * PROTOTYPE BOARD USES DIFFERENT DISPLAY LAYOUT
 * 
 * ## ##  time
 * ###    score
 */
// clang-format off
// clang-format on

#include <Arduino.h>
#include "maxDriver.h"

struct thingMode {
private:
  uint8_t currentVal = 0;
  uint8_t lastVal    = -1;

public:
  uint8_t current() {
    return currentVal;
  }

  uint8_t last() {
    return lastVal;
  }

  void set(uint8_t val) {
    lastVal    = currentVal;
    currentVal = val;
  }
};
thingMode mode;

thingDisplay display;

void setup() {
  Serial.begin(115200);
  display.displaySetup();
}


void loop() {
  switch(mode.current()) {
    // Error.
    default:
      if(mode.current() != mode.last()) {
        mode.set(-1);
        display.spiSend(display.maxRegisters.digit7, 0b01001111);
        display.spiSend(display.maxRegisters.digit6, 0b00000101);
        display.spiSend(display.maxRegisters.digit5, 0b00000101);
        display.spiSend(display.maxRegisters.digit4, 0b00000001);
        display.spiSend(display.maxRegisters.digit3, 0b01110110);
        display.spiSend(display.maxRegisters.digit2, 0b00110011);
        display.spiSend(display.maxRegisters.digit1, 0b01110111);
        display.spiSend(display.maxRegisters.digit0, 0b00000000);
        
      }
      break;

    // 0.
    case 0:
      if(mode.current() != mode.last()) {
        mode.set(0);
      }
      mode.set(-1);
      break;

      // 1.
    case 1:
      if(mode.current() != mode.last()) {
        mode.set(1);
      }
      break;
  }
  
  delay(50);
}
