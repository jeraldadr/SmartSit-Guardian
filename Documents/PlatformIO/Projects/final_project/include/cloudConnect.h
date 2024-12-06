#ifndef CLOUDCONNECT_H
#define CLOUDCONNECT_H
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <time.h>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>  // For HMAC-SHA256
#include "esp_http_client.h"  // For HMAC-SHA256



//time
// NTP server details
const long gmtOffset_sec = 0;       // Offset for UTC
const int daylightOffset_sec = 0;  // No daylight saving time for UTC

/*Function to send sensor data to the server
This function sends the heart rate, oxygen saturation, and sitting time data to a server via HTTP.
It constructs the URL path with the sensor data, makes a GET request, and handles the server's response.*/
void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime);

void connectWIFI();
String getAWSTimestamp();

String hashPayload(const String& payload);
/*
void hmacSha256(const unsigned char *key, size_t keyLen, 
                const unsigned char *data, size_t dataLen, 
                unsigned char *output);
void generateSigningKey(const String &secretKey, const String &dateStamp, 
                        const String &region, const String &service, 
                        unsigned char *signingKey);
String createCanonical(String payloadHash);
String toHex(const unsigned char *data, size_t length);
void computeSignature(const unsigned char *signingKey, const String &stringToSign, unsigned char *signature);
String getCanonicalRequestHash(const String &canonicalRequest);
String createStringtoSign(String canonicalRequest, String timestamp, String dateString);
String byteToHex(unsigned char* byteArray, size_t length);
*/
#endif // CLOUDCONNECT_H