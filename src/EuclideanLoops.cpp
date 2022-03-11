#include "DMX.h"
#include "variables.h"
#include <Arduino.h>

class Light {
public:
    int id;
    boolean state;
    int val;
    int division;
    int fadeTime;
    unsigned long lastRefresh;

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

    void setFadeTime(int f) {
        fadeTime = f;
    }

    int getFadeTime() {
        return fadeTime;
    }

    unsigned long getLastRefresh() {
        return lastRefresh;
    }

    void setLastRefresh(unsigned long l) {
        lastRefresh += l;
    }

    Light() {
        state = false;
        lastRefresh = 0;
    }
};

const int arrayLength = sizeof(divisions) / sizeof(divisions[0]);
Light *lightObjects = new Light[arrayLength];

void setupLightObjects() {
    // Serial.println("setup lightObjects: ");
    for (int i = 0; i < arrayLength; i++) {
        lightObjects[i].setId(i);
        lightObjects[i].setDivision(divisions[i]);
        lightObjects[i].setVal(0);
        lightObjects[i].setFadeTime(fadeTimes[i]);
        // Serial.println((String) "ID: " + lightObjects[i].getId() + "FadeTime: " + lightObjects[i].getFadeTime());
    }
    setupDMX();
}

void triggerEuclideanStates(unsigned long count) {
    for (int i = 0; i < arrayLength; i++) {
        if (count % lightObjects[i].getDivision() == 0) {
            lightObjects[i].triggerState();
        }
    }
}

// void drawState(unsigned long count) {
//     for (int i = 0; i < arrayLength; i++) {
//         int val = lightObjects[i].getVal();
//         int len;
//         if (val == 0) {
//             len = 1;
//         } else {
//             len = trunc(log10(val)) + 1;
//         }
//         Serial.print(val);
//         // Serial.print((String) " lRT: " + lightObjects[i].getLastRefresh());
//         for (int i; i < 12 - len; i++) {
//             Serial.print(" ");
//         }
//         Serial.print(" | ");
//     }
//     Serial.println((String) "|" + count);
// }

void checkEuclideanStates(unsigned long count) {
    for (int i = 0; i < arrayLength; i++) {

        Light &currentLight = lightObjects[i];
        boolean currentState = currentLight.getState();

        setDMXVal(i + 1, currentLight.getVal());

        int specificTickrate = ((long)currentLight.getFadeTime() * 1000L) / 256;

        if (count - currentLight.getLastRefresh() >= specificTickrate) {
            currentLight.setLastRefresh(specificTickrate);

            if (currentState && currentLight.getVal() < 255) {
                currentLight.incrementVal();

                setDMXVal(i + 1, currentLight.getVal());
            } else if (!currentState && currentLight.getVal() > 0) {

                currentLight.decrementVal();
                setDMXVal(i + 1, currentLight.getVal());
            }
        }
    }
}
