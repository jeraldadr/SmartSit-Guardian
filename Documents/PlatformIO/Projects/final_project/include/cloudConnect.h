#ifndef CLOUDCONNECT_H
#define CLOUDCONNECT_H
#include <WiFiClientSecure.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <time.h>



//time
// NTP server details
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;       // Offset for UTC
const int daylightOffset_sec = 0;  // No daylight saving time for UTC


/*Function to send sensor data to the server
This function sends the heart rate, oxygen saturation, and sitting time data to a server via HTTP.
It constructs the URL path with the sensor data, makes a GET request, and handles the server's response.*/
void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime);

void connectWIFI();
String getAWSTimestamp();
String hmacSHA256(const String &key, const String &data);

String hashPayload(const String& payload);

#endif // CLOUDCONNECT_H