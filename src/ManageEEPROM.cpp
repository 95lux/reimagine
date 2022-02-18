#include <Arduino.h>
#include <EEPROM.h>

int address = 0x1;

void printByte(byte byte) {
    for (int i = 7; i >= 0; i--) {
        // Serial.print(bitRead(byte,i));
    }
}

void writeLongToEEPROM(long number) {
    for (int i = 3; i >= 0; i--) {
        int byteFraction = number >> (8 * i);
        byte byte = byteFraction & 0xFF; // = 0b11111111 <- Bitmaske
        EEPROM.write(address + i, byte);
    }
    // Serial.println("stored to EEPROM");
}

long readLongFromEEPROM() {
    unsigned long number = 0L;
    for (int i = 3; i >= 0; i--) {
        int byte = EEPROM.read(address + i);
        int byteFraction = byte << (8 * i);
        number = number | byteFraction;
    }
    // Serial.println((String)number + " read from EEPROM");
    return number;
}