#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "1dd5ca59-fdf9-4c64-9102-ac29090eb19e"
#define CHARACTERISTIC_UUID "62516736-8bc6-45ad-9b0f-9a73e0b2d155"
#define DEVICE_NAME "ESP32test"

#define LEFT_IN1 GPIO_NUM_16
#define LEFT_IN2 GPIO_NUM_17
#define RIGHT_IN1 GPIO_NUM_18
#define RIGHT_IN2 GPIO_NUM_19
#define LEFT_D1 GPIO_NUM_21
#define RIGHT_D1 GPIO_NUM_22

#define LEFT_SF GPIO_NUM_34
#define RIGHT_SF GPIO_NUM_35
#define LEFT_FB GPIO_NUM_36
#define RIGHT_FB GPIO_NUM_39

#define MISC0 GPIO_NUM_23
#define MISC1 GPIO_NUM_25
#define MISC2 GPIO_NUM_26
#define MISC3 GPIO_NUM_27
#define MISC4 GPIO_NUM_32
#define MISC5 GPIO_NUM_33


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth not enabled: Run `make menuconfig`
#endif

// BLE instance
BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

class BelayCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
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
    if(value == "on1") {
      digitalWrite(BUILTIN_LED, HIGH);
    }
    if(value == "off1") {
      digitalWrite(BUILTIN_LED, LOW);
    }
    if(value == "on2") {
      digitalWrite(GPIO_NUM_4, HIGH);
    }
    if(value == "off2") {
      digitalWrite(GPIO_NUM_4, LOW);
    }
  }
};

// init
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_NUM_4, OUTPUT);
  Serial.begin(115200);
  Serial.println("Device started, starting server...");

  BLEDevice::init(DEVICE_NAME);
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new BelayCharacteristicCallbacks());
  pCharacteristic->setValue("Hello, world");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  delay(2000);
}