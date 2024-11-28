#include "TouchSensor.h"


void initTouchSensor(Adafruit_CAP1188 &cap, TFT_eSPI &tft) {
  Serial.println("CAP1188 test!");
  tft.begin();
  tft.setRotation(1);
  tft.setTextSize(5);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if (!cap.begin(0x29)) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
}

/*IMPLEMENT Interrupts and FreeRTOS*/
bool readTouch(Adafruit_CAP1188 &cap, TFT_eSPI &tft) {
  u_int8_t count = 0;
  uint8_t touched = cap.touched();
  if (touched == 0) {
    count = 0;
  }
  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      count++;
    }
  }
  delay(50);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,15);
  if (count > 0) 
  {
    tft.println("Started");
    return true;
  }
  tft.setCursor(0, 15);
  tft.println("Not");
  tft.setCursor(0, 60);
  tft.println("Touched");
  return false;
}