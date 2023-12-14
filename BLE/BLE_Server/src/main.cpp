#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// macros
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "0ce06608-9aa6-11ee-b9d1-0242ac120002"
#define CHARACTERISTIC_UUID "26fbb5a6-9aa6-11ee-b9d1-0242ac120002"


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE server");
  delay(200);

  //invoke factory method init() on BLEDevice class
  BLEDevice::init("Xiao");
  //create pointer object of type BLEServer 
  // assign it the return of factory method createServer(), from BLEDevice class
  // recall that pointer allows us to control dynamic memory beyond its scope
  // otherwise, object pServer would be destroyed after setup()
  // also recall scope reolution operator '::'
  // is used when accessing (global) class method (static)
  BLEServer *pServer = BLEDevice::createServer();
  // create pointer object of type BLEServer
  // access pServer instance method to create a BLEService
  // assign to pointer
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // create characteristic through pService instance method
  // assign to pointer (notice args bit-wise OR)
  // setting up both read and write chatacteristic with UUID
  BLECharacteristic *pChar = pService->createCharacteristic(
                                      CHARACTERISTIC_UUID,
                                      BLECharacteristic::PROPERTY_READ |
                                      BLECharacteristic::PROPERTY_WRITE);
  

  // set characteristic method through instance method 
  pChar -> setValue("Hello world :3");
  // start the service
  pService->start();

  // advertising method is now done through BLEDevice class 
  // instead of using pServer instace method
  // create advertising object
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();

  // give the service its UUID
  pAdvertising->addServiceUUID(SERVICE_UUID);
  // what method do?
  pAdvertising->setScanResponse(true);
  // this helps with iphone connection issue?
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  // advertising is done through static method of different class?
  BLEDevice::startAdvertising();;

  Serial.println("Characteristics defined!  This is now visible to your phone");
}

void loop() {

  delay(2000);
}

