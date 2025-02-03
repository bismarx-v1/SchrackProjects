/**
 * This now works but has no anims.
 * Mode (0|1) - (hh:mm:ss(ctu)|mm:ss.ss(ctd)).
 * Display time max 0x22550FF - 99:59:59.99
*/
/**
 * CHECK:
 * TODO:
 * FIX:
 */

#include <Arduino.h>
#include "maxDriverButtonV2.h"
#include "macrosTimersV2.h"
#include "schrackButtonButtons.h"


// Constants.
#define NUMBER_OF_DIGITS        6    // Number of digits on the display.
#define NUMBER_OF_DIGITS_DRIVER 8    // Number of digits on the driver.
#define GAME_LENGTH_PAUSE       400  // Game length - 15 seconds.


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


/**
 * @brief Updates the timer1Hz var from the timerVar.
 */
#define TIMER_1HZ_UPDATE()                                                                                                     \
  { timer1Hz = timerVal % 100 / 50; }

/**
 * @brief Updates the timer2Hz var from the timerVar.
 */
#define TIMER_2HZ_UPDATE()                                                                                                     \
  { timer2Hz = timerVal % 100 / 25 % 2; }

// Main timer.
hw_timer_t* timerMainHandle = NULL;
#define TIMER_READ_DIVIDER 10000


void setup() {
  Serial.begin(115200);  //DEBUG
  delay(2000);           //DEBUG

  // Setups.0
  display.setup();                  // Display.
  pinMode(GPIO_LED_GREEN, OUTPUT);  // Some leds.
  pinMode(GPIO_LED_RED, OUTPUT);
  buttons.setup();  // Buttons.

  // Create the timer.
  N_TIMER_BEGIN(timerMainHandle, 1, 8000)
}


