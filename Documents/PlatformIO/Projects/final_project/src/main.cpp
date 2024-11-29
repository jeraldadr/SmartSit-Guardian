#include <Arduino.h>
#include <stdio.h>

// // Sensors
#include "TouchSensor.h"
#include "BioSensor.h"
#include "Accelerometer.h"

// Internet
#include <HttpClient.h>
#include <WiFi.h>
#include <inttypes.h>

// MCU 
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"


// Reset pin, MFIO pin, Buzzer pin
const int resPin = 37;
const int mfioPin = 38;
const int buzzer = 25;

// Sensors
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); // Biosensor
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET); // Touch Sensor
LSM6DSO myIMU; // Accelerometer
TFT_eSPI tft = TFT_eSPI(); // screen 

// variable for logic control and calibration
bool touched = false;
bool calibrated = false;
unsigned long sittingTime = 0;
float min_X = 0;
float max_X = 0;

void setup(){
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Wire.begin();

  initBioSensor(bioHub);
  initTouchSensor(cap, tft); 
  initAccel(myIMU);

  sittingTime = millis(); // initialize the timestamp 
}

void loop(){ 
  // Once touched, start recording user data
  if (!touched && readTouch(cap, tft)) {
    tone(buzzer, 1000, 300);
    noTone(buzzer);
    calibrateAccel(myIMU, min_X, max_X); 
    sittingTime = millis(); // reset the timestamp
    touched = true;
  }

  delay(100); 

  if (touched) {
    // bioData body = readBioData(bioHub); // biosensor needs 5-10s to start updating 
    // if (receiveValidData(body)) // once the data is validated, send it to the "database"

    float sittingData = readAccel(myIMU, min_X, max_X); 
    unsigned long elapsedTime = (millis() - sittingTime) / 1000;

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0,15);
    tft.fillRect(0, 15, 120, 20, TFT_BLACK);
    tft.print("time: ");
    tft.setCursor(0,65);
    tft.print(elapsedTime);
    tft.println("s");
    if (!onPosition(sittingData)) {
      touched = false;
    }
  }
  
}