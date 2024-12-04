#include "secrets.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"



WiFiClientSecure net = WiFiClientSecure();


void connectWIFI()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("Wifi Connected!")
}





void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime) {
  // Validate inputs
  if (heartRate <= 0 || oxygen <= 0 || oxygen > 100 || elapsedTime < 0) {
    Serial.println("Invalid sensor data. Request not sent.");
    return;
  }

  StaticJsonDocument<200> doc;

  doc["heartRate"] = heartRate;
  doc["oxygen"] = oxygen;
  doc["elapsedTime"] = elapsedTime;
  char jsonBuffer[512];
  serializeJson(doc,jsonBuffer);
  Serial.println("sending info: " + jsonBuffer)

  HTTPClient http;

  String url = String(endpoint) + "/" + bucket_name + "/" + object_key;
  net.setInsecure();
  http.begin(net,url);
  http.addHeader("Content-Type", "application/json");
  //generate signature
  http.addHeader("Authorization", signature);

  int httpResponseCode = http.PUT(jsonBuffer);  // Sending data as JSON

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.println("Successfully uploaded to S3!");
  } else {
    Serial.print("Error in uploading: ");
    Serial.println(httpResponseCode);
  }
http.end();
}