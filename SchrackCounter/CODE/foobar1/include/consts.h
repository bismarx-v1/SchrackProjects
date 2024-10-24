#include <Arduino.h>

#ifndef CONSTS_H
#define CONSTS_H

// the opcodes for the MAX7221 and MAX7219
// #define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SCANLIMIT 11
#define OP_SHUTDOWN 12
// #define OP_DISPLAYTEST 15

/**
 * real dispaly's #				- max7something's #
 * time(ss.ss)	; score(###)	- time(ss.ss)	; score(###)
 * 0, 1, 2, 3	; 4, 5, 6		- 0, 4, 6, 2	; 1, 5, 7
 */
// Reversed list of the digit geristers for time
const uint8_t OP_DIGITS_TIME_REV[4] = {OP_DIGIT2, OP_DIGIT6, OP_DIGIT4, OP_DIGIT0};

// Reversed list of the digit geristers for score
const uint8_t OP_DIGITS_SCORE_REV[3] = {OP_DIGIT7, OP_DIGIT5, OP_DIGIT1};

const uint8_t addr = 0;	 // position of the controlled driver in a daisy chain

#define D_TIME 0   // Time display
#define D_SCORE 1  // Score display

/**
 * | -A- |
 * F     B
 * |     |
 *   -G-
 * |     |
 * E     C
 * | -D- | (p)
 * -------------------------------pABCDEFG */
const uint8_t NUMBER_CODES[] = {
	0b01111110,	 // 0
	0b00110000,	 // 1
	0b01101101,	 // 2
	0b01111001,	 // 3
	0b00110011,	 // 4
	0b01011011,	 // 5
	0b01011111,	 // 6
	0b01110000,	 // 7
	0b01111111,	 // 8
	0b01111011	 // 9
};

#define MAX_DEVICES 1

#endif