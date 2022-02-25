#include "Conceptinetics.h"

#define DMX_MASTER_CHANNELS 12
#define RXEN_PIN 2

DMX_Master dmx_master(DMX_MASTER_CHANNELS, RXEN_PIN);

void setupDMX() {
    dmx_master.enable();
    dmx_master.setChannelRange(1, 12, 255);
}

void setDMXVal(int id, int val) {
    dmx_master.setChannelValue(id, val);
}