#include <Arduino.h>

#include "BluetoothSerial.h"

// used if pin will be req'd
//#define USE_PIN 
// pin that will be used
const char *pin = "1234"; 

// name passed as arg to bt obj
String device_name = "lil'_Slave";

// macro is checking if bluetooth is properly enabled 
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif  

// checking if board is compatible[?]
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

// bt obj
BluetoothSerial SerialBT;

void setup() {

  Serial.begin(115200);
  

  //init bt serial comms with device name
  SerialBT.begin(device_name); 

  // oh cool, i didn't know you can format strings in duino
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}