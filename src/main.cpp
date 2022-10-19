#include "DMX.h"
#include "EuclideanLoops.h"
#include "ManageEEPROM.h"
#include "variables.h"
#include <Arduino.h>
#include <avr/sleep.h>

// #define DEBUG

unsigned long sec;
static unsigned long lastRefreshTime1000ms = 0;
int storeButton;
int storeButtonLastState;
unsigned long storeButtonLastSwitched = 0;
int resetButton;
int resetButtonLastState;

void resetCounter() {
    sec = 0L;
}

void updateTC() {
    sec++;
// Serial.println(sec);
#ifndef DEBUG
    triggerEuclideanStates(sec);
#endif
}

void setup() {
    sec = 0L;
#ifdef DEBUG
    Serial.begin(9600);
#endif
    sec = readLongFromEEPROM();
#ifndef DEBUG
    setupLightObjects();
#endif
    pinMode(12, INPUT_PULLUP);
}

void loop() {
    unsigned long milisecs = millis();
    storeButton = digitalRead(13);
    resetButton = digitalRead(12);

    if (storeButton == LOW) {
        switchOffLights();
        if (milisecs - storeButtonLastSwitched >= 20000) {
            // write to eeprom after 20 secs if no power is available, then go to sleep
            writeLongToEEPROM(sec);
            disableDMX();
        }
        storeButtonLastSwitched = milisecs;
    } else if (storeButton == HIGH) {
        if (storeButton != storeButtonLastState) {
            setupDMX();
        }
        checkEuclideanStates(milisecs);
    }

    if (resetButton != resetButtonLastState && resetButton == LOW) {
        resetCounter();
    }

    storeButtonLastState = storeButton;
    resetButtonLastState = resetButton;

    if (milisecs - lastRefreshTime1000ms >= 1000) {
        // updateTC in 1 sec intervals
        lastRefreshTime1000ms += 1000;
        updateTC();
    }

    // drawState(sec);
}
