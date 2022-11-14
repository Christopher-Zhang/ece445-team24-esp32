
#include "serverCallbacks.h"

void BelayServerCallbacks::onConnect(BLEServer * pServer) {
    Serial.print("Connected to ");
    Serial.println(pServer->getConnectedCount());
}
