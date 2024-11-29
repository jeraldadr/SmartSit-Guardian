#include "Accelerometer.h"


void initAccel(LSM6DSO &myIMU) {
  //Assume serial is already initialized
  // LSM6DSO Sensor Set-Up
  delay(10);
  if( myIMU.begin() )
    Serial.println("Accelerometer Ready.");
  else { 
    Serial.println("Could not connect to accelerometer.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Accelerometer Loaded Settings.");
}

void calibrateAccel(LSM6DSO &myIMU, float &min_X, float &max_X) {
  unsigned long startTime = millis(); 
  unsigned long runTime = millis();
  
  min_X = 0.0; 
  max_X = 0.0; 

  Serial.println("Start calibration for 10s");
  while ((runTime - startTime) < calibrationTime) {
    float x = myIMU.readFloatAccelX();
    min_X = min(min_X, x);
    max_X = max(max_X, x);
    runTime = millis();
  }
  Serial.println("Done calibration");
}

float readAccel(LSM6DSO &myIMU, float &min_X, float &max_X) {
  float x_val = myIMU.readFloatAccelX();
  x_val = (x_val - min_X) * (255 - 0) / (max_X - min_X); 
  return x_val;
}

bool onPosition(float x_val) {
  if (x_val >= 300) 
    return false;
  return true;
}
  
