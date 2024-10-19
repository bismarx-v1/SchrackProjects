#include <Arduino.h>

#ifndef PINS_H
#define PINS_H

// GPIO  | NAME      | DESC. 

// 02    | MAIN_BTN  | Schrack light switch; --debounced --pullup 
#define	GPIO_BTN_MAIN		2

// 05    | R_LED     | LEDs, Indication --debug
#define	GPIO_LED_R			5

// 06    | G_LED     | LEDs, Indication --debug
#define	GPIO_LED_G			6

// 09    | BUZZER    | Passive buzzer
#define	GPIO_BUZZER			9

// 10    | MISO      | SPI; SD Card
#define	GPIO_SPI_MISO		10

// 11    | CLOCK     | SPI clock; SD Card, MAX7219
#define	GPIO_SPI_CLOCK		11

// 12    | MOSI      | SPI; SD Card, MAX7219
#define	GPIO_SPI_MOSI		12

// 04    | cSelect0  | Chipselect for MAX7219; --defHigh
#define	GPIO_SPI_SEL_MAX	4

// 13    | cSelect1  | Chipselect for SD Card; --defHigh
#define	GPIO_SPI_SEL_SDCARD	13

// 38    | D_EXT1    | External device, eg: LEDs, Button
#define	GPIO_IO_EXT_1		38

// 39    | D_EXT2    | External device, eg: LEDs, Button
#define	GPIO_IO_EXT_2		39

// 40    | D_EXT3    | External device, eg: LEDs, Button
#define	GPIO_IO_EXT_3		40

// 41    | MODE_SW   | External switch, --unused
#define	GPIO_SW_MODE		41

// 42    | RESET_SW  | External switch, Time/Score reset
#define	GPIO_SW_RESET		42

#endif