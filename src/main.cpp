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

//globals
float threshold = 0; 

//.h 
void belay();
void stop(int lower_flag);
void lower();
void checkFeedback();

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
    if(value == "belay") {
      belay();
    }
    if(value == "stop") {
      int lower_flag = 0;
      stop(lower_flag);
    }
    if(value == "lower") {
      int lower_flag = 1;
      stop(lower_flag);
    }
  }
};

/*
in the belay state, turn on the green LED
*/
void belay() {
  Serial.println("Entered the belaying state!");
  digitalWrite(GPIO_NUM_0, LOW); 
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(GPIO_NUM_4, HIGH); //figure out which pin we want later
  Serial.print("Current button value:");
  Serial.println(digitalRead(GPIO_NUM_23));
  //PWM CONTROL IS HERE (SET THE VOLTAGES AND PINS)

}

/*
in the stopped state, turn on the red LED
*/
void stop(int lower_flag) {
  Serial.println("Entered the stopped state!");
  digitalWrite(GPIO_NUM_0, HIGH); 
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(GPIO_NUM_4, LOW); //figure out which pin we want later
  
  //stop the motors

  if(lower_flag) {
    lower(); //we should only be entering the lower state after stopping
  }
}

/*
in the lower state, turn on the blue LED
*/
void lower() {
  Serial.println("Entered the lower state!");
  digitalWrite(BUILTIN_LED, HIGH); //blue
  digitalWrite(GPIO_NUM_0, LOW); //red
  digitalWrite(GPIO_NUM_4, LOW); //green
  int done = 0;
  while(!done) {
    Serial.println("Lowering!!");
    digitalWrite(BUILTIN_LED, HIGH);
    digitalWrite(GPIO_NUM_0, LOW); 
    digitalWrite(GPIO_NUM_4, LOW); //figure out which pin we want later
    done = digitalRead(GPIO_NUM_23); //button press
    Serial.println(done); 
    //send a signal to servo to turn lever 
    //maybe should light an LED to show it? 
  }
}

/**
 * @brief 
 * might be a good idea to drop this function inside the loop
 */
void checkFeedback() { 
  //check feedback voltage and either call 
  // float voltage = float(digitalRead(GPIO_NUM_)); //some value we read from the feedback pin
  float voltage = analogRead(LEFT_FB); //should enter teh stopped state 
  Serial.println(voltage);
  //currently threshold is just 0
  if(voltage > threshold) {
    stop(0); //just want to enter the regular stopped state
  } else {
    belay();
  }
}


// init
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_NUM_4, OUTPUT);
  pinMode(GPIO_NUM_0, OUTPUT);
  pinMode(GPIO_NUM_32, INPUT); //setting the pin modes here
  pinMode(GPIO_NUM_25, OUTPUT);

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
  digitalWrite(GPIO_NUM_25, HIGH); //I just want a random pin for power
  checkFeedback();
  // Serial.println(digitalRead(GPIO_NUM_2));
}