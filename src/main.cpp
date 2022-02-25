#include "EuclideanLoopds.h"
#include "ManageEEPROM.h"
#include "variables.h"
#include <Arduino.h>

unsigned long sec;
static unsigned long lastRefreshTime1000ms = 0;
int storeButton;
int storeButtonLastState;
int resetButton;
int resetButtonLastState;

void resetCounter() {
    sec = 0L;
}

void updateTC() {
    sec++;
    // Serial.println(sec);
    triggerEuclideanStates(sec);
}

void setup() {
    sec = 0L;
    // Serial.begin(9600);
    sec = readLongFromEEPROM();
    setupLightObjects();
    pinMode(13, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
}

void loop() {
    storeButton = digitalRead(13);
    resetButton = digitalRead(12);

    if (storeButton != storeButtonLastState) {
        if (storeButton == LOW) {
            writeLongToEEPROM(sec);
        }
    }
    if (resetButton != resetButtonLastState) {
        if (resetButton == LOW) {
            resetCounter();
        }
    }

    storeButtonLastState = storeButton;
    resetButtonLastState = resetButton;

    unsigned long milisecs = millis();

    checkEuclideanStates(milisecs);

    if (milisecs - lastRefreshTime1000ms >= 1000) {
        // updateTC in 1 sec intervals
        lastRefreshTime1000ms += 1000;
        updateTC();
    }
    // drawState(sec);
}
