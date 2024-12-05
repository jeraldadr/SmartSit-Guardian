#include "secrets.h"
#include "cloudConnect.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <time.h>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>  // For HMAC-SHA256
#include "esp_http_client.h"




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

  Serial.println("Wifi Connected!");

   // Configure time using NTP
  Serial.println("Synchronizing time...");
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

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
  String timestamp = getAWSTimestamp();
  String dateString = String(timestamp).substring(0, 8);
  String payload = "{\"heartRate\":" + String(heartRate) +
                   ",\"oxygen\":" + String(oxygen) +
                   ",\"sittingTime\":" + String(elapsedTime) + "}";

  // Hash the payload
  String payloadHash = hashPayload(payload);
  unsigned char signingkey[32];
  unsigned char signature[32];
  String canonicalRequest = createCanonical(payloadHash);
  String StringtoSign = createStringtoSign(canonicalRequest,timestamp,dateString);
  generateSigningKey(AWS_SECRET_KEY,dateString,region,"s3", signingkey);
  computeSignature(signingkey,StringtoSign,signature);
  String signedHeaders = "content-type;host;x-amz-date;x-amz-content-sha256";  // Include all signed headers
  String scope = dateString + "/us-west-1/s3/aws4_request";
  String authHeader = "Authorization: AWS4-HMAC-SHA256 Credential=" ;
  authHeader += AWS_ACCESS_KEY_ID;
  authHeader += "/" ;
  authHeader += scope ;
  authHeader += ",SignedHeaders=";
  authHeader += signedHeaders ;
  authHeader += ",Signature=";
  authHeader += byteToHex(signature, 32);


  esp_http_client_handle_t client;
  esp_http_client_config_t config = {
    .url = "https://s3.us-west-1.amazonaws.com/smartsit-bucket/sensor-info.txt",  // Combine the endpoint, bucket name, and object key
    .cert_pem = AWS_CERT_CA,
    .buffer_size = 8192,  // Increase buffer size for response body (default: 1024)
    .buffer_size_tx = 8192
  };

  client = esp_http_client_init(&config);

  // Set HTTP headers
  esp_http_client_set_header(client, "Content-Type", "application/json");  // Content-Type for JSON payload
  esp_http_client_set_header(client, "x-amz-date", timestamp.c_str());  // Replace with actual timestamp
  esp_http_client_set_header(client, "x-amz-content-sha256", payloadHash.c_str());  // Replace with payload hash if needed
  esp_http_client_set_header(client, "Authorization", authHeader.c_str());  // Set Authorization header
  
  // Set HTTP method to PUT
  esp_http_client_set_method(client, HTTP_METHOD_PUT);
  
  // Send the PUT request with the payload
  esp_http_client_set_post_field(client, payload.c_str(), strlen(payload.c_str()));  // Sending JSON payload as data

  // Perform the HTTP request
  esp_err_t err = esp_http_client_perform(client);

  if (err == ESP_OK) {
    Serial.printf("HTTP Status Code: %d\n", esp_http_client_get_status_code(client));
  } else {
    Serial.printf("HTTP request failed: %s\n", esp_err_to_name(err));
  }

  // Clean up
  esp_http_client_cleanup(client);
}


