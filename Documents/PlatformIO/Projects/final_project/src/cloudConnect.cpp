#include "cloudConnect.h"

// make sure to enable "Maximize Compatibility" in iPhone HotSpot Setting
char ssid[50] = "Jeremy";  
char pass[50] = "99999999";

void nvs_access() {
  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  // Open
  Serial.printf("\n");
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... ");
  nvs_handle_t my_handle;
  err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    Serial.printf("Done\n");
    Serial.printf("Retrieving SSID/PASSWD\n");
    size_t ssid_len;
    size_t pass_len;
    err = nvs_get_str(my_handle, "ssid", ssid, &ssid_len);
    err |= nvs_get_str(my_handle, "pass", pass, &pass_len);
    switch (err) {
      case ESP_OK:
        Serial.printf("Done\n");
        Serial.printf("SSID = %s\n", ssid);
        Serial.printf("PASSWD = %s\n", pass);
        break;
      case ESP_ERR_NVS_NOT_FOUND:
        Serial.printf("The value is not initialized yet!\n");
        break;
      default:
        Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
  }

  // Close
  nvs_close(my_handle);
}

void connectWiFi() {
    delay(1000);
    nvs_access();
    delay(1000);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());
}

void sendSensorData(float heartRate, float oxygen, unsigned long elapsedTime) {
  // Validate inputs
  if (heartRate <= 0 || oxygen <= 0 || oxygen > 100 || elapsedTime < 0) {
    Serial.println("Invalid sensor data. Request not sent.");
    return;
  }

  WiFiClient c;
  HttpClient http(c);     

  // Construct URL path with sensor data
  // String urlPath = "/?heartRate=" + String(heartRate) +
  //                  "&oxygen=" + String(oxygen) +
  //                  "&sittingTime=" + String(elapsedTime);

  // int err = http.get(serverIP, serverPort, urlPath.c_str());
  String payload = "{\"heartRate\":" + String(heartRate) +
                 ",\"oxygen\":" + String(oxygen) +
                 ",\"sittingTime\":" + String(elapsedTime) + "}";
  err = http.post(serverIP, serverPort, "/update", "application/json", payload);

  if (err == 0) {
    Serial.println("Started request successfully");

    err = http.responseStatusCode();
    if (err >= 0) {
      Serial.print("Got status code: ");
      Serial.println(err);

      err = http.skipResponseHeaders();
      if (err >= 0) {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println("Body returned follows:");

        // Read and print the body
        unsigned long timeoutStart = millis();
        char c;

        while ((http.connected() || http.available()) && 
              ((millis() - timeoutStart) < kNetworkTimeout)) {
          if (http.available()) {
            c = http.read();
            Serial.print(c);

            bodyLen--;
            timeoutStart = millis(); // Reset timeout counter
          } else {
            delay(kNetworkDelay); // Wait for data
          }
        }
      } else {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    } else {
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  } else {
    Serial.print("Connection failed: ");
    Serial.println(err);
  }

  http.stop();
}