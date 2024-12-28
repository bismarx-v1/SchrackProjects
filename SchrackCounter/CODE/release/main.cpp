/**
 * Button("Counter")
 */

/**
 * save trigger count
 * save each game
 * add an sd card setup timeout(max attempts)
 */

/**
 * PROTOTYPE BOARD USES DIFFERENT PINS
 * PROTOTYPE BOARD USES DIFFERENT DISPLAY LAYOUT
 * 
 * display layout
 * release          prototype
 * ## ##  time      0#######
 * ###    score     score time
 */
// clang-format off
// clang-format on

// BUILD SETTINGS.
#define IS_PROTOTYPE_LAYOUT 0  // Building for the prototype?
#define NATS_DEBUG          1  // Debug?
#define NATS_SD_CARD        1  // Sd card?

// Constants.
#define GAME_LENGTH           10 * 1000  // Game length in millis.
#define GAME_FREEZE_LENGTH    5000
#define GAME_END_BLINK_LENGTH 500

#include <Arduino.h>
#include "macros.h"
#include "maxDriver.h"
#include "buttons.h"
#include "sdCardStuff.h"


struct thingMode {
private:
  uint8_t currentVal = 0;
  uint8_t lastVal    = -1;

public:
  uint8_t current() {
    return currentVal;
  }

  uint8_t last() {
    return lastVal;
  }

  void set(uint8_t val) {
    lastVal    = currentVal;
    currentVal = val;
  }

  void setCur(uint8_t val) {
    currentVal = val;
  }
};
thingMode mode;


/**
 * @brief Diplays the error screen.
 */
void diplayErrorScreen() {
  display.spiSend(display.maxRegisters.digit7, 0b01001111);
  display.spiSend(display.maxRegisters.digit6, 0b00000101);
  display.spiSend(display.maxRegisters.digit5, 0b00000101);
  display.spiSend(display.maxRegisters.digit4, 0b00000001);
  display.spiSend(display.maxRegisters.digit3, 0b01110110);
  display.spiSend(display.maxRegisters.digit2, 0b00110011);
  display.spiSend(display.maxRegisters.digit1, 0b01110111);
  display.spiSend(display.maxRegisters.digit0, 0b00000000);
}


/**
 * @brief Displays high score text.
 * @note Numbers not included.
 */
void highScoreTextScreen() {
  display.spiSend(display.maxRegisters.digitArray[7], 0b00110111);
  display.spiSend(display.maxRegisters.digitArray[6], 0b00000110);
  display.spiSend(display.maxRegisters.digitArray[5], 0b00000000);
  display.spiSend(display.maxRegisters.digitArray[4], 0b00000000);
}


void setup() {
  NAT_MODE_DEBUG(Serial.begin(115200); delay(2000); Serial.println("setup");)  // Debug mode.


  display.displaySetup();                          // Set up the display.
  buttonsSetup();                                  // Set pinmodes and attach the interrupt.
  #if NATS_SD_CARD == 1
  log_e("a");
  display.spiSend(display.maxRegisters.digitArray[7], 0b00001110);
  display.spiSend(display.maxRegisters.digitArray[6], 0b01111110);
  display.spiSend(display.maxRegisters.digitArray[5], 0b01110111);
  display.spiSend(display.maxRegisters.digitArray[4], 0b00111101);
  display.spiSend(display.maxRegisters.digitArray[3], 0b01011011);
  display.spiSend(display.maxRegisters.digitArray[2], 0b00111101);
  NAT_MODE_SD_CARD(sdSetup(GPIO_SPI_SEL_SDCARD);)  // Use sd card?
  log_e("b");
  #endif
}


/**
 * ================
 * mode map
 * ----------------
 * default  - err screen
 * 0        - main menu.
 * 1        - game running.
 * 2        - game ended, save data and wait a bit.
 * ================
 */
