#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
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


// declare pins
const int resPin = 4;
const int mfioPin = 13;

// declare sensors
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); 
bioData body; 
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);

  Wire.begin();
  int result = bioHub.begin();
  if (result == 0) // Zero errors!
    Serial.println("Sensor started!");
  else
    Serial.println("Could not communicate with the sensor!");
 
  Serial.println("Configuring Sensor...."); 
  int error = bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings. 
  if(error == 0){ 
    Serial.println("Sensor configured.");
  }
  else {
    Serial.println("Error configuring sensor.");
    Serial.print("Error: "); 
    Serial.println(error); 
  }

  Serial.println("Loading up the buffer with data....");
  delay(4000); 
}

void loop() {
  body = bioHub.readBpm();
  Serial.print("Heartrate: ");
  Serial.println(body.heartRate); 
  Serial.print("Confidence: ");
  Serial.println(body.confidence); 
  Serial.print("Oxygen: ");
  Serial.println(body.oxygen); 
  Serial.print("Status: ");
  Serial.println(body.status); 
  // Slow it down or your heart rate will go up trying to keep up
  // with the flow of numbers
  delay(250); 
}

