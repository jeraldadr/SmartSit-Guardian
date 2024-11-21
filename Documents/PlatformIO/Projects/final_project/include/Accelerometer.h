#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Adafruit_LSM6DSO.h>


// Declarations
void initAccel(LSM6DSO &myIMU); 
/*Initializes the LSM6DSO in order to use the accelerometer*/

void calibrateAccel(float &min_X, float &max_X);
/*10s calibration in order to determine the minimum x and 
maximum x values to be used for the threshold*/

float readAccel();
/*Reads accelerometer value from LSM6DSO and maps it using a custom mapping equation*/



#endif

