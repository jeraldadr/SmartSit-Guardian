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


// Reset pin, MFIO pin 
const int resPin = 37;
const int mfioPin = 38;
const int buzzer = 25;

// declare sensors
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); // Biosensor
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET); // Touch Sensor

LSM6DSO myIMU; // Accelerometer

TFT_eSPI tft = TFT_eSPI(); // screen 

bool touched = false;

void setup(){
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Wire.begin();

  initBioSensor(bioHub);
  initTouchSensor(cap, tft); 
  initAccel(myIMU);
  
}

void loop(){ 
  // Once touched, start recording user data
  if (!touched && readTouch(cap, tft)) {
    tone(buzzer, 1000, 300);
    noTone(buzzer);
    touched = true;
  }
  if (touched) {
    // Note: biosensor needs 5-10s to start updating 
    readBioData(bioHub);
  }
  delay(100); 
}