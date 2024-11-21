#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>


void touchinit(Adafruit_CAP1188 &cap, TFT_eSPI &tft) {
  Serial.begin(9600);
  Serial.println("CAP1188 test!");
  tft.begin();
  tft.setRotation(1);
  if (!cap.begin(0x29)) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
}

/*IMPLEMENT Interrupts and FreeRTOS*/
void readTouch(Adafruit_CAP1188 &cap, TFT_eSPI &tft) {
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
  Serial.println();
  delay(50);
  tft.setTextSize(10);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0,15);
  tft.println(count);
}