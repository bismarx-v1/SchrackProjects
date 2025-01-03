/**
 * This now works but has no anims.
 * Mode (0|1) - (hh:mm:ss(ctu)|mm:ss.ss(ctd)).
 * Display max 0x22550FF
*/
/**
 * CHECK:
 * Check for "//DEBUG".
 * Check all uses of Serial.
 * TODO:
 * Ctu paused add anim.
 * Ctd paused add anim.
 * Ctd end screen anim.
 * Ctd start - blink cursor.
 * Other anims?
 * Add error and state logging using "log_e()".
 * FIX:
 */

#include <Arduino.h>
#include "maxDriverV2.h"
#include "macrosTimersV2.h"
#include "schrackStopwatchButtons.h"

// Constants.
#define NUMBER_OF_DIGITS 6  // Number of digits on the display.
#define NUMBER_OF_DIGITS_DRIVER 8  // Number of digits on the driver.
#define DISPLAY_MAX_TIME 0x22550FF  // Max number of seconds*10^-2 that fit on the display. Converts to 99:59:59.99.
#define HOUR_DISPLAY_DP_SHIFT 2  // By how many digits should the decimal point on the hour display be shifted.

/**
 * @param STUFF The stuff in the if.
 * @brief Sets the last state and does some stuff.
 */
#define CASE_ON_ENTER(STUFF)                                                                                                   \
  {                                                                                                                            \
    if(stateCurrent != stateLast) {                                                                                            \
      stateLast = stateCurrent;                                                                                                \
      STUFF                                                                                                                    \
    }                                                                                                                          \
  }


/**
 * @param STUFF The stuff in the if.
 * @brief Does stuff before changing case.
 */
#define CASE_ON_EXIT(STUFF)                                                                                                    \
  {                                                                                                                            \
    if(stateCurrent != stateLast) {                                                                                            \
      STUFF                                                                                                                    \
    }                                                                                                                          \
  }


// Main timer.
hw_timer_t* timerMainHandle = NULL;
#define TIMER_READ_DIVIDER 10000


/**
 * @param timeVarPointer Pointer to the time var.
 * @param cursorPos Position of the digit from the least significant digit.
 * @param operation Add or subtract 1. (!1|1) - (+1|-1).
 * @brief Takes in time in seconds*10^-2 and shifts the digit, pointed to by the cursor, by one up or down.
 * @note This is not a good solution, but i gave up on thinking.
 */
void timeDigitChange(uint32_t* timeVarPointer, uint8_t cursorPos, uint8_t operation) {
  uint8_t n01 = timeVarPointer[0] % 100;
  uint8_t n23 = (timeVarPointer[0] - n01) / 100 % 60;
  uint8_t n45 = (timeVarPointer[0] - n01 - n23 * 100) / 6000 % 60;
  uint8_t n67 = (timeVarPointer[0] - n01 - n23 * 100 - n45 * 6000) / 360000;
  if(cursorPos == 0 || cursorPos == 1) {
    uint8_t n0 = n01 % 10;
    uint8_t n1 = n01 / 10;
    if(cursorPos == 0) {
      if(operation != 1) {
        n0 = (n0 + 1) % 10;
      } else {
        n0 = (n0 + 10 - 1) % 10;
      }
    } else {
      if(operation != 1) {
        n1 = (n1 + 1) % 10;
      } else {
        n1 = (n1 + 10 - 1) % 10;
      }
    }
    timeVarPointer[0] = (n67) * 360000 + (n45) * 6000 + (n23) * 100 + (n1 * 10 + n0);
  } else if(cursorPos == 2 || cursorPos == 3) {
    uint8_t n2 = n23 % 10;
    uint8_t n3 = n23 / 10;
    if(cursorPos == 2) {
      if(operation != 1) {
        n2 = (n2 + 1) % 10;
      } else {
        n2 = (n2 + 10 - 1) % 10;
      }
    } else {
      if(operation != 1) {
        n3 = (n3 + 1) % 6;
      } else {
        n3 = (n3 + 6 - 1) % 6;
      }
    }
    timeVarPointer[0] = (n67) * 360000 + (n45) * 6000 + (n3 * 10 + n2) * 100 + (n01);
  } else if(cursorPos == 4 || cursorPos == 5) {
    uint8_t n4 = n45 % 10;
    uint8_t n5 = n45 / 10;
    if(cursorPos == 4) {
      if(operation != 1) {
        n4 = (n4 + 1) % 10;
      } else {
        n4 = (n4 + 10 - 1) % 10;
      }
    } else {
      if(operation != 1) {
        n5 = (n5 + 1) % 6;
      } else {
        n5 = (n5 + 6 - 1) % 6;
      }
    }
    timeVarPointer[0] = (n67) * 360000 + (n5 * 10 + n4) * 6000 + (n23) * 100 + (n01);
  } else if(cursorPos == 6 || cursorPos == 7) {
    uint8_t n6 = n67 % 10;
    uint8_t n7 = n67 / 10;
    if(cursorPos == 6) {
      if(operation != 1) {
        n6 = (n6 + 1) % 10;
      } else {
        n6 = (n6 + 10 - 1) % 10;
      }
    } else {
      if(operation != 1) {
        n7 = (n7 + 1) % 10;
      } else {
        n7 = (n7 + 10 - 1) % 10;
      }
    }
    timeVarPointer[0] = (n7 * 10 + n6) * 360000 + (n45) * 6000 + (n23) * 100 + (n01);
  }
}