void loop() {
  NAT_MODE_DEBUG(Serial.println("loop");)
  // File.
  char          filename[14] = "/database.csv";  // Include the \0.
  const uint8_t filenameLen  = 14;

  static uint16_t hiScore = 0;  // High score var.

  static uint8_t setupInLoop = 0;
  if(setupInLoop == 0) {
    setupInLoop               = 1;
    uint16_t bufferReadLen[1] = {7};  // Create a buffer to write to.
    uint8_t  bufferRead[bufferReadLen[0]];
    memset(bufferRead, 0, bufferReadLen[0]);
    readCell(filename, 0, 1, bufferRead, bufferReadLen);  // Read a cell.
    String bufferReadString = (char*)bufferRead;
    hiScore                 = bufferReadString.toInt();
  }


  static uint16_t score = 0;  // Last/this run score variable.
  static uint64_t timeGameStartOrEnd =
  0;  // Var that keeps track of when this entered mode 1 or 2. Length of these modes depends on this var.
  static uint64_t timeGameFreezeLastTrig = 0;  // Time keeping var for the ending(mode 2) blinking.
  static uint8_t  gameFreezeDisplayState = 1;  // State of the zeros on the ending screen (The zeros blink).

  switch(mode.current()) {
    default:  // Err screen.
      if(mode.current() != mode.last()) {
        mode.set(-1);
        diplayErrorScreen();  // Display error screen.
      }
      break;


    case 0:  // Main menu.
      if(mode.current() != mode.last()) {
        mode.set(0);
        buttons.main = 0;
        display.display(display.DIS_SCORE, hiScore, -1);
        highScoreTextScreen();  // High score.
      }

      if(buttons.main > 0) {
        buttons.main--;
        mode.setCur(1);
      }
      break;


    case 1:  // Game running.
      if(mode.current() != mode.last()) {
        mode.set(1);
        buttons.main       = 0;
        timeGameStartOrEnd = millis();
        score              = 0;
      }

      for(uint8_t i = 0; i < 256; i++) {
        if(buttons.main == 0) {
          break;
        }
        buttons.main--;
        score++;
      }

#if 0
      static uint8_t  whileProtection        = 0; 
      while(buttons.main > 0 && whileProtection < 255) {
        whileProtection++;
        buttons.main--;
        score++;
        /*if(timeGameStartOrEnd + GAME_LENGTH < millis()) {
          mode.setCur(2);
        }*/
      }
      whileProtection = 0;
#endif

      display.display(display.DIS_SCORE, score, -1);

      if(timeGameStartOrEnd + GAME_LENGTH < millis()) {
        mode.setCur(2);
        display.display(display.DIS_TIME, 0, 2);
      } else {
        display.display(display.DIS_TIME, (GAME_LENGTH + (timeGameStartOrEnd - millis())) / 10, 2);
      }
      break;


    case 2:  // Game ended freeze period
      if(mode.current() != mode.last()) {
        mode.set(2);
        timeGameStartOrEnd = millis();
        if(score > hiScore) {  // Set high score.
          hiScore = score;

          String  val1String = String(hiScore);
          uint8_t val1Len    = val1String.length();
          char    val1[val1Len];
          val1String.toCharArray(val1, val1Len);
          writeCell(filename, filenameLen, 0, 1, val1, val1Len, 0);  // Write a shorter val. //DEBUG
        }
      }

      if(timeGameFreezeLastTrig + GAME_END_BLINK_LENGTH < millis()) {
        timeGameFreezeLastTrig = millis();
        if(gameFreezeDisplayState == 1) {
          display.display(display.DIS_TIME, 0x7fff, -1);  // Clear. Dp is somehow wrong.
        } else {
          display.display(display.DIS_TIME, 0, 2);  // Show zeros. Dp is somehow wrong.
        }
        gameFreezeDisplayState = 1 - gameFreezeDisplayState;
      }


      if(timeGameStartOrEnd + GAME_FREEZE_LENGTH < millis()) {
        mode.setCur(0);
      }
      break;
  }

#if DEBUG_NATS
  Serial.println(buttons.main);
  if(Serial.available() > 0) {
    uint8_t incomingByte = Serial.read();
    Serial.println(incomingByte, HEX);
    buttons.main = buttons.main + incomingByte;
    Serial.println(buttons.main, HEX);
    Serial.println(mode.current(), HEX);
  }
#endif

  vTaskDelay(10);  // Replace with ups targetting.
  //vTaskDelay(5);
}
