#include <Arduino.h>

#include "Consts.h"
#include "Driver.h"
#include "LedControl.h"

void setup() {
    displaySetup();
}

void loop() {
    switch(0) {
        case 0:
            displaySetNum(D_TIME, 1234);
            break;

        default:

            break;
    }
}