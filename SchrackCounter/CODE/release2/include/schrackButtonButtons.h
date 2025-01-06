#include <Arduino.h>

#include "schrackButtonPins.h"

#ifndef _BUTTONS_H
  #define _BUTTONS_H

  #ifndef DETECT_FALLING_EDGE
    #define DETECT_FALLING_EDGE 1
  #endif

struct buttonsStruct {
  uint8_t primary     = 0;
  uint8_t reset       = 0;
  uint8_t mode        = 0;
  void    setup();
  void    clear();
};
buttonsStruct buttons;


void IRAM_ATTR callbackBtnPrimary() {
  if(digitalRead(GPIO_BTN_MAIN_SWITCH) == 1 - DETECT_FALLING_EDGE) {
    buttons.primary++;
  } else {
    buttons.primary++;
  }
}


void IRAM_ATTR callbackBtnReset() {
  if(digitalRead(GPIO_BTN_RESET) == 1 - DETECT_FALLING_EDGE) {
    buttons.reset++;
  }
}


void IRAM_ATTR callbackBtnMode() {
  buttons.mode = digitalRead(GPIO_BTN_MODE);
}


/**
 * @brief Sets up the object().
 */
void buttonsStruct::setup() {
  pinMode(GPIO_BTN_MAIN_SWITCH, INPUT);
  pinMode(GPIO_BTN_RESET, INPUT);
  pinMode(GPIO_BTN_MODE, INPUT);
  mode = digitalRead(GPIO_BTN_MODE);

  #if DETECT_FALLING_EDGE
  attachInterrupt(GPIO_BTN_MAIN_SWITCH, callbackBtnPrimary, CHANGE);
  attachInterrupt(GPIO_BTN_RESET, callbackBtnReset, FALLING);
  attachInterrupt(GPIO_BTN_MODE, callbackBtnMode, CHANGE);
  #else
  attachInterrupt(GPIO_BTN_MAIN_SWITCH, callbackBtnPrimary, CHANGE);
  attachInterrupt(GPIO_BTN_RESET, callbackBtnReset, RISING);
  attachInterrupt(GPIO_BTN_MODE, callbackBtnMode, CHANGE);
  #endif
}


/**
 * @brief Clears all button values.
 */
void buttonsStruct::clear() {
  primary       = 0;
  reset       = 0;
}

#endif
