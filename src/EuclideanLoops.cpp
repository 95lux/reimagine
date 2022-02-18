// #include "DMX.h"
#include "variables.h"
#include <Arduino.h>

class Light {
public:
    int id;
    boolean state;
    int val;
    int division;

    void setId(int i) {
        id = i;
    }

    int getId() {
        return id;
    }

    void triggerState() {
        state ^= true;
        // Serial.println((String) "id: " + id + " triggered to: " + state);
    }

    boolean getState() {
        return state;
    }

    void setDivision(int d) {
        division = d;
    }

    int getDivision() {
        return division;
    }

    int getVal() {
        return val;
    }

    void incrementVal() {
        val = ++val;
    }

    void decrementVal() {
        val = --val;
    }

    void setVal(int v) {
        val = v;
    }
    Light() {
        state = false;
    }
};

const int arrayLength = sizeof(divisions) / sizeof(divisions[0]);
Light *lightObjects = new Light[arrayLength];
int fadeLength = 2;

void setupLightObjects() {
    // Serial.println("setup lightObjects: ");
    for (int i = 0; i < arrayLength; i++) {
        lightObjects[i].setId(i);
        lightObjects[i].setDivision(divisions[i]);
        lightObjects[i].setVal(0);
    }
    // setupDMX();
}

void triggerEuclideanStates(long count) {
    for (int i = 0; i < arrayLength; i++) {
        if (count % lightObjects[i].getDivision() == 0) {
            lightObjects[i].triggerState();
        }
    }
}

void drawState() {
    for (int i = 0; i < arrayLength; i++) {
        int val = lightObjects[i].getVal();
        int len;
        if (val == 0) {
            len = 1;
        } else {
            len = trunc(log10(val)) + 1;
        }
        Serial.print(val);
        for (int i; i < 6 - len; i++) {
            Serial.print(" ");
        }
        Serial.print(" | ");
    }
    Serial.println();
}

//
void checkEuclideanStates(long count) {
    for (int i = 0; i < arrayLength; i++) {
        // setDMXVal(i + 1, lightObjects[i].getVal());
        // Light &currentLight = lightObjects[i];
        boolean currentState = lightObjects[i].getState();
        if (currentState) {
            if (lightObjects[i].getVal() <= 255) {
                lightObjects[i].incrementVal();
                // setDMXVal(i + 1, lightObjects[i].getVal());
            }
        } else if (!currentState) {
            if (lightObjects[i].getVal() > 0) {
                lightObjects[i].decrementVal();
                // setDMXVal(i + 1, lightObjects[i].getVal());
            }
        }
    }
}
