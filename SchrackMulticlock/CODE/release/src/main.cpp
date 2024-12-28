/**
 * Timer("Multiclock")
 */

/**
 * display layout
 * count up   count up
 * hh:mm:ss   mm:ss.(:)ss
 */
// clang-format off
// clang-format on

/*
millis = 0x14996FF

ms = millis % 100
S = int((millis - ms) / 100 % 60)
M = int((millis - S - ms) / 6000 % 60)
H = int((millis - M - S - ms) / 360000% 60)

print(str(H) + ":" + str(M) + ":"  + str(S) + "." + str(ms))
 */


#include <Arduino.h>
#include "maxDriver.h"
#include "buttons.h"


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
  display.spiSend(display.maxRegisters.digitArray[5], 0b01001111);
  display.spiSend(display.maxRegisters.digitArray[4], 0b00000101);
  display.spiSend(display.maxRegisters.digitArray[3], 0b00000101);
  display.spiSend(display.maxRegisters.digitArray[2], 0b00000000);
  display.spiSend(display.maxRegisters.digitArray[1], 0b00001001);
  display.spiSend(display.maxRegisters.digitArray[0], 0b01111001);
  /*display.spiSend(display.maxRegisters.digitArray[5], display.maxRegisters.numberCharArray[5]);
  display.spiSend(display.maxRegisters.digitArray[4], display.maxRegisters.numberCharArray[4]);
  display.spiSend(display.maxRegisters.digitArray[3], display.maxRegisters.numberCharArray[3]);
  display.spiSend(display.maxRegisters.digitArray[2], display.maxRegisters.numberCharArray[2]);
  display.spiSend(display.maxRegisters.digitArray[1], display.maxRegisters.numberCharArray[1]);
  display.spiSend(display.maxRegisters.digitArray[0], display.maxRegisters.numberCharArray[0]);*/
}


void diplayCountup() {
  display.spiSend(display.maxRegisters.digitArray[5], 0b00000101);
  display.spiSend(display.maxRegisters.digitArray[4], 0b01001111);
  display.spiSend(display.maxRegisters.digitArray[3], 0b01110111);
  display.spiSend(display.maxRegisters.digitArray[2], 0b00111101);
  display.spiSend(display.maxRegisters.digitArray[1], 0b00111011);
  display.spiSend(display.maxRegisters.digitArray[0], 0b00000000);
}


void setup() {
  display.displaySetup();  // Set up the display.
  buttons.setup();         // Set pinmodes and attach the interrupt.
  mode.set(buttons.mode);
}


/**
 * ================
 * mode map
 * ----------------
 * default  - err screen
 * 0        - timer.
 * 1        - stopwatch.
 * 2        - running.
 * 3        - paused.
 * 4        - ended.
 * ================
 */
void loop() {
  Serial.println("Loop");
  uint32_t        millisCur  = 0;
  static uint32_t millisLast = millisCur;
  millisCur                  = millis();

  static int8_t   countDir = 0;
  static uint32_t time     = 0;
  if(time > 0x14996FF) {
    mode.set(-1);
  }

  static uint8_t  digitCursor = 0;
  static uint32_t remainingNum;
  static uint8_t  tMs;
  static uint8_t  tS;
  static uint8_t  tM;
  static uint8_t  tH;

  switch(mode.current()) {
    default:  // Err screen.
      if(mode.current() != mode.last()) {
        mode.set(-1);
        diplayErrorScreen();  // Display error screen.
      }
      break;


    case 0:  // Timer.
      if(mode.current() != mode.last()) {
        mode.set(0);
        countDir    = -1;
        digitCursor = 0;
        time        = 0;
      }
      display.display(0, time);
      mode.set(buttons.mode);

      remainingNum = time;  // This is really bad.
      tMs          = remainingNum % 100;
      remainingNum -= tMs;
      tS = remainingNum / 100 % 60;
      remainingNum -= tS;
      tM = remainingNum / 6000 % 60;
      remainingNum -= tM;
      tH = remainingNum / 360000 % 60;

      display.display(1, time);

      for(uint8_t i = 0; i < 256; i++) {  // Shift cursor.
        if(buttons.dSelect < 1) {
          break;
        }

        buttons.dSelect--;
        digitCursor = (digitCursor + 1) % 6;
      }

      for(uint8_t i = 0; i < 256; i++) {  // Increase time. // +15s.
        if(buttons.timeInc < 1) {
          break;
        }

        buttons.timeInc--;
        time += 15 * 100;
      }

      for(uint8_t i = 0; i < 256; i++) {  // Decrease time. // +60s.
        if(buttons.timeDec < 1) {
          break;
        }

        buttons.timeDec--;
        time += 60 * 100;
      }

      for(uint8_t i = 0; i < 256; i++) {  // Reset time.
        if(buttons.reset < 1) {
          break;
        }

        buttons.reset--;
        time = 0;
      }

      if(buttons.start) {
        buttons.start = 0;
        mode.setCur(2);
        // break;
      }


      time = tH * 360000 + tM * 6000 + tS * 100 + tMs;  // This is bad.
      break;


    case 1:  // Stopwatch.
      if(mode.current() != mode.last()) {
        mode.set(1);
        countDir = 1;
        time     = 0;
        diplayCountup();
      }
      mode.set(buttons.mode);
      break;


    case 2:  // Running.
      if(mode.current() != mode.last()) {
        mode.set(2);
      }
      if(time == 0) {
        mode.setCur(4);
        break;
      }

      if(time + countDir * (millisCur - millisLast) <= 0) {
        mode.setCur(4);
        break;
      }

      if(time + countDir * (millisCur - millisLast) > 0x14996FF) {  // So what this code will have bugs and glitches?.
        mode.setCur(4);
        break;
      }

      if(buttons.reset > 0) {
        buttons.reset = 0;
        mode.setCur(buttons.mode);
      }

      if(buttons.start > 0) {
        buttons.start = 0;
        mode.setCur(3);
      }

      time = time + countDir * (millisCur - millisLast);
      display.display((countDir == 1 ? 0 : 1), time);  // It's just "wrong" and I don't care.
      break;


    case 3:  // Paused.
      if(mode.current() != mode.last()) {
        mode.set(3);
      }

      if(buttons.start > 0) {
        buttons.start = 0;
        mode.setCur(2);
      }
      break;


    case 4:  // Ended.
      if(mode.current() != mode.last()) {
        mode.set(4);
      }

      if(buttons.start > 0 || buttons.reset > 0) {
        buttons.start = 0;
        buttons.reset = 0;
        mode.setCur(buttons.mode);
      }

      break;
  }

  log_e("%i", mode.current());
  vTaskDelay(10);  // Replace with ups targetting.
}
