#include "secrets.h"
#include "cloudConnect.h"
#include <WiFiClientSecure.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <time.h>
#include "mbedtls/sha256.h"
#include <mbedtls/md.h>  // For HMAC-SHA256




void connectWIFI()
{
  WiFiClientSecure net = WiFiClientSecure();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.print("Wifi Connected!");

   // Configure time using NTP
  Serial.println("Synchronizing time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Wait until the time is set
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for time sync...");
    delay(1000);
  }

  // Print the current time
  Serial.println("Time synchronized!");
}

String getAWSTimestamp() {
  char timeStampBuffer[20];
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    return "Time sync failed";
  }

  // Format the time as YYYYMMDDTHHMMSSZ
  strftime(timeStampBuffer, sizeof(timeStampBuffer), "%Y%m%dT%H%M%SZ", &timeinfo);
  return String(timeStampBuffer);
}



void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime) {
  // Validate inputs
  if (heartRate <= 0 || oxygen <= 0 || oxygen > 100 || elapsedTime < 0) {
    Serial.println("Invalid sensor data. Request not sent.");
    return;
  }

  StaticJsonDocument<200> doc;
  String payload = "{\"heartRate\":" + String(heartRate) +
                   ",\"oxygen\":" + String(oxygen) +
                   ",\"sittingTime\":" + String(elapsedTime) + "}";

  // Hash the payload
  String payloadHash = hashPayload(payload);
  
  

  HttpClient http;
  String url = String(endpoint) + "/" + bucket_name + "/" + object_key;
  WiFiClientSecure net;
  net.setCACert(AWS_CERT_CA); // Set CA certificate if needed
  

  // Send the request
  int httpResponseCode = http.PUT(payload);  // Sending data as JSON

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


String createCanonical(String payloadHash){
  String date = getAWSTimestamp();
  String canonicalRequest = "PUT\n";
  canonicalRequest += "/smartsit-bucket/sensor-info.txt\n";
  canonicalRequest += "" + "\n";
  canonicalRequest += "content-type:application/json\n" + "host:s3.amazonaws.com\n";
  canonicalRequest += "x-amz-date:" + date + "\n";
  canonicalRequest += "x-amz-content-sha256:" + payloadHash + "\n";
  canonicalRequest += "host;x-amz-content-sha256;x-amz-date\n";
  canonicalRequest += payloadHash;
  // Assuming you have the list of headers
  

  return canonicalRequest
}

// Function to hash the payload
String hashPayload(const String& payload) {
  // Buffer to hold the binary hash result
  unsigned char hash[32]; // SHA-256 produces a 32-byte hash

  // Compute the SHA-256 hash
  mbedtls_sha256_context ctx;
  mbedtls_sha256_init(&ctx);
  mbedtls_sha256_starts_ret(&ctx, 0); // 0 = SHA-256, not SHA-224
  mbedtls_sha256_update_ret(&ctx, (const unsigned char*)payload.c_str(), payload.length());
  mbedtls_sha256_finish_ret(&ctx, hash);
  mbedtls_sha256_free(&ctx);

  // Convert the hash to a hexadecimal string
  char hashHex[65]; // 64 hex characters + 1 for null terminator
  for (int i = 0; i < 32; i++) {
    sprintf(&hashHex[i * 2], "%02x", hash[i]);
  }

  return String(hashHex);
}

String createStringtoSign(string canonicalRequest) {
  String stringToSign = "AWS4-HMAC-SHA256\n";
  String timestamp = getAWSTimestamp();
  String dateString = String(timestamp).substring(0, 8);
  stringToSign += timestamp + "\n";
  stringToSign += dateString + "/us-west-1/s3/aws4_request\n";
  String canonicalRequestHash = getCanonicalRequestHash(canonicalRequest);
  stringToSign += canonicalRequestHash;
}

String getCanonicalRequestHash(String canonicalRequest) {
  // Create SHA-256 object
  SHA256 sha256;
  uint8_t hash[SHA256_SIZE];
  sha256.update((const uint8_t*)canonicalRequest.c_str(), canonicalRequest.length());
  sha256.finalize(hash);

  // Convert hash to hex
  String hexHash = "";
  for (int i = 0; i < SHA256_SIZE; i++) {
    hexHash += String(hash[i], HEX);
  }
  return hexHash;
}