<<<<<<< HEAD
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>


// Part 2 (Hardware SPI)

// Define CAP1188 object for the touch sensor
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// Define TFT_eSPI object for the display purpose
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  Serial.println("CAP1188 test!");
  tft.begin();
  tft.setRotation(1);

  
  if (!cap.begin(0x29)) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
}

void loop() {
  u_int8_t count = 0;
  uint8_t touched = cap.touched();

  if (touched == 0) {
    count = 0;
  }
  
  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      count++;
    }
  }

  Serial.println();
  delay(50);
  tft.setTextSize(10);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0,15);
  tft.println(count);
=======
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>


// Part 2 (Hardware SPI)

// Define CAP1188 object for the touch sensor
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// Define TFT_eSPI object for the display purpose
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  Serial.println("CAP1188 test!");
  tft.begin();
  tft.setRotation(1);

  
  if (!cap.begin(0x29)) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
}

void loop() {
  u_int8_t count = 0;
  uint8_t touched = cap.touched();

  if (touched == 0) {
    count = 0;
  }
  
  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      count++;
    }
  }

  Serial.println();
  delay(50);
  tft.setTextSize(10);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0,15);
  tft.println(count);
>>>>>>> 3494f3cea314119076c8e2f8772cca4db8d2f247
}