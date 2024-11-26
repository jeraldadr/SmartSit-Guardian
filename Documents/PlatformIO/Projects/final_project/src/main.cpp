#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
a#include "TouchSensor.h"
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


// declare pins
const int resPin = 4;
const int mfioPin = 13;

// declare sensors
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); // Biosensor
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET); // touch sensor
LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B, Accelerometer
TFT_eSPI tft = TFT_eSPI(); // screen 


void setup() {
  Serial.begin(9600);

  Wire.begin();
  
}

void loop() {

}

