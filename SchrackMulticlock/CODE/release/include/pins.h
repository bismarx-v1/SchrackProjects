#include <Arduino.h>


#ifndef _PINS_H
  #define _PINS_H

  // GPIO  | NAME      | DESC.

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_BTN_TIME_INC 9

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_BTN_STARTSTOP 10

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_BTN_TIME_DEC 12

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_BTN_DIGIT_SEL 13

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_BTN_RESET 14

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_BTN_MODE 11

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_COLON2 37

  // 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup
  #define GPIO_COLON1 38

  // 09    | BUZZER    | Passive buzzer
  #define GPIO_BUZZER 8

  // 11    | CLOCK     | SPI clock; SD Card, MAX7219
  #define GPIO_SPI_CLOCK 42

  // 12    | MOSI      | SPI; SD Card, MAX7219
  #define GPIO_SPI_MOSI 1

  // 04    | cSelect0  | Chipselect for MAX7219; --defHigh
  #define GPIO_SPI_SEL_MAX 2

  // 38    | D_EXT1    | External device, eg: LEDs, Button
  #define GPIO_IO_EXT_1 35

  // 39    | D_EXT2    | External device, eg: LEDs, Button
  #define GPIO_IO_EXT_2 36
#endif
