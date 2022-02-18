#include "EuclideanLoopds.h"
#include "ManageEEPROM.h"
#include <Arduino.h>

unsigned long sec;
static unsigned long lastRefreshTime20ms = 0;
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
    Serial.begin(9600);
    setupLightObjects();
    sec = 0;
    pinMode(13, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    sec = readLongFromEEPROM();
}

void clk() {
    if (millis() - lastRefreshTime20ms >= 5) {
        // increment lastRefreshTime in 20ms intervals
        lastRefreshTime20ms += 5;
        // update DMX in 20ms intervals
        checkEuclideanStates(sec);
    }
    if (millis() - lastRefreshTime1000ms >= 1000) {
        // updateTC in 1 sec intervals
        lastRefreshTime1000ms += 1000;
        updateTC();
    }
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
    clk();
    drawState();
}
