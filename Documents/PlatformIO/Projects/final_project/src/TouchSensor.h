#ifndef TOUCHSENSOR_H
#define TOUCHSENSOR_H

#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Define necessary pins for touch sensor
#define CAP1188_CS  10
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13
#define CAP1188_RESET  9

// Initialize the touch sensor
void initTouchSensor(Adafruit_CAP1188 &cap, TFT_eSPI &tft);

// Detect whether the user touch the sensor
void readTouch(Adafruit_CAP1188 &cap, TFT_eSPI &tft);

#endif // TOUCHSENSOR_H