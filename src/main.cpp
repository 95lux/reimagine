#include "DMX.h"
#include "EuclideanLoops.h"
#include "ManageEEPROM.h"
#include "define.h"
#include "variables.h"
#include <Arduino.h>
#include <avr/sleep.h>

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

void setup() {
    sec = 0L;
#ifdef DEBUG
    Serial.begin(9600);
#endif
    sec = readLongFromEEPROM();
    setupLightObjects(sec);
    pinMode(12, INPUT_PULLUP);
}

void loop() {
    unsigned long millisecs = millis();
    storeButton = digitalRead(13);
    resetButton = digitalRead(12);

    if (storeButton == LOW) {
        switchOffLights();
        if (millisecs - storeButtonLastSwitched >= 20000) {
            // write to eeprom after 20 secs if no power is available, then disable DMX (Output on Pin 2,3)
            writeLongToEEPROM(sec);
            disableDMX();
        }
        storeButtonLastSwitched = millisecs;
    } else if (storeButton == HIGH) {
        if (storeButton != storeButtonLastState) {
            setupDMX();
        }
        checkEuclideanStates(sec, millisecs);
    }

    if (resetButton != resetButtonLastState && resetButton == LOW) {
        resetCounter();
    }

    storeButtonLastState = storeButton;
    resetButtonLastState = resetButton;

    if (millisecs - lastRefreshTime1000ms >= 1000) {
        // updateTC in 1 sec intervals
        lastRefreshTime1000ms = millisecs;
        sec++;
        // updateTC();
    }

    // drawState(sec);
}
