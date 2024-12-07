#ifndef CLOUDCONNECT_H
#define CLOUDCONNECT_H

#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include "WiFi.h"

void nvs_access();

void connectAWS(WiFiClientSecure &net, MQTTClient &client);

void publishMessage(MQTTClient &client, float avgHeart, float avgOxy);

void messageHandler(String &topic, String &payload);

#endif // CLOUDCONNECT_H