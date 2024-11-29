#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <SparkFunLSM6DSO.h>

const unsigned long calibrationTime = 10000; // 10s calibration
const int THRESHOLD_VALUE = map(1.5, -2, 2, 0, 255);   // threshold

// Initialize the accelerometer 
void initAccel(LSM6DSO &myIMU);

// Calibrate Operation
// calibrate 10s for accelerometer value mapping 
void calibrateAccel(LSM6DSO &myIMU, float &min_X, float &max_X);

// Read the data from the accelerometer and map it
float readAccel(LSM6DSO &myIMU, float &min_X, float &max_X);

// Determine whether the user sitting on the object
bool onPosition(float x_val);

#endif // ACCELEROMETER_H