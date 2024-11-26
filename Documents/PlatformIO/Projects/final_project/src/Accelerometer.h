#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <SparkFunLSM6DSO.h>
#include <SPI.h>

void initAccel(LSM6DSO &myIMU);
void calibrateAccel(LSM6DSO &myIMU, float &min_X, float &max_X);
float readAccel(LSM6DSO &myIMU, float &min_X, float &max_X);

#endif // ACCELEROMETER_H