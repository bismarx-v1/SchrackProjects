#include <Arduino.h>

#include "pins.h"

#ifndef BUTTONS_H
  #define BUTTONS_H

  #define MORE_BUTTONS 0

struct buttonsStruct {
  uint8_t main = 0;
  #if MORE_BUTTONS
  uint8_t mode  = 0;
  uint8_t reset = 0;
  #endif
};
buttonsStruct buttons;

void IRAM_ATTR callbackBtnMain() {
  buttons.main++;
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
  attachInterrupt(GPIO_BTN_MAIN, callbackBtnMain, FALLING);

  #if MORE_BUTTONS
  pinMode(GPIO_SW_MODE, INPUT);
  attachInterrupt(GPIO_SW_MODE, callbackBtnMode, FALLING);

  pinMode(GPIO_SW_RESET, INPUT);
  attachInterrupt(GPIO_SW_RESET, callbackBtnReset, FALLING);
  #endif
}


#endif
