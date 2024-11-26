#include "Accelerometer.h"


const unsigned long calibrationTime = 10000; // 10s calibration
const int THRESHOLD_VALUE = map(1.5, -2, 2, 0, 255);   // threshold


void initAccel(LSM6DSO &myIMU) {
  //Assume serial is already initialized
  // LSM6DSO Sensor Set-Up
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

// calibrate 10s for accelerometer value mapping 
void calibrateAccel(LSM6DSO &myIMU, float &min_X, float &max_X) {
  unsigned long startTime = millis(); 
  unsigned long runTime = millis();
  const unsigned long calibrationTime = 10000;
  min_X = 0.0; 
  max_X = 0.0; 

  // calibration phrase
  while ((runTime - startTime) < calibrationTime) {
    float x = myIMU.readFloatAccelX();
    min_X = min(min_X, x);
    max_X = max(max_X, x);
    runTime = millis();

    // might need to change here, since we need to take care more dimension (more accurate)

  }
}

// reading and mapping the value
float readAccel(LSM6DSO &myIMU, float &min_X, float &max_X) {
  float x_val = myIMU.readFloatAccelX();
  x_val = (x_val - min_X) * (255 - 0) / (max_X - min_X); 
  return x_val;
}
  
