#ifndef BIOSENSOR_H
#define BIOSENSOR_H

#include <SparkFun_Bio_Sensor_Hub_Library.h>

// Initialize the Biosensor
void initBioSensor(SparkFun_Bio_Sensor_Hub &bioHub);

// Read the data from the Biosensor
// The data has to be recorded only when the status is 3
bioData readBioData(SparkFun_Bio_Sensor_Hub &bioHub);

// Validate the data
bool receiveValidData(bioData &body);

#endif // BIOSENSOR_H