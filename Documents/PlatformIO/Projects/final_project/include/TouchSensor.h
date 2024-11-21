#ifndef TOUCHSENSOR_H
#define TOUCHSENSOR_H
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>

void touchinit(Adafruit_CAP1188 &cap, TFT_eSPI &tft);
/*Initilizes Adafruit_CAP1188 touch sensor*/

void readTouch(Adafruit_CAP1188 &cap, TFT_eSPI &tft);



#endif