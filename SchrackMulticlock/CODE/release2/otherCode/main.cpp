/**
 * A.
 * Mode (0|1) - (hh:mm:ss(ctu)|mm:ss.ss(ctd)).
 * Display max 0x22550FF
*/
/**
 * TODO:
 * check all //DEBUG
 * count done cases
 * FIX:
 * start ctu from 0
 * stop ctu count when paused
 * stop ctd count when paused
 * ctu reset vars after doing a reset
 * ctd running no digits visible
 * 
 * slow down paused screen anim
 * ctd don't reset on resume
 * 
 * pausing offsets colons ad seconds
 */


#include <Arduino.h>
#include "maxDriverV2.h"
#include "macrosTimers.h"
#include "schrackStopwatchButtons.h"


// Settings.
#define CTD_SETUP_RESET 1  // Reset cursor and start value on ctd case setup? (0|!0) - (don't|do).


/**
 * @param STUFF The stuff in the if.
 * @brief Sets the last state and does some stuff.
 */
#define CASE_SETUP(STUFF)                                                                                                      \
  {                                                                                                                            \
    if(stateCurrent != stateLast) {                                                                                            \
      stateLast = stateCurrent;                                                                                                \
      STUFF                                                                                                                    \
    }                                                                                                                          \
  }


// Handle tor the main timer.
hw_timer_t* timer1Handle = NULL;


void setup() {
  // Serial.
  Serial.begin(115200);
  delay(2000);  //DEBUG
  // Setups.
  display.setup();  // Display.
  pinMode(GPIO_COLON1, OUTPUT);
  pinMode(GPIO_COLON2, OUTPUT);
  buttons.setup();  // Buttons.

  // Timer stuff.
  N_TIMER_BEGIN(timer1Handle, 1, 8000);  // Set timer to count.
  N_TIMER_START(timer1Handle);           // Start the timer.

  log_e("Setup done.");
}


