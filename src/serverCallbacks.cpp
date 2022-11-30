
#include "serverCallbacks.h"

void BelayServerCallbacks::onConnect(BLEServer * pServer) {
    Serial.print("Successfully connected to ");
    Serial.println(pServer->getConnectedCount());
}

void BelayServerCallbacks::onDisconnect(BLEServer * pServer) {
    Serial.print("Device disconnected");
}
