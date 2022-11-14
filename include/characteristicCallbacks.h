#pragma once
#include <BLECharacteristic.h>
class BelayCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic);
};