void loop() {
  static uint64_t timer1Val = 0;  // milliseconds*10.
  N_TIMER_READ(timer1Handle, timer1Val, 10000)

  static uint64_t timer1ValCountStart;
  static uint32_t timer1ValCountPassedTime;
  static uint32_t timer1ValCountOnPausedTime;

  static uint8_t stateCurrent = 0;
  static uint8_t stateLast    = -1;
  // Constant text patterns.
  const uint8_t segmentsErr[8]    = {0b01001111, 0b00000101, 0b00000101, 0b01110110, 0b00111011, 0b01110111, 0b0, 0b0};
  const uint8_t segmentsCtu[8]    = {0b01001110, 0b0, 0b01110000, 0b01000000, 0b0111110, 0b0, 0b0, 0b0};
  const uint8_t segmentsCtd[8]    = {0b01001110, 0b0, 0b01110000, 0b01000000, 0b0111101, 0b0, 0b0, 0b0};  //DEBUG - DELETE?
  const uint8_t segmentsPaused[8] = {0b01100111, 0b01110111, 0b00111110, 0b01011011, 0b01001111, 0b00111101, 0b0, 0b0};
  const uint8_t segmentsEnded[8] = {0b01001111, 0b01110110, 0b00111101, 0b01001111, 0b00111101, 0b0, 0b0, 0b0};
  const uint8_t segmentsOwerflow[8] = {0b01001111, 0b00000101, 0b00000101, 0b0, 0b00000001, 0b00110000, 0b0, 0b0};

  static uint32_t timeCountFrom            = 0;  // Time to count from in the ctd mode.
  static uint8_t  timeCountFromDigitCursor = 0;  // Cursor for which digit to increase in the start menu for the ctd mode.
  static uint8_t  showDigitSelectDot       = 1;

  static uint8_t clock05Hz;  // Used for anything that should be 1s on 1s off, like the colons.
  static uint8_t clock1Hz;   // Used for anything that should be 0.5s on 0.5s off.
  clock05Hz = timer1Val / 100 % 2;
  clock1Hz  = timer1Val / 25 % 2;

  static uint8_t colon1 = 0;
  static uint8_t colon2 = 0;


  /**
   * case - desc
   * -----------
   * default - Error screen
   * 0 - Ctu start menu
   * 1 - Ctd start menu
   * 2 - Ctu running
   * 3 - Ctd running
   * 4 - Ctu paused
   * 5 - Ctd paused
   * 6 - Ctu ended
   * 7 - Ctd ended
   */
  switch(stateCurrent) {
    // Error screen.
    default:
      CASE_SETUP(colon1 = 0; colon2 = 0;)
      display.displaySegments(segmentsErr);
      break;

    // Count up start menu.
    case 0:
      CASE_SETUP(colon1 = 0; colon2 = 0;)
      display.displaySegments(segmentsCtu);
      // Change mode.
      if(buttons.mode == 1) {
        stateCurrent = 1;  // Goto case 1.
        break;
      }

      // Start.
      if(buttons.start > 0) {
        buttons.start = 0;
        stateCurrent  = 2;  // Goto case 2.
      }
      break;

    // Count down start menu.
    case 1:
      CASE_SETUP(if(CTD_SETUP_RESET != 0) {
        timeCountFrom            = 0;
        timeCountFromDigitCursor = 0;
      } colon1          = 0;
                 colon2 = 0;
                 if(stateLast != 3) {
                  timer1ValCountPassedTime = 0;
                 })
      // Change mode.
      if(buttons.mode == 0) {
        stateCurrent = 0;  // Goto case 0.
        break;
      }

      // Reset.
      if(buttons.reset > 0) {
        buttons.reset = 0;
        timeCountFrom = 0;
      }

      // Start.
      if(buttons.start > 0) {
        buttons.start = 0;
        stateCurrent  = 3;  // Goto case 3.
      }

      // Digit select.
      for(uint8_t i = 0; i < 0xff; i++) {
        if(buttons.digitSelect == 0) {
          break;
        }
        buttons.digitSelect--;
        timeCountFromDigitCursor = (timeCountFromDigitCursor + 1) % 6;  // Move the cursor.
      }

      // Time increment.
      for(uint8_t i = 0; i < 0xff; i++) {
        if(buttons.timeInc == 0) {
          break;
        }
        buttons.timeInc--;

        // Enjoy this thing,,, Sowwy >w<.
        uint8_t n01 = timeCountFrom % 100;
        uint8_t n23 = (timeCountFrom - n01) / 100 % 60;
        uint8_t n45 = (timeCountFrom - n01 - n23 * 100) / 6000 % 60;
        uint8_t n67 = (timeCountFrom - n01 - n23 * 100 - n45 * 6000) / 360000;
        if(timeCountFromDigitCursor == 0 || timeCountFromDigitCursor == 1) {
          uint8_t n2 = n23 % 10;
          uint8_t n3 = n23 / 10;
          if(timeCountFromDigitCursor == 0) {
            n2 = (n2 + 1) % 10;
          } else {
            n3 = (n3 + 1) % 6;
          }
          timeCountFrom = (n67) * 360000 + (n45) * 6000 + (n3 * 10 + n2) * 100 + (n01);
        } else if(timeCountFromDigitCursor == 2 || timeCountFromDigitCursor == 3) {
          uint8_t n4 = n45 % 10;
          uint8_t n5 = n45 / 10;
          if(timeCountFromDigitCursor == 2) {
            n4 = (n4 + 1) % 10;
          } else {
            n5 = (n5 + 1) % 6;
          }
          timeCountFrom = (n67) * 360000 + (n5 * 10 + n4) * 6000 + (n23) * 100 + (n01);
        } else if(timeCountFromDigitCursor == 4 || timeCountFromDigitCursor == 5) {
          uint8_t n6 = n67 % 10;
          uint8_t n7 = n67 / 10;
          if(timeCountFromDigitCursor == 4) {
            n6 = (n6 + 1) % 10;
          } else {
            n7 = (n7 + 1) % 10;
          }
          timeCountFrom = (n7 * 10 + n6) * 360000 + (n45) * 6000 + (n23) * 100 + (n01);
        }
      }

      // Time decrement.
      for(uint8_t i = 0; i < 0xff; i++) {
        if(buttons.timeDec == 0) {
          break;
        }
        buttons.timeDec--;
        // Again :3, but something has changed >:3<.
        uint8_t n01 = timeCountFrom % 100;
        uint8_t n23 = (timeCountFrom - n01) / 100 % 60;
        uint8_t n45 = (timeCountFrom - n01 - n23 * 100) / 6000 % 60;
        uint8_t n67 = (timeCountFrom - n01 - n23 * 100 - n45 * 6000) / 360000;
        if(timeCountFromDigitCursor == 0 || timeCountFromDigitCursor == 1) {
          uint8_t n2 = n23 % 10;
          uint8_t n3 = n23 / 10;
          if(timeCountFromDigitCursor == 0) {
            n2 = (n2 + 9) % 10;
          } else {
            n3 = (n3 + 5) % 6;
          }
          timeCountFrom = (n67) * 360000 + (n45) * 6000 + (n3 * 10 + n2) * 100 + (n01);
        } else if(timeCountFromDigitCursor == 2 || timeCountFromDigitCursor == 3) {
          uint8_t n4 = n45 % 10;
          uint8_t n5 = n45 / 10;
          if(timeCountFromDigitCursor == 2) {
            n4 = (n4 + 9) % 10;
          } else {
            n5 = (n5 + 5) % 6;
          }
          timeCountFrom = (n67) * 360000 + (n5 * 10 + n4) * 6000 + (n23) * 100 + (n01);
        } else if(timeCountFromDigitCursor == 4 || timeCountFromDigitCursor == 5) {
          uint8_t n6 = n67 % 10;
          uint8_t n7 = n67 / 10;
          if(timeCountFromDigitCursor == 4) {
            n6 = (n6 + 9) % 10;
          } else {
            n7 = (n7 + 9) % 10;
          }
          timeCountFrom = (n7 * 10 + n6) * 360000 + (n45) * 6000 + (n23) * 100 + (n01);
        }
      }

      // Write to display.
      display.display(1, timeCountFrom);
      // Add the dot to show the selected digit.
      display.segmentsArrayObjGlobal[5 - timeCountFromDigitCursor] =
      display.segmentsArrayObjGlobal[5 - timeCountFromDigitCursor] | 0b10000000 * showDigitSelectDot;
      // Toggle the digit select.
      showDigitSelectDot = clock1Hz;
      break;

    // Ctu running.
    case 2:
      CASE_SETUP(timer1ValCountStart = timer1Val;)
      if(timer1Val - timer1ValCountStart > 0x22550FF) {
        stateCurrent = 6;  // Goto case 6.
        break;
      }

      // Cancel.
      if(buttons.reset > 0) {
        buttons.reset = 0;
        stateCurrent  = 0;  // Goto case 0.
        break;
      }

      // Pause.
      if(buttons.start > 0) {
        buttons.start = 0;
        stateCurrent  = 4;  // Goto case 4.
        break;
      }


      // Write to display.
      display.display(0, timer1Val - timer1ValCountStart);
      // Toggle colons.
      colon1 = clock05Hz;
      colon2 = clock05Hz ^ 1;
      break;

    // Ctd running.
    case 3:
      CASE_SETUP(timer1ValCountStart = timer1Val;)
      if(timeCountFrom - (timer1Val - timer1ValCountStart + timer1ValCountPassedTime) <= 0 || timeCountFrom - (timer1Val - timer1ValCountStart + timer1ValCountPassedTime) > 0x22550FF) {
        stateCurrent = 7;  // Goto case 7.
        break;
      }

      // Cancel.
      if(buttons.reset > 0) {
        buttons.reset = 0;
        stateCurrent  = 1;  // Goto case 1.
        break;
      }

      // Pause.
      if(buttons.start > 0) {
        buttons.start = 0;
        timer1ValCountOnPausedTime = timeCountFrom - (timer1Val - timer1ValCountStart + timer1ValCountPassedTime);
        stateCurrent  = 5;  // Goto case 5.
        break;
      }


      // Write to display.
      display.display(1, timeCountFrom - (timer1Val - timer1ValCountStart + timer1ValCountPassedTime));
      // Toggle colons.
      colon1 = clock05Hz;
      colon2 = clock05Hz ^ 1;
      break;

    // Ctu paused.
    case 4:
      CASE_SETUP()

      // Cancel.
      if(buttons.reset > 0) {
        buttons.reset = 0;
        stateCurrent  = 0;  // Goto case 0.
        break;
      }

      // Resume.
      if(buttons.start > 0) {
        buttons.start = 0;
        stateCurrent  = 2;  // Goto case 2.
        break;
      }

      // Toggle colons.
      colon1 = clock05Hz;
      colon2 = clock05Hz;
      // Toggle text.
      if(clock05Hz == 1) {
        display.display(0, timer1Val - timer1ValCountStart);
      } else {
        display.displaySegments(segmentsPaused);
      }
      break;

    // Ctd paused.
    case 5:
      CASE_SETUP(timer1ValCountPassedTime = timer1Val - timer1ValCountStart + timer1ValCountPassedTime;)

      // Cancel.
      if(buttons.reset > 0) {
        buttons.reset = 0;
        stateCurrent  = 1;  // Goto case 1.
        break;
      }

      // Resume.
      if(buttons.start > 0) {
        buttons.start = 0;
        stateCurrent  = 3;  // Goto case 3.
        break;
      }

      // Toggle colons.
      colon1 = clock05Hz;
      colon2 = clock05Hz;
      // Toggle text.
      if(clock05Hz == 1) {
        display.display(1, timer1ValCountOnPausedTime);
      } else {
        display.displaySegments(segmentsPaused);
      }
      break;

    // Ctu ended.
    case 6:
      CASE_SETUP()

      // Text.
      display.displaySegments(segmentsOwerflow);
      break;

    // Ctd ended.
    case 7:
      CASE_SETUP()
      // Cancel.
      if(buttons.reset > 0) {
        buttons.reset = 0;
        stateCurrent  = 1;  // Goto case 1.
        break;
      }

      // Resume.
      if(buttons.start > 0) {
        buttons.start = 0;
        stateCurrent  = 1;  // Goto case 3.
        break;
      }

      // Toggle colons.
      colon1 = clock05Hz;
      colon2 = clock05Hz;
      // Text.
      display.displaySegments(segmentsEnded);
      break;
  }


  /*// Blink the colons.
  uint8_t clock05Hz = timer1Val / 100 % 2;
  digitalWrite(GPIO_COLON1, clock05Hz);
  digitalWrite(GPIO_COLON2, clock05Hz ^ 1);
  // Display stuff.
  display.display(buttons.mode, timer1Val);*/

  display.push();
  digitalWrite(GPIO_COLON1, colon1);
  digitalWrite(GPIO_COLON2, colon2);
  display.clear();
  delay(50);
}