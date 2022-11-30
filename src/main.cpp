#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "serverCallbacks.h"
#include "characteristicCallbacks.h"
#include "constants.h"
#include "main.h"
#include "belay.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth not enabled: Run `make menuconfig`
#endif

// BLE instance
BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;


// init
void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(GPIO_NUM_4, OUTPUT);
  // pinMode(GPIO_NUM_0, OUTPUT);
  // pinMode(GPIO_NUM_32, INPUT); //setting the pin modes here
  // pinMode(GPIO_NUM_25, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(LEFT_D1, OUTPUT);


  Serial.begin(115200);
  while(!Serial){
    
  }
  Serial.println("Device started, starting server...");

  BLEDevice::init(DEVICE_NAME);
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE 
      | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic:: PROPERTY_INDICATE
      | BLECharacteristic::PROPERTY_WRITE_NR
  );

  pServer->setCallbacks(new BelayServerCallbacks());

  pCharacteristic->setCallbacks(new BelayCharacteristicCallbacks());
  pCharacteristic->setValue("Hello, world");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  BLEAdvertisementData advertisingData;
  advertisingData.setPartialServices(BLEUUID(SERVICE_UUID));
  advertisingData.setName(DEVICE_NAME);
  pAdvertising->setAdvertisementData(advertisingData);
  pAdvertising->start();
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(LEFT_D1, LOW);
}

void loop() {
  delay(1000);
  switch(state) {
    case STOP_STATE:
      pCharacteristic->setValue("stop");
      break;
    case LOWER_STATE:
      pCharacteristic->setValue("lower");
      break;
    case CLIMB_STATE:
      pCharacteristic->setValue("climb");
      break;
    default:
      pCharacteristic->setValue("unknown");
    pCharacteristic->notify();
  }
  // pCharacteristic->setValue()
  // digitalWrite(GPIO_NUM_25, HIGH); //I just want a random pin for power
  // checkFeedback();
  // Serial.println(digitalRead(GPIO_NUM_2));
}