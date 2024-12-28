#include <Arduino.h>

#include "pins.h"

#ifndef BUTTONS_H
  #define BUTTONS_H

  #define MORE_BUTTONS        0
  #define DETECT_FALLING_EDGE 1

struct buttonsStruct {
  uint8_t main = 0;
  #if MORE_BUTTONS
  uint8_t mode  = 0;
  uint8_t reset = 0;
  #endif
};
buttonsStruct buttons;


void IRAM_ATTR callbackBtnMain() {
  #if IS_PROTOTYPE_LAYOUT == 1
  static volatile uint64_t callbackMainLastTrig = 0;
  if(digitalRead(GPIO_BTN_MAIN) == 1 - DETECT_FALLING_EDGE) {
    if(digitalRead(GPIO_BTN_MAIN) == 1 - DETECT_FALLING_EDGE) {
      if(digitalRead(GPIO_BTN_MAIN) == 1 - DETECT_FALLING_EDGE) {
        if(callbackMainLastTrig + 2 < millis()) {
          callbackMainLastTrig = millis();
          buttons.main++;
        }
      }
    }
  }
  #else
  if(digitalRead(GPIO_BTN_MAIN) == 1 - DETECT_FALLING_EDGE) {
    buttons.main++;
  }
  #endif
}

  #if MORE_BUTTONS
void IRAM_ATTR callbackBtnMode() {
  buttons.mode++;
}

void IRAM_ATTR callbackBtnReset() {
  buttons.reset++;
}
  #endif

void buttonsSetup() {
  pinMode(GPIO_BTN_MAIN, INPUT);
  #if DETECT_FALLING_EDGE
  attachInterrupt(GPIO_BTN_MAIN, callbackBtnMain, FALLING);
  #else
  attachInterrupt(GPIO_BTN_MAIN, callbackBtnMain, RISING);
  #endif

  #if MORE_BUTTONS
  pinMode(GPIO_SW_MODE, INPUT);
  attachInterrupt(GPIO_SW_MODE, callbackBtnMode, FALLING);

  pinMode(GPIO_SW_RESET, INPUT);
  attachInterrupt(GPIO_SW_RESET, callbackBtnReset, FALLING);
  #endif
}


#endif
