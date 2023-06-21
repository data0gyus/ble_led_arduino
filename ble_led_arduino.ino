#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

int LedPin = 25;
int buttonPin = 27;
bool bFlag = false;
BLECharacteristic *pCharacteristic;
BLEServer *pServer;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(LedPin,OUTPUT);
  pinMode(buttonPin,INPUT);

  BLEDevice::init("LED BLE MESH SERVER");
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setValue("0");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it on your phone!");
}

void loop() {
  std::string dataFromFlutter = pCharacteristic -> getValue();
  Serial.println(String(dataFromFlutter.c_str()));
  int buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if(digitalRead(buttonPin)==HIGH){
    bFlag = true;
  }
  else {
    if (dataFromFlutter == "0") {
      digitalWrite(LedPin, LOW); // LED를 끄는 코드
    } 
    else if (dataFromFlutter == "1") {
      digitalWrite(LedPin, HIGH); // LED를 켜는 코드

    pCharacteristic -> setValue(dataFromFlutter);
    pCharacteristic -> notify();
    }
  delay(1000);
}
}