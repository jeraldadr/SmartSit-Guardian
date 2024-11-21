#include <Arduino.h>
#include <string>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM6DSO.h>


void initAccel(LSM6DSO &myIMU) {
  //Assume serial is already initialized
  // LSM6DSO Sensor Set-Up

  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");
}

void calibrateAccel(float &min_X, float &max_X) {
  unsigned long startTime = millis(); 
  unsigned long runTime = millis();
  const unsigned long calibrationTime = 10000;
  min_X = 0.0; 
  max_X = 0.0; 
  pinMode(LED, OUTPUT); 
  // calibration phrase
  while ((runTime - startTime) < calibrationTime) {
      float x = myIMU.readFloatAccelX();
      min_X = min(min_X, x);
      max_X = max(max_X, x);
      runTime = millis();
  }

}
float readAccel() {
  x_val = myIMU.readFloatAccelX();
  x_val = (x_val - min_X) * (255 - 0) / (max_X - min_X); 
   return x_val
  }
  
