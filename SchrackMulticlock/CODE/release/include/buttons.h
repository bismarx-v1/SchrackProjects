#include <Arduino.h>

#include "pins.h"

#ifndef _BUTTONS_H
  #define _BUTTONS_H

  #define DETECT_FALLING_EDGE 1

struct buttonsStruct {
  void    setup();
  uint8_t start   = 0;
  uint8_t reset   = 0;
  uint8_t dSelect = 0;
  uint8_t timeInc = 0;
  uint8_t timeDec = 0;
  uint8_t mode    = 0;
};
buttonsStruct buttons;


void IRAM_ATTR callbackBtnStartstop() {
  if(digitalRead(GPIO_BTN_STARTSTOP) == 1 - DETECT_FALLING_EDGE) {
    buttons.start++;
  }
}


void IRAM_ATTR callbackBtnReset() {
  if(digitalRead(GPIO_BTN_RESET) == 1 - DETECT_FALLING_EDGE) {
    buttons.reset++;
  }
}


void IRAM_ATTR callbackBtnDSel() {
  if(digitalRead(GPIO_BTN_DIGIT_SEL) == 1 - DETECT_FALLING_EDGE) {
    buttons.dSelect++;
  }
}


void IRAM_ATTR callbackBtnTimeInc() {
  if(digitalRead(GPIO_BTN_TIME_INC) == 1 - DETECT_FALLING_EDGE) {
    buttons.timeInc++;
  }
}


void IRAM_ATTR callbackBtnTimeDec() {
  if(digitalRead(GPIO_BTN_TIME_DEC) == 1 - DETECT_FALLING_EDGE) {
    buttons.timeDec++;
  }
}


void IRAM_ATTR callbackBtnMode() {
  buttons.mode = digitalRead(GPIO_BTN_MODE);
}


void buttonsStruct::setup() {
  pinMode(GPIO_BTN_STARTSTOP, INPUT);
  pinMode(GPIO_BTN_RESET, INPUT);
  pinMode(GPIO_BTN_DIGIT_SEL, INPUT);
  pinMode(GPIO_BTN_TIME_INC, INPUT);
  pinMode(GPIO_BTN_TIME_DEC, INPUT);
  pinMode(GPIO_BTN_MODE, INPUT);

  #if DETECT_FALLING_EDGE
  attachInterrupt(GPIO_BTN_STARTSTOP, callbackBtnStartstop, FALLING);
  attachInterrupt(GPIO_BTN_RESET, callbackBtnReset, FALLING);
  attachInterrupt(GPIO_BTN_DIGIT_SEL, callbackBtnDSel, FALLING);
  attachInterrupt(GPIO_BTN_TIME_INC, callbackBtnTimeInc, FALLING);
  attachInterrupt(GPIO_BTN_TIME_DEC, callbackBtnTimeDec, FALLING);
  attachInterrupt(GPIO_BTN_MODE, callbackBtnMode, CHANGE);
  #else
  attachInterrupt(GPIO_BTN_STARTSTOP, callbackBtnStartstop, RISING);
  attachInterrupt(GPIO_BTN_RESET, callbackBtnReset, RISING);
  attachInterrupt(GPIO_BTN_DIGIT_SEL, callbackBtnDSel, RISING);
  attachInterrupt(GPIO_BTN_TIME_INC, callbackBtnTimeInc, RISING);
  attachInterrupt(GPIO_BTN_TIME_DEC, callbackBtnTimeDec, RISING);
  attachInterrupt(GPIO_BTN_MODE, callbackBtnMode, CHANGE);
  #endif
}


#endif
