#pragma once

// #ifndef SERVER_CALLBACKS_H
// #define SERVER_CALLBACKS_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class BelayServerCallbacks: public BLEServerCallbacks{
    public:
        void onConnect(BLEServer * pServer);
        void onDisconnect(BLEServer * pServer);
};

// #endif