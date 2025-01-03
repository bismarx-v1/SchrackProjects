#include <Arduino.h>
#include "macrosTimersV2.h"


// Main timer.
hw_timer_t* timerMainHandle = NULL;
#define TIMER_READ_DIVIDER 10000


// Setup.
void setup() {
  // Serial.
  Serial.begin(115200);  //DEBUG
  delay(2000);           //DEBUG

  // Create the timer.
  N_TIMER_BEGIN(timerMainHandle, 1, 8000)
  N_TIMER_START(timerMainHandle)
}


void loop() {
  static uint64_t val = 0;
  N_TIMER_READ(timerMainHandle, val, TIMER_READ_DIVIDER)
  Serial.println("Loop");
  Serial.println(val);
  if(val >= 1000) {
    N_TIMER_WRITE(timerMainHandle, 100, TIMER_READ_DIVIDER)
    N_TIMER_READ(timerMainHandle, val, TIMER_READ_DIVIDER)
    Serial.print("Reset: ");
    Serial.println(val);
  }
  delay(100);
}