#include "DMX.h"
#include "variables.h"
#include <Arduino.h>

class Light {
public:
    int id;
    boolean state;
    int val;
    unsigned int pauseTime;
    unsigned int fadeTime;
    unsigned long lastStateChange;
    unsigned long lastTick;

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

    void setState(boolean s) {
        state = s;
    }

    boolean getState() {
        return state;
    }

    void setPauseTime(unsigned int p) {
        pauseTime = p;
    }

    int getPauseTime() {
        return pauseTime;
    }

    int getVal() {
        return val;
    }

    void incrementVal() {
        if (val <= 255)
            val++;
    }

    void decrementVal() {
        if (val > 0)
            val--;
    }

    void setVal(int v) {
        val = v;
    }

    void setFadeTime(int f) {
        fadeTime = f;
    }

    unsigned int getFadeTime() {
        return fadeTime;
    }

    unsigned long getLastStateChange() {
        return lastStateChange;
    }

    void setLastStateChange(unsigned long l) {
        lastStateChange = l;
    }

    unsigned long getLastTick() {
        return lastTick;
    }

    void setLastTick(unsigned long t) {
        lastTick = t;
    }

    Light() {
        // state = false;
        // lastStateChange = 0;
        lastTick = 0;
    }
};

const int arrayLength = sizeof(pauseTimes) / sizeof(pauseTimes[0]);
Light *lightObjects = new Light[arrayLength];

void setupLightObjects(unsigned long count) {
    // Serial.println("setup lightObjects: ");
    for (int i = 0; i < arrayLength; i++) {
        lightObjects[i].setId(i);
        lightObjects[i].setPauseTime(pauseTimes[i]);
        lightObjects[i].setVal(0);
        lightObjects[i].setFadeTime(fadeTimes[i]);

        int period = pauseTimes[i] + fadeTimes[i];
        int startedPeriod = count % period;

        if (startedPeriod < pauseTimes[i]) {
            // FadeOut
            lightObjects[i].setState(false);
            lightObjects[i].setVal(0);
            lightObjects[i].setLastStateChange(count - startedPeriod);
        } else {
            // FadeIn
            lightObjects[i].setState(true);
            lightObjects[i].setVal(((double)(startedPeriod - pauseTimes[i]) / fadeTimes[i]) * 255);
            lightObjects[i].setLastStateChange(count - startedPeriod + pauseTimes[i]);
            // Serial.print("startval:  ");
        }
        // Serial.println((String) "Laststatechange lamp " + (i + 1) + " " + lightObjects[i].getLastStateChange() + " val: " + lightObjects[i].getVal());
    }
#ifndef DEBUG
    setupDMX();
#endif
}
#ifdef DEBUG
void drawState(unsigned long secs) {
    for (int i = 0; i < arrayLength; i++) {
        int val = lightObjects[i].getVal();
        // int len;
        // if (val == 0) {
        //     len = 1;
        // } else {
        //     len = trunc(log10(val)) + 1;
        // }
        Serial.print(val);
        Serial.print("; ");
        Serial.print(lightObjects[i].getState());
        Serial.print("; ");
        Serial.print(secs - lightObjects[i].getLastStateChange());

        // Serial.print(lightObjects[i].getLastStateChange());
        // Serial.print((String) " lRT: " + lightObjects[i].getLastRefresh());
        // for (int i; i < 12 - len; i++) {
        //     Serial.print(" ");
        // }
        Serial.print(" | ");
    }
    Serial.println((String) "|" + secs);
}
#endif

void checkEuclideanStates(unsigned long count, unsigned long millisecs) {
    for (int i = 0; i < arrayLength; i++) {
        Light &currentLight = lightObjects[i];

#ifndef DEBUG
        setDMXVal(i + 1, currentLight.getVal());
#endif

        if (currentLight.getState() == false) {
            // Pause
            if (millisecs - currentLight.getLastTick() >= 1 && currentLight.getVal() > 0) {
                // FadeOut
                // Tickrate = () (100ms) / number of values (256)
                // int timeForStep = 100 / 256;
                // 1ms per Tick in Fadeout = 256ms per Fadeout
                // if (millisecs - currentLight.getLastTick() >= 1) {
                currentLight.decrementVal();
                setDMXVal(i + 1, currentLight.getVal());
                // setDMXVal(i + 1, 125);
                // currentLight.setLastTick(millisecs);
                // }
            } else if (currentLight.getVal() <= 0) {
                setDMXVal(i + 1, 0);
            }
            if (count - currentLight.getLastStateChange() >= currentLight.getPauseTime()) {
                currentLight.setState(true);
                currentLight.setLastStateChange(count);
            }
        } else if (currentLight.getState() == true) {
            // FadeIn
            // check milliseconds for calculation of smooth fade
            unsigned int timeForStep = ((long)currentLight.getFadeTime() * 1000L) / 255;
            if (millisecs - currentLight.getLastTick() >= timeForStep && currentLight.getVal() < 255) {
                currentLight.incrementVal();
                setDMXVal(i + 1, currentLight.getVal());
                currentLight.setLastTick(millisecs);
            } else if (millisecs - currentLight.getLastTick() >= timeForStep) {
                setDMXVal(i + 1, 255);
            }
            // check when was last state change to
            if (count - currentLight.getLastStateChange() >= currentLight.getFadeTime()) {
                currentLight.setState(false);
                currentLight.setLastStateChange(count);
            }
            // currentLight.setLastRefresh();
        }
    }
    // drawState(count);
}

void switchOffLights() {
#ifndef DEBUG
    for (int i = 0; i < arrayLength; i++) {
        setDMXVal(i + 1, 0);
    }
#endif
}