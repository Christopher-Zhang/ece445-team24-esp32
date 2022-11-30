#include <Arduino.h>
#include "constants.h"
#include "characteristicCallbacks.h"
#include "belay.h"

void BelayCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if(value.length() > 0) {
        Serial.println("________");
        Serial.print("New value: "); 
        for(int i = 0; i < value.length(); i++) {
        Serial.print(value[i]);
        }
        Serial.println();
        Serial.println("________");
    }
    // if(value == "on1") {
    //     digitalWrite(BUILTIN_LED, HIGH);
    // }
    // if(value == "off1") {
    //     digitalWrite(BUILTIN_LED, LOW);
    // }
    // if(value == "on2") {
    //     digitalWrite(GPIO_NUM_4, HIGH);
    // }
    // if(value == "off2") {
    //     digitalWrite(GPIO_NUM_4, LOW);
    // }
    // if(value == "belay") {
    //     belay();
    // }
    // if(value == "stop") {
    //     int lower_flag = 0;
    //     stop(lower_flag);
    // }
    // if(value == "lower") {
    //     int lower_flag = 1;
    //     stop(lower_flag);
    // }
}