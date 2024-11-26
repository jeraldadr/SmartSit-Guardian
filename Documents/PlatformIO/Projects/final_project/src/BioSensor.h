#ifndef BIOSENSOR_H
#define BIOSENSOR_H

#include <SparkFun_Bio_Sensor_Hub_Library.h>

// Initialize the Biosensor
void initBioSensor(SparkFun_Bio_Sensor_Hub &bioHub);

// Read the data from the Biosensor
void readBioData(SparkFun_Bio_Sensor_Hub &bioHub);

#endif // BIOSENSOR_H