/*
neopixel will animate depending on temp
oled screen will display temp 
if temp sensor detect high temp
it will communicate with web server,
change neopixel, and show warning on oled

*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif
#include <OneWire.h>
#include <DallasTemperature.h>

// TS led
#define led 2
#define TOGGLE(x) digitalWrite(x, digitalRead(x) ? LOW : HIGH)

// OLED fields
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 oled ( 
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1);
//oled image
static const uint8_t image_data_isshin1[1040] = {
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙█████████████∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███∙∙∙∙∙∙∙∙∙∙∙██████████████∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████∙∙∙∙∙∙∙∙∙███████████████∙∙∙∙∙∙∙∙∙∙███∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████∙∙∙∙∙∙∙∙████████████████∙∙∙∙∙∙∙∙███∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙∙∙∙∙∙█████████████████∙∙∙∙∙∙█████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙∙∙∙∙█∙███████████████∙∙∙∙∙██████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███████∙∙∙█████████████████∙∙∙∙██████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██████∙∙∙█████████████████∙∙∙███████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███████∙∙█████████████████∙∙███████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙███████∙█████████████████∙███████∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███∙██████∙█████████████████∙██████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████∙█████∙█████████████████∙█████∙██∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙████∙█████████████████∙████∙∙█∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙∙█∙∙████████∙█████████∙█∙∙█████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██████∙∙∙███████████████████∙∙∙██████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████████████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙█████████████████████████████████∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙███████████████████████████████████████∙█∙██████████████████████████████████∙∙∙█████████████████████████████████████████████∙∙
    // ∙∙███████████████████████████████████████∙█████████████████∙█∙█████████████████∙██████████████████████████████████████████████∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██████∙█████████∙∙██∙████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████████████∙∙∙∙███∙∙∙∙████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███████∙█████∙∙∙███∙∙∙█████∙███████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████████∙∙∙∙∙█∙█████∙█∙∙∙∙∙∙███████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙██████████∙∙█████∙∙██████████∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙██████████∙∙∙█████∙∙∙██████████∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙████████∙███████∙████████∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙█∙∙∙∙∙∙∙∙∙∙███████████████████████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙█∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙█∙∙∙∙∙∙∙∙∙∙∙████∙███∙∙████████∙█∙∙∙∙██∙████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙█∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙█∙█∙∙∙∙∙∙∙∙∙∙∙█∙∙█∙∙∙█∙∙∙∙∙∙∙∙∙∙∙█████∙██∙█████████∙████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙█∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙█∙█∙∙∙∙█∙∙∙∙∙∙█∙∙█∙∙█∙∙∙∙∙∙█∙∙∙∙∙∙███████∙██████████████∙∙∙█∙███∙∙█∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙█∙█∙∙∙∙█∙∙∙∙∙∙█∙∙███∙∙∙∙∙∙∙∙∙██∙∙∙██████∙██████∙███████∙∙███∙██∙∙█∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙█∙█∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙████████∙∙∙∙∙∙∙∙∙∙∙∙∙███████████∙███████∙█∙███████∙∙█████∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙█∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███∙███∙████∙█████████∙█∙█∙∙∙█∙∙∙∙∙∙∙∙∙∙███∙∙██∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███████████∙████████∙███∙█∙∙█∙∙∙∙█∙∙∙∙∙∙∙∙█∙█∙∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙███∙∙∙∙∙∙∙██∙∙∙∙∙∙∙█████∙█∙∙∙∙∙████████████████████∙∙∙██∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙████∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙███████∙∙█∙███████∙█████∙∙∙∙∙███∙∙∙∙∙∙∙∙█∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙█∙█∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙██████∙█∙█████∙█∙∙██∙█∙∙∙∙∙∙██∙∙∙∙∙∙█∙█∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙∙███∙█∙██∙∙∙∙∙∙∙█████∙█∙∙∙∙∙█∙∙∙∙∙∙∙█∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙█∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙█∙∙█∙∙█∙█∙∙∙∙∙∙∙∙█████∙∙∙∙∙██∙∙∙█∙∙∙∙∙∙█∙∙∙∙∙█∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙█∙██∙∙∙∙∙∙█∙∙∙∙∙∙∙∙██∙∙█∙∙∙∙█∙∙∙∙∙∙██∙██████∙██████∙██∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙█∙∙∙∙∙∙█∙∙∙∙∙∙∙█∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙∙██∙∙███████████∙█∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██████████████∙██∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙██████∙██∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██████████∙████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███████████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙█∙█████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙██∙∙
    // ∙∙████████████████████████████████████████████████████∙██∙█∙∙∙███∙█∙██████████████████████████████████████████████████████████∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙██∙██∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙████∙█∙████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙███∙█∙∙████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█████∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0xff, 0xe0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0xff, 0xf0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0xff, 0xf0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x07, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x0f, 0xff, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x0b, 0xff, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x8f, 0xff, 0xf8, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x8f, 0xff, 0xf8, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xcf, 0xff, 0xf9, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xef, 0xff, 0xfb, 0xf8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0xef, 0xff, 0xfb, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xef, 0xff, 0xfb, 0xed, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x7d, 0xef, 0xff, 0xfb, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x4f, 0xf7, 0xfd, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x1f, 0xff, 0xfc, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xff, 0xff, 0xff, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xaf, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xeb, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xcd, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x1c, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0x1c, 0x7d, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0xbe, 0x81, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x1b, 0xff, 0x3e, 0x7f, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x27, 0xfe, 0x3e, 0x3f, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x06, 0xff, 0x7f, 0x7f, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x02, 0x20, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x09, 0x00, 0x04, 0x40, 0x07, 0xb9, 0xfe, 0x86, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x14, 0x04, 
    0x30, 0x0a, 0x00, 0x24, 0x40, 0x07, 0xdb, 0xfe, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x24, 0x04, 
    0x30, 0x0a, 0x10, 0x24, 0x81, 0x03, 0xfb, 0xff, 0xf1, 0x72, 0x00, 0x20, 0x03, 0x00, 0x20, 0x04, 
    0x30, 0x0a, 0x10, 0x27, 0x00, 0x63, 0xf7, 0xef, 0xe7, 0x64, 0x00, 0x20, 0x00, 0x0c, 0x28, 0x04, 
    0x30, 0x00, 0x08, 0x7f, 0x80, 0x03, 0xff, 0xbf, 0xaf, 0xe7, 0xd0, 0x00, 0x00, 0x10, 0x00, 0x04, 
    0x30, 0x00, 0x08, 0x88, 0x00, 0x01, 0xdd, 0xef, 0xfa, 0x88, 0x01, 0xcc, 0x00, 0x20, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x08, 0x00, 0x01, 0xff, 0xdf, 0xee, 0x90, 0x80, 0x50, 0x00, 0xc0, 0x00, 0x04, 
    0x30, 0x00, 0x70, 0x18, 0x0f, 0xa0, 0xff, 0xff, 0xf1, 0x80, 0x80, 0x60, 0x00, 0x20, 0x00, 0x04, 
    0x30, 0x00, 0x08, 0x78, 0x00, 0x40, 0xfe, 0x5f, 0xdf, 0x07, 0x00, 0x90, 0x00, 0x10, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x94, 0x00, 0x80, 0x3f, 0x5f, 0x4d, 0x03, 0x02, 0x90, 0x00, 0x30, 0x00, 0x04, 
    0x30, 0x00, 0x13, 0x00, 0x03, 0x00, 0x3a, 0xc0, 0x7d, 0x04, 0x04, 0x08, 0x00, 0x98, 0x00, 0x04, 
    0x30, 0x00, 0x60, 0x20, 0x06, 0x00, 0x92, 0x80, 0x7c, 0x18, 0x81, 0x05, 0x80, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0xb0, 0x20, 0x19, 0x08, 0x1b, 0xf7, 0xec, 0x00, 0x61, 0x02, 0x02, 0x20, 0x00, 0x04, 
    0x30, 0x01, 0x08, 0x00, 0x00, 0x60, 0x0c, 0xff, 0xea, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x04, 
    0x30, 0x02, 0x04, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x02, 0x00, 0x00, 0x00, 0x05, 0xfb, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0xfe, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcb, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xa3, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf6, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xd7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xa7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 15


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Variables to store temperature values
String temperatureF = "";
String temperatureC = "";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;

// Replace with your network credentials
const char* ssid = "CenturyLink6934";
const char* password = "2eb6vd3dn7hu2d";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP DS18B20 Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Celsius</span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Fahrenheit</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;F</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";


String readDSTemperatureC();
String readDSTemperatureF();
String processor(const String& var);
void showTxt(String msg);
void clear();
void isshin1();
void failBlink();



void setup(){
  //led init
  pinMode(led, OUTPUT);
  failBlink();
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Start up the DS18B20 library
  sensors.begin();

  // initiate oled 
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)){  // if oled doesnt initialize (0x3c is serial address?)
    Serial.println(F("SSD1306 allocation failure"));
    for(;;)failBlink();;
  }

  delay(2000);

  clear();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);

  isshin1();
  clear();


  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  oled.write("Connecting to WiFi\n");
  oled.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    oled.write(".");
    oled.display();
  }
  clear();
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  oled.write(WiFi.localIP());
  oled.display();
  delay(3000);
  clear();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureF.c_str());
  });
  // Start server
  server.begin();
  oled.setTextSize(2);
}
 
void loop(){
  if ((millis() - lastTime) > timerDelay) {
    //clear();
    temperatureC = readDSTemperatureC();
    //oled.write(temperatureC);
    //oled.display;
    temperatureF = readDSTemperatureF();
    //delay(2000);
    //clear();
    lastTime = millis();
  }  
}




void showTxt(String msg){
  oled.println(msg);
  oled.display();
}

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  else if(var == "TEMPERATUREF"){
    return temperatureF;
  }
  return String();
}

String readDSTemperatureC() {
  // clear oled
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  //char sTemp = char(tempC);
  int sTemp = char(tempC);
  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    //oled.write("Failed to read sensor");
    //delay(2000);
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
    //oled.printf("temp C: %s",tempC);
    
    //oled.write(sTemp);
    //oled.display();
  }
  return String(tempC);
}

String readDSTemperatureF() {
  clear();
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempF = sensors.getTempFByIndex(0);

  if(int(tempF) == -196){
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  }else if(70 <= tempF && tempF <=80){
      Serial.print("Temperature Fahrenheit: ");
      Serial.println(tempF);
      oled.write("Operating\ntemp\noptimal");
      oled.display();
  }else if(tempF < 70){
      Serial.print("Temperature Fahrenheit: ");
      Serial.println(tempF);
      oled.write("Operating\nbelow\ntemp");
      oled.display();
  }else if(tempF > 80){
      Serial.print("Temperature Fahrenheit: ");
      Serial.println(tempF);
      oled.write("Operating\ntemp\ncritical!");
      oled.display();
  }
  delay(2000);
  return String(tempF);
}

void failBlink(){
  
  for(byte i = 0; i<=4;i++){
    TOGGLE(led);
    delay(500);

  }
}

void clear(){
  oled.clearDisplay();            
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(0,0);
  oled.display();
}

// logo
void isshin1(){
  digitalWrite(led, HIGH);
  oled.drawBitmap(0, 0, image_data_isshin1, 128, 64, 1);
  oled.display();
  delay(5000);
  digitalWrite(led, LOW);
  clear();
}