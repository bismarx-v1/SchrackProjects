/**
 * Test.
 */

/**
 * B0 - write more comments
 * B1 - fix game being ~33% faster
 * B2 - add case 3
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
#define DEBUG_NATS            1
#define GAME_LENGTH           15*60*10  // B2 relevant
#define GAME_FREEZE_LENGTH    5000
#define GAME_END_BLINK_LENGTH 500

#include <Arduino.h>
#include "maxDriver.h"
#include "Buttons.h"

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

  void setCur(uint8_t val) {
    currentVal = val;
  }
};
thingMode mode;


void setup() {
#if DEBUG_NATS
  Serial.begin(115200);
#endif
  display.displaySetup();
}


/**
 * ================
 * mode map
 * ----------------
 * default  - err screen
 * 0        - main menu
 * 1        - game running
 * 2        - game ended, wait a bit
 * ================
 */
void loop() {
  static uint8_t  whileProtection        = 0;
  static uint16_t score                  = 0;
  static uint64_t timeGameStartOrEnd     = 0;
  static uint64_t timeGameFreezeLastTrig = 0;
  static uint8_t  gameFreezeDisplayState = 1;

  switch(mode.current()) {
      // Err screen.
    default:
      if(mode.current() != mode.last()) {
        mode.set(-1);
        display.spiSend(display.maxRegisters.digit7, 0b01001111);  // Display error screen.
        display.spiSend(display.maxRegisters.digit6, 0b00000101);
        display.spiSend(display.maxRegisters.digit5, 0b00000101);
        display.spiSend(display.maxRegisters.digit4, 0b00000001);
        display.spiSend(display.maxRegisters.digit3, 0b01110110);
        display.spiSend(display.maxRegisters.digit2, 0b00110011);
        display.spiSend(display.maxRegisters.digit1, 0b01110111);
        display.spiSend(display.maxRegisters.digit0, 0b00000000);
      }

      break;

      // Main menu.
    case 0:
      if(mode.current() != mode.last()) {
        mode.set(0);
        display.spiSend(display.maxRegisters.digit0, 0b01110111);
      }

      if(buttons.main > 0) {
        buttons.main--;
        mode.setCur(1);
      }


      break;

      // Game running.
    case 1:
      if(mode.current() != mode.last()) {
        mode.set(1);
        timeGameStartOrEnd = millis();
      }

      while(buttons.main > 0 && whileProtection < 255) {
        whileProtection++;
        buttons.main--;
        score++;
        /*if(timeGameStartOrEnd + GAME_LENGTH < millis()) {
          mode.setCur(2);
        }*/
      }
      whileProtection = 0;

      display.display(display.DIS_SCORE, score, -1);

      if(timeGameStartOrEnd + GAME_LENGTH < millis()) {
        mode.setCur(2);
        display.display(display.DIS_TIME, 0, -1);
      } else {
        display.display(display.DIS_TIME, (GAME_LENGTH + (timeGameStartOrEnd - millis())) / 10, -1);
      }

      break;

      // Game ended freeze period
    case 2:
      if(mode.current() != mode.last()) {
        mode.set(2);
        timeGameStartOrEnd = millis();
      }

      if(timeGameFreezeLastTrig + GAME_END_BLINK_LENGTH < millis()) {
        timeGameFreezeLastTrig = millis();
        if(gameFreezeDisplayState == 1) {
          display.display(display.DIS_TIME, 0x7fff, -1);  // Clear.
        } else {
          display.display(display.DIS_TIME, 0, -1);  // Show zeros.
        }
        gameFreezeDisplayState = 1 - gameFreezeDisplayState;
      }


      if(timeGameStartOrEnd + GAME_FREEZE_LENGTH < millis()) {
        mode.setCur(3);
      }

      break;
  }

#if DEBUG_NATS
  if(Serial.available() > 0) {
    uint8_t incomingByte = Serial.read();
    Serial.println(incomingByte, HEX);
    buttons.main = buttons.main + incomingByte;
    Serial.println(buttons.main, HEX);
    Serial.println(mode.current(), HEX);
  }
#endif

  vTaskDelay(10);
  //vTaskDelay(5);
}