void loop() {
  uint32_t timeLoopStart = micros();
  // States.
  static uint8_t stateCurrent = 0;
  static uint8_t stateLast    = -1;

  // Constant text patterns.
  const uint8_t segmentsErr[NUMBER_OF_DIGITS_DRIVER] = {0b1001111, 0b101, 0b101, 0, 0b1110110, 0b111011, 0b1110111, 0};
  const uint8_t segmentsEnd[NUMBER_OF_DIGITS_DRIVER] = {0b1001111, 0b10101, 0b111101, 0, 0, 0, 0, 0};
  const uint8_t segmentsHI[NUMBER_OF_DIGITS_DRIVER]  = {0b110111, 0b110000, 0, 0, 0, 0, 0, 0};

  // Game lengths.
  const uint8_t  gameLengthsArrayLength                   = 3;
  const uint16_t gameLengthsArray[gameLengthsArrayLength] = {500, 1500, 3000};
  static uint8_t gameLengthsArrayIndex                    = 1;

  // Score stuff.
  static uint16_t score                             = 1;    // Current score.
  static uint16_t scoreGameNumber                   = 0;    // Current score.
  static uint16_t highScore[gameLengthsArrayLength] = {0};  // Max reached score.
  static uint16_t highScoreGameNumber               = 0;    // Number of the game that reached the high score
  static uint8_t  isHighScore                       = 0;

  // Timer values.
  static uint64_t timerVal;  // Timer time val.
  static uint8_t  timer1Hz;  // 1Hz pulse based on the timer.
  static uint8_t  timer2Hz;  // 2Hz pulse based on the timer.

  // Display additions.
  static uint8_t decimalPoints = 0;
  static uint8_t ledsGreen     = 0;
  static uint8_t ledsRed       = 0;

  /**
   * case - desc
   * -----------
   * default - Error screen
   * 0 - High score
   * 1 - Game running
   * 2 - Game end
  */
#define caseHighScore   0
#define caseGameRunning 1
#define caseGameEnd     2

  switch(stateCurrent) {
    // Error screen.
    default:
      CASE_ON_ENTER()
      display.displaySegments(segmentsErr);
      CASE_ON_EXIT(buttons.clear();)
      break;

    // High score.
    case caseHighScore:
      CASE_ON_ENTER()
      // Register buttons.
      if(buttons.primary > 0) {  // Start.
        buttons.primary = 0;
        stateCurrent    = caseGameRunning;
      } else if(buttons.mode > 0) {  // Change game length.
        buttons.mode          = 0;
        gameLengthsArrayIndex = (gameLengthsArrayIndex + 1) % gameLengthsArrayLength;
      }
      // Display.
      display.displaySegments(segmentsHI);
      display.display(display.SCR_SCORE, highScore[gameLengthsArrayIndex]);

      CASE_ON_EXIT(buttons.clear();)
      break;

    // Game running.
    case caseGameRunning:
      CASE_ON_ENTER(N_TIMER_RESET(timerMainHandle) N_TIMER_START(timerMainHandle) score = 1; scoreGameNumber++;)
      // Read timer
      N_TIMER_READ(timerMainHandle, timerVal, TIMER_READ_DIVIDER)

      // Update the timer.
      TIMER_1HZ_UPDATE()
      decimalPoints = decimalPoints & 0b10111111 | (timer1Hz << 6);
      // Display.
      display.display(display.SCR_TIME, gameLengthsArray[gameLengthsArrayIndex] - timerVal);
      display.display(display.SCR_SCORE, score);

      // Register buttons.
      if(timerVal >= gameLengthsArray[gameLengthsArrayIndex]) {  // Gmae ended.
        stateCurrent = caseGameEnd;
      } else {
        if(buttons.primary > 0) {  // Button pressed.
          for(uint8_t i = 0; i < 256; i++) {
            score++;
            buttons.primary--;
            if(buttons.primary == 0) {
              break;
            }
          }
        }
        if(buttons.reset > 0) {  // Reset.
          stateCurrent = caseGameEnd;
        }
      }

      CASE_ON_EXIT(N_TIMER_STOP(timerMainHandle) buttons.clear();)
      break;

    // Game end.
    case caseGameEnd:
      CASE_ON_ENTER(
      N_TIMER_RESET(timerMainHandle) N_TIMER_START(timerMainHandle) isHighScore = 0;
      if(score > highScore[gameLengthsArrayIndex]) {
        highScore[gameLengthsArrayIndex] = score;
        highScoreGameNumber              = scoreGameNumber;
        isHighScore                      = 1;
      } if(isHighScore == 1) { tone(GPIO_BUZZER, 1000, 600); } else {
        tone(GPIO_BUZZER, 500, 300);
      })

      // Read timer.
      N_TIMER_READ(timerMainHandle, timerVal, TIMER_READ_DIVIDER)
      TIMER_1HZ_UPDATE()
      TIMER_2HZ_UPDATE()
      // Display.
      if(timer1Hz == 1) {
        display.display(display.SCR_TIME, 0);
        display.display(display.SCR_SCORE, score);
      } else {
        display.displaySegments(segmentsEnd);
        display.display(display.SCR_SCORE, score);
      }

      if(isHighScore == 1) {
        if(timer2Hz == 1) {
          ledsGreen = 0;
          ledsRed   = 1;
        } else {
          ledsGreen = 1;
          ledsRed   = 0;
        }
      }


      // State change.
      if(timerVal >= GAME_LENGTH_PAUSE) {
        stateCurrent = caseHighScore;
      }

      CASE_ON_EXIT(N_TIMER_STOP(timerMainHandle) buttons.clear(); ledsGreen = 0; ledsRed = 0;)
      break;
  }

  // Set the decimal points.
  if(decimalPoints > 0) {                                               // Check if any points need to be set.
    for(uint8_t digit = 0; digit < NUMBER_OF_DIGITS_DRIVER; digit++) {  // Go through all the digits.
      display.segmentsArrayObjGlobal[digit] =
      display.segmentsArrayObjGlobal[digit] | ((decimalPoints >> NUMBER_OF_DIGITS_DRIVER - 1 - digit) & 1)
                                              << NUMBER_OF_DIGITS_DRIVER - 1;  // Set the dP bit.
    }
  }

  // Send stuff to display.
  display.push();
  digitalWrite(GPIO_LED_GREEN, ledsGreen);
  digitalWrite(GPIO_LED_RED, ledsRed);
  display.clear();
  log_e("State current: %x.", stateCurrent);
  log_e("State last: %x.", stateLast);
  log_e("Loop length: %ius", micros() - timeLoopStart);
  delay(50);
}