// Setup.
void setup() {
  // Serial.
  Serial.begin(115200);  //DEBUG
  delay(2000);           //DEBUG

  // Setups.
  display.setup();               // Display.
  pinMode(GPIO_COLON1, OUTPUT);  // Display colons.
  pinMode(GPIO_COLON2, OUTPUT);
  buttons.setup();  // Buttons.

  // Create the timer.
  N_TIMER_BEGIN(timerMainHandle, 1, 8000)
}


// Loop.
void loop() {
  // States.
  static uint8_t stateCurrent = 0;
  static uint8_t stateLast    = -1;

  // Constant text patterns.
  const uint8_t segmentsErr[NUMBER_OF_DIGITS_DRIVER]      = {0b1001111, 0b101, 0b101, 0b1110110, 0b111011, 0b1110111, 0, 0};
  const uint8_t segmentsCtuStart[NUMBER_OF_DIGITS_DRIVER] = {0b1001110, 0, 0b1110000, 0b1000000, 0b111110, 0, 0, 0};
  const uint8_t segmentsPaused[NUMBER_OF_DIGITS_DRIVER]   = {0b1100111, 0b1110111, 0b111110, 0b1011011, 0b1001111, 0b111101, 0, 0};
  const uint8_t segmentsOwerflow[NUMBER_OF_DIGITS_DRIVER] = {0b1001111, 0b101, 0b101, 0, 0b1, 0b110000, 0, 0};
  const uint8_t segmentsEnd[NUMBER_OF_DIGITS_DRIVER]      = {0, 0b1001111, 0b1110110, 0b111101, 0, 0, 0, 0};

  // Display additions (colons and decimal points).
  static uint8_t colon1        = 0;
  static uint8_t colon2        = 0;
  static uint8_t decimalPoints = 0;


  // Timer values.
  static uint64_t timerVal;        // Timer time val.
  static uint64_t timerValBeforePause;        // Timer time val.
  static uint8_t  timer1Hz;        // 1Hz pulse based on the timer.
  static uint8_t  timer05Hz;       // 0.5Hz pulse based on the timer.
  static uint32_t timerCountFrom;  // Timer count from.

  // Ctd set time cursor.
  static uint8_t timerCountFromCursorPos = 0;

/**
 * @brief Updates the timer1Hz var from the timerVar.
 */
#define TIMER_1HZ_UPDATE()                                                                                                     \
  { timer1Hz = timerVal % 100 / 50; }

/**
 * @brief Updates the timer05Hz var from the timerVar.
 */
#define TIMER_05HZ_UPDATE()                                                                                                    \
  { timer05Hz = timerVal % 1000 / 100 % 2; }

  /**
   * case - desc
   * -----------
   * default - Error screen
   * 0 - Ctu start
   * 1 - Ctu running
   * 2 - Ctu paused
   * 3 - Ctu end
   * 4 - Ctd start
   * 5 - Ctd running
   * 6 - Ctd paused
   * 7 - Ctd end
  */
#define ctuStart   0
#define ctuRunning 1
#define ctuPaused  2
#define ctuEnd     3
#define ctdStart   4
#define ctdRunning 5
#define ctdPaused  6
#define ctdEnd     7

  // Main state switch.
  switch(stateCurrent) {
    // Error screen.
    default:
      CASE_ON_ENTER()
      display.displaySegments(segmentsErr);
      CASE_ON_EXIT(buttons.clear();)
      break;

    // Ctu start.
    case ctuStart:
      CASE_ON_ENTER()
      // Display menu screen.
      display.displaySegments(segmentsCtuStart);

      // State changes.
      if(buttons.mode == 1) {         // Switch mode.
        stateCurrent = ctdStart;      // Goto "Ctd start".
      } else if(buttons.start > 0) {  // Start ctu.
        buttons.start = 0;
        N_TIMER_RESET(timerMainHandle);  // Set timer to 0.
        stateCurrent = ctuRunning;          // Goto "Ctu running".
      }
      CASE_ON_EXIT(buttons.clear();)
      break;

    // Ctu running.
    case ctuRunning:
      CASE_ON_ENTER(N_TIMER_START(timerMainHandle))

      // Time stuff.
      N_TIMER_READ(timerMainHandle, timerVal, TIMER_READ_DIVIDER)  // Read time.
      display.display(1, timerVal);                                // Display time.
      // Toggle the colons.
      TIMER_1HZ_UPDATE()
      colon1 = timer1Hz;
      colon2 = timer1Hz ^ 1;

      // State changes.
      if(timerVal > DISPLAY_MAX_TIME) {      // Time overflow.
        stateCurrent = ctuEnd;        // Goto "Ctu end".
      } else if(buttons.reset > 0) {  // Cancel.
        buttons.reset = 0;
        stateCurrent  = ctuStart;     // Goto "Ctu start".
      } else if(buttons.start > 0) {  // Pause.
        buttons.start = 0;
        stateCurrent  = ctuPaused;  // Goto "Ctu paused".
      }
      CASE_ON_EXIT(N_TIMER_STOP(timerMainHandle) colon1 = 0; colon2 = 0; buttons.clear();)
      break;

    // Ctu paused.
    case ctuPaused:
      CASE_ON_ENTER()
      TIMER_05HZ_UPDATE()

      // Display the text.
      display.displaySegments(segmentsPaused);

      // State changes.
      if(buttons.reset > 0) {  // Cancel.
        buttons.reset = 0;
        stateCurrent  = ctuStart;     // Goto "Ctu start".
      } else if(buttons.start > 0) {  // Resume.
        buttons.start = 0;
        stateCurrent  = ctuRunning;  // Goto "Ctu running".
      }
      CASE_ON_EXIT(buttons.clear();)
      break;

    // Ctu end.
    case ctuEnd:
      CASE_ON_ENTER()
      // Text.
      display.displaySegments(segmentsOwerflow);

      // State changes.
      if(buttons.reset > 0) {  // Reset.
        buttons.reset = 0;
        stateCurrent  = ctuStart;     // Goto "Ctu start".
      } else if(buttons.start > 0) {  // Start.
        buttons.start = 0;
        stateCurrent  = ctuStart;  // Goto "Ctu start".
      }
      CASE_ON_EXIT(buttons.clear();)
      break;

    // Ctd start.
    case ctdStart:
      CASE_ON_ENTER(timerCountFrom = 0; timerCountFromCursorPos = 0; decimalPoints = 1 << HOUR_DISPLAY_DP_SHIFT;)

      // State change and registering other buttons.
      if(buttons.mode == 0) {         // Switch mode.
        stateCurrent = ctuStart;      // Goto "Ctu start".
      } else if(buttons.start > 0) {  // Start ctd.
        buttons.start = 0;
        N_TIMER_RESET(timerMainHandle);  // Set timer to 0.
        stateCurrent = ctdRunning;          // Goto "Ctd running".
      } else if(buttons.reset > 0) {        // Reset
        buttons.reset  = 0;
        timerCountFrom = 0;                 // Reset.
      } else if(buttons.digitSelect > 0) {  // Digit selsect (shift cursor).
        buttons.digitSelect     = 0;
        timerCountFromCursorPos = (timerCountFromCursorPos + 1) % NUMBER_OF_DIGITS;  // Shift the cursor.
        decimalPoints           = 1 << timerCountFromCursorPos + HOUR_DISPLAY_DP_SHIFT;       // Update the decimal point (it shows the selected digit).
      } else if(buttons.timeInc > 0) {  // Digit increment.
        buttons.timeInc = 0;
        timeDigitChange(&timerCountFrom, timerCountFromCursorPos, 0);
      } else if(buttons.timeDec > 0) {  // Digit decrement.
        buttons.timeDec = 0;
        timeDigitChange(&timerCountFrom, timerCountFromCursorPos, 1);
      }

      // Display the set time.
      display.display(0, timerCountFrom);
      CASE_ON_EXIT(buttons.clear(); decimalPoints = 0;)
      break;

    // Ctd running.
    case ctdRunning:
      CASE_ON_ENTER(N_TIMER_START(timerMainHandle))

      // Time stuff.
      N_TIMER_READ(timerMainHandle, timerVal, TIMER_READ_DIVIDER)  // Read time.
      display.display(0, timerCountFrom - timerVal);               // Display time.
      // Toggle the colons.
      TIMER_1HZ_UPDATE()
      colon1 = timer1Hz;
      colon2 = timer1Hz ^ 1;

      // State changes.
      if((uint32_t)(timerCountFrom - timerVal) > DISPLAY_MAX_TIME ||
         (uint32_t)(timerCountFrom - timerVal) == 0) {  // Time overflow or zero.
        stateCurrent = ctdEnd;                          // Goto "Ctd end".
      } else if(buttons.reset > 0) {                    // Cancel.
        buttons.reset = 0;
        stateCurrent  = ctdStart;     // Goto "Ctd start".
      } else if(buttons.start > 0) {  // Pause.
        buttons.start = 0;
        stateCurrent  = ctdPaused;  // Goto "Ctd paused".
      }
      CASE_ON_EXIT(N_TIMER_STOP(timerMainHandle) colon1 = 0; colon2 = 0; buttons.clear();)
      break;

    // Ctd paused (same as ctu paused, just different jumps).
    case ctdPaused:
      CASE_ON_ENTER()

      // Display the text.
      display.displaySegments(segmentsPaused);

      // State changes.
      if(buttons.reset > 0) {  // Cancel.
        buttons.reset = 0;
        stateCurrent  = ctdStart;     // Goto "Ctd start".
      } else if(buttons.start > 0) {  // Resume.
        buttons.start = 0;
        stateCurrent  = ctdRunning;  // Goto "Ctd running".
      }
      CASE_ON_EXIT(buttons.clear();)
      break;

    // Ctd end.
    case ctdEnd:
      CASE_ON_ENTER()
      // Text.
      display.displaySegments(segmentsEnd);

      // State changes.
      if(buttons.reset > 0) {  // Reset.
        buttons.reset = 0;
        stateCurrent  = ctdStart;     // Goto "Ctd start".
      } else if(buttons.start > 0) {  // Start.
        buttons.start = 0;
        stateCurrent  = ctdStart;  // Goto "Ctd start".
      }
      CASE_ON_EXIT(buttons.clear();)
      break;
  }

  // Set the decimal points.
  if(decimalPoints > 0) { // Check if any points need to be set.
    for(uint8_t digit = 0; digit < NUMBER_OF_DIGITS_DRIVER; digit++) {  // Go through all the digits.
      display.segmentsArrayObjGlobal[digit] = display.segmentsArrayObjGlobal[digit] | ((decimalPoints >> NUMBER_OF_DIGITS_DRIVER-1-digit) & 1) << NUMBER_OF_DIGITS_DRIVER-1; // Set the dP bit.
    }
  }
  
  // Send stuff to display.
  display.push();
  digitalWrite(GPIO_COLON1, colon1);
  digitalWrite(GPIO_COLON2, colon2);
  display.clear();
  delay(50);
}
