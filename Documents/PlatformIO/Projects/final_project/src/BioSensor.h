#ifndef BIOSENSOR_H
#define BIOSENSOR_H

#include <SparkFun_Bio_Sensor_Hub_Library.h>

void initBioSensor(SparkFun_Bio_Sensor_Hub &bioHub);
void readBioData(SparkFun_Bio_Sensor_Hub &bioHub);

#endif // BIOSENSOR_H