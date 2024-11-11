#include <Arduino.h>
#include "pins.h"
#include "buttons.h"


void setup() {
  Serial.begin(115200);
  buttonsSetup();
}

void loop() {
  Serial.print(buttons.main);
  Serial.print("\t");
  Serial.println(digitalRead(GPIO_BTN_MAIN));
  delay(100);
}
