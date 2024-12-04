#ifndef CLOUDCONNECT_H
#define CLOUDCONNECT_H

#include "secrets.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"


// EC2 Server information
const char* WIFI_SSID = "Jerald";
const char* WIFI_PASSWORD = 12345678;

// AWS Bucket INFO
const char* bucket_name = "smartsit-bucket";
const char* object_key = "sensor-info.txt";  // The file name in the S3 bucket
const char* region = "us-west-1";  
const char* endpoint = "https://s3.us-west-1.amazonaws.com";  
// AWS KEYS
static const char AWS_CERT_CA[] = 
"-----BEGIN CERTIFICATE-----\n"
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5\n"
"-----END CERTIFICATE-----\n";

static const char AWS_CERT_CRT =
"-----BEGIN CERTIFICATE-----\n"
"MIIDWTCCAkGgAwIBAgIUF7C8WL4+K5vlAVPNEfvg+RVhrVowDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MTIwMzIwMDE1
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL36ZEDiG83/RgNE+oRq
Aax7yHzjLKb9hHgHk3bi3k3g3BtsdOCV9pWDHvBWIBhQEMa//Sqg70czMJmUxEXn
RivzK9fzGvRI/lxf5adweJ1Gg79Md77lNMRWDGhryElgB5FkEbKBYBkNPl1pQq1E
/L1bF8zJRsUL3R4zO/WcFNML4r60R4vmJGzHM7FAldsuNrzwvc37TDFutMXvzp9v
j7R6XUU9wYxRnxfBMbrbHkOC84Ya/NVSYIUpynpp5/kdBF/Ip6wRzqE+qRV+3ivQ
/kSSYLbOmGFUxOcd+SVFyIOUUrEQFwNTL9t2KRWWPA0Nrmj3S0hi2UTUAv2pJVax
LWMCAwEAAaNgMF4wHwYDVR0jBBgwFoAUyYVRcPdsIWQ6iqxeuDQgzZgXqVowHQYD
VR0OBBYEFPFKhtSW/zGV1f3LZymUwQ/UegOfMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBVEq437/xCQbr8qlT9l2rUnXkK
CqsaB0V66Fv19CTxkdf9PVZbMp5XuioL3V+FiOAo4haaQHso9QrO+IqsF0nU7onL
ekEemd67WmxCuL7p/rJM+AKiaQYJf6Ig16TBO3LJJchZcjiwx5gdubSMrvzewzCE
AhwxJUPVx3bxukyWj6lkw1gnIrqYIXbzGWr2eb7iH/iKsiJZxxXcECRgN7EloBp1
56jGGP0+p5RZ4X4+eSDRi2orOZWYfsh7V+cYEAxHWliYQkMSqXGtGOzbJQV5fuj9
bquBf5s39aZEZCCaV4pZc7Ni6x0IplIWEEik9tMNh8F/i5xWeVDMNLALX3bg\n"
"-----END CERTIFICATE-----\n";

static const char AWS_CERT_PRIVATE = 
"-----BEGIN RSA PRIVATE KEY----\n"
"MIIEpAIBAAKCAQEAvfpkQOIbzf9GA0T6hGoBrHvIfOMspv2EeAeTduLeTeDcG2x0
4JX2lYMe8FYgGFAQxr/9KqDvRzMwmZTERedGK/Mr1/Ma9Ej+XF/lp3B4nUaDv0x3
vuU0xFYMaGvISWAHkWQRsoFgGQ0+XWlCrUT8vVsXzMlGxQvdHjM79ZwU0wvivrRH
i+YkbMczsUCV2y42vPC9zftMMW60xe/On2+PtHpdRT3BjFGfF8ExutseQ4Lzhhr8
1VJghSnKemnn+R0EX8inrBHOoT6pFX7eK9D+RJJgts6YYVTE5x35JUXIg5RSsRAX
A1Mv23YpFZY8DQ2uaPdLSGLZRNQC/aklVrEtYwIDAQABAoIBADo726lL5gwCGPr9
lPEr8XFmzDEEDfbuNUH5yIkjx9MFf1Lkrvr83QY9mClBrTrmqA3UYg1EIbSoi1g4
kuV+nDLyqljay0NgcJ/uQV5PCpnjOJVWIkqgxcj9O3Zyu4VKa2d0zjvyjnt/ZMOL
dYXZZMPCfYwnkorqNRCB0fpaqsN6FIZr0ij5T0u6eNon+kUNQ6KayD1sQw72sJM5
7pp5JbHqYQZ81xnxwvsoS8uC3QKPZlMKwgHL6G4TDPnuTsgvhGm1Uqg3HEeIv9KS
+DNHNk+0/rbxa31uNxbHDMLkhZYrFseFNb6TgSpkiJLRMMAa71Z0rbxiZ4ydF2W+
pPt7v6kCgYEA8FsV6kEHFdduYVuqak+Z/HGjix9LztxUTG65weNRXaB224XOB3vQ
662sTd4QaEoaBVc4f2pGa1iKEYnKTy+lTRFL0+Erb2aZ5cLmq73YJ/F4E9gJXrui
dE3qivt9FUXafAwbjETO6gQRoQwq0kjFNrTaMSmR6zZdDxjtkz/ccu0CgYEAylfj
73Xsxy6m12Xs+NOY6JK1q2JcClWG4mWhTKmrOTav7+PIMiw4nOxqnWT/GsfyrJM2
LKtqBpt8A9uk8+HrD4nMWoH/nDKQYcTDqs/zLzXKDOZQDs5jvu4Rx/qKfnmfoEyX
6FiujvAupzcU8VN7xDWV16AEwforTvJHPZl6h48CgYEAwrIV03avZZrnl8eoCIvI
gXwP7R8BymQhiPRcVllrzHqyLOp7VQaP2okJPN43zjLQVykq49NTjZrf5IsILRK8
ROxQzyXjiE7FNR+muNGEaf6otPJXzLwTQUDoT4Kd9QBrlfY8zmficB+xpBH2EfbO
gZ0H7bDtuWmFAjjAIXomeZUCgYEAxLVtAf7CkWDpEb4lAkGzBWaa9mJI2EMNPktO
5BiOPLDNmYj3ts8GuoebnifxWTfI07zJjeOWz5WYmqeNVQrsWc68u8BlnpQJiQKg
tg4zfFHue1vruHksvxDyeFc5DHtwk4XiqB6OlKcqs6T41yMxzy3I3jbSaUGrmLQw
TdJHLEECgYAVPySWJDuJiNCw3dr0qiMfqMscr3mR+5TBwMW70PgR2ykviRGwBPO/
9IbzLofRhenuoh4hcKV640IKUdpIheGhKf32X8WlMLvLNxwDT013VKAeHktWI72N
1FDNwUG1+M/XtpChAwUzKMTLZ3DrvoJqT/7BuFYwAr0R6+5o0r7gcw==\n"
"-----END RSA PRIVATE KEY-----\n";


/*Function to send sensor data to the server
This function sends the heart rate, oxygen saturation, and sitting time data to a server via HTTP.
It constructs the URL path with the sensor data, makes a GET request, and handles the server's response.*/
void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime);

/*Connect to AWS*/
void connectAWS();
void messageHandler(String &topic, String &payload);
#endif // CLOUDCONNECT_H