String createCanonical(String payloadHash){
  String date = getAWSTimestamp();
  String canonicalRequest = "PUT\n";
  canonicalRequest += "/smartsit-bucket/sensor-info.txt\n";
  canonicalRequest += "\n";
  canonicalRequest += "content-type:application/json\n";
  canonicalRequest += "host:s3.us-west-1.amazonaws.com\n";
  canonicalRequest += "x-amz-date:" + date + "\n";
  canonicalRequest += "x-amz-content-sha256:" + payloadHash + "\n";
  canonicalRequest += "content-type;host;x-amz-date;x-amz-content-sha256\n";
  canonicalRequest += payloadHash;
  // Assuming you have the list of headers
  return canonicalRequest;
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

String createStringtoSign(String canonicalRequest, String timestamp, String dateString) {
  String stringToSign = "AWS4-HMAC-SHA256\n";
  stringToSign += timestamp + "\n";
  stringToSign += dateString + "/us-west-1/s3/aws4_request\n";
  String canonicalRequestHash = getCanonicalRequestHash(canonicalRequest);
  stringToSign += canonicalRequestHash;
  return stringToSign;
}


String getCanonicalRequestHash(const String &canonicalRequest) {
  unsigned char hash[32]; // Buffer for the SHA-256 hash (32 bytes)
  // Compute the SHA-256 hash
  mbedtls_sha256_context sha256Ctx;
  mbedtls_sha256_init(&sha256Ctx);
  mbedtls_sha256_starts_ret(&sha256Ctx, 0); // 0 for SHA-256 (not SHA-224)
  mbedtls_sha256_update_ret(&sha256Ctx, (const unsigned char *)canonicalRequest.c_str(), canonicalRequest.length());
  mbedtls_sha256_finish_ret(&sha256Ctx, hash);
  mbedtls_sha256_free(&sha256Ctx);

  // Convert the hash to a hexadecimal string
  String hexHash = "";
  for (int i = 0; i < 32; i++) {
    if (hash[i] < 16) hexHash += "0"; // Add leading zero for single-digit bytes
    hexHash += String(hash[i], HEX);
  }

  return hexHash;
}



void hmacSha256(const unsigned char *key, size_t keyLen, 
                const unsigned char *data, size_t dataLen, 
                unsigned char *output) {
  mbedtls_md_context_t ctx;
  const mbedtls_md_info_t *info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, info, 1);
  mbedtls_md_hmac_starts(&ctx, key, keyLen);
  mbedtls_md_hmac_update(&ctx, data, dataLen);
  mbedtls_md_hmac_finish(&ctx, output);
  mbedtls_md_free(&ctx);
}

void generateSigningKey(const String &secretKey, const String &dateStamp, 
                        const String &region, const String &service, 
                        unsigned char *signingKey) {
  unsigned char kDate[32];
  unsigned char kRegion[32];
  unsigned char kService[32];
  
  // Step 1: kDate = HMAC("AWS4" + SecretAccessKey, DateStamp)
  String kSecret = "AWS4" + secretKey;
  hmacSha256((unsigned char *)kSecret.c_str(), kSecret.length(), 
             (unsigned char *)dateStamp.c_str(), dateStamp.length(), 
             kDate);
  
  // Step 2: kRegion = HMAC(kDate, Region)
  hmacSha256(kDate, 32, (unsigned char *)region.c_str(), region.length(), kRegion);
  
  // Step 3: kService = HMAC(kRegion, Service)
  hmacSha256(kRegion, 32, (unsigned char *)service.c_str(), service.length(), kService);
  
  // Step 4: kSigning = HMAC(kService, "aws4_request")
  String aws4Request = "aws4_request";
  hmacSha256(kService, 32, (unsigned char *)aws4Request.c_str(), aws4Request.length(), signingKey);
}

void computeSignature(const unsigned char *signingKey, const String &stringToSign, unsigned char *signature) {
  // Perform HMAC-SHA256 of the string to sign using the signing key
  hmacSha256(signingKey, 32, (unsigned char *)stringToSign.c_str(), stringToSign.length(), signature);
}

// Helper function to print the signature in hex format
String toHex(const unsigned char *data, size_t length) {
  String hexString = "";
  for (size_t i = 0; i < length; i++) {
    char hex[3];
    sprintf(hex, "%02x", data[i]);
    hexString += hex;
  }
  return hexString;
}

String byteToHex(unsigned char* byteArray, size_t length) {
  String hexString = "";
  for (size_t i = 0; i < length; i++) {
    hexString += String("0123456789ABCDEF"[byteArray[i] >> 4]);  // First nibble
    hexString += String("0123456789ABCDEF"[byteArray[i] & 0x0F]);  // Second nibble
  }
  return hexString;
}
