#ifndef CLOUDCONNECT_H
#define CLOUDCONNECT_H

#include <HttpClient.h>
#include <WiFi.h>
#include <inttypes.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"


// EC2 Server information
const char serverIP[] = "3.93.25.243";  // EC2 IP address (varied each time launch the instance)
const int serverPort = 5000;
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

/* Function to initialize and access Non-Volatile Storage (NVS)
This function initializes the NVS storage, retrieves the saved Wi-Fi credentials (SSID and password),
and prints them to the Serial monitor. If the credentials are not found, it notifies the user.*/ 
void nvs_access(); 

/*Function to connect to Wi-Fi using stored credentials
This function retrieves Wi-Fi credentials from NVS and attempts to connect to the network.
It prints the connection status, IP address, and MAC address once connected.*/
void connectWiFi();

/*Function to send sensor data to the server
This function sends the heart rate, oxygen saturation, and sitting time data to a server via HTTP.
It constructs the URL path with the sensor data, makes a GET request, and handles the server's response.*/
void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime);

#endif // CLOUDCONNECT_H