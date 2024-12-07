#include <Arduino.h>
#include <stdio.h>

// // Sensors
#include "TouchSensor.h"
#include "BioSensor.h"
#include "Accelerometer.h"

// Cloud Connection
#include "cloudConnect.h"


// Reset pin, MFIO pin, Buzzer pin
const int resPin = 37;
const int mfioPin = 38;
const int buzzer = 25;

// Sensors
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin); // Biosensor
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET); // Touch Sensor
LSM6DSO myIMU; // Accelerometer
TFT_eSPI tft = TFT_eSPI(); // screen 

// cloud 
WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(512);

// variable for logic control and calibration
bool touched = false;
bool calibrated = false;
unsigned long sittingTime = 0;
unsigned long heartRateSum = 0;
unsigned long oxygenSum = 0;
unsigned int heartRateCount = 0;
float averageHeartRate = 0; 
float averageOxygen = 0;

float min_X = 0;
float max_X = 0;

void setup(){
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  connectAWS(net, client);

  Wire.begin();

  initBioSensor(bioHub);
  initTouchSensor(cap, tft); 
  initAccel(myIMU);

  sittingTime = millis(); // initialize the timestamp
 
}

void loop(){ 
  client.loop();

  // // Once touched, start recording user data
  if (!touched && readTouch(cap, tft)) {
    tone(buzzer, 1000, 300);
    noTone(buzzer);
    calibrateAccel(myIMU, min_X, max_X); 
    sittingTime = millis(); // reset the timestamp
    touched = true;
  }

  delay(100); 

  if (touched) {
    bioData body = readBioData(bioHub); // biosensor needs 5-10s to start updating 
    if (receiveValidData(body)) // once the data is validated, send it to the "database" 
    {
      heartRateSum += body.heartRate;
      oxygenSum += body.oxygen; 
      heartRateCount++;
    }
    float sittingData = readAccel(myIMU, min_X, max_X); 
    unsigned long elapsedTime = (millis() - sittingTime) / 1000;

    // display the sitting time on the screen
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0,15);
    tft.fillRect(0, 15, 120, 20, TFT_BLACK);
    tft.print("time: ");
    tft.setCursor(0,65);
    tft.print(elapsedTime);
    tft.println("s");

    // once the user is not on the object
    if (!onPosition(sittingData)) {
      averageHeartRate = (heartRateCount > 0) ? (float)heartRateSum / heartRateCount : 0;
      averageOxygen = (heartRateCount > 0) ? (float)oxygenSum / heartRateCount : 0;
      Serial.print("Average Heart Rate: ");
      Serial.print(averageHeartRate, 2); 
      Serial.println(" bpm");
      Serial.print("Average Oxygen: ");
      Serial.print(averageOxygen, 2); 
      Serial.println(" lpm");

      // Publish the averages to MQTT
      publishMessage(client, averageHeartRate, averageOxygen);
      delay(2000);

      // Reset for next session
      touched = false;
      heartRateSum = 0;
      oxygenSum = 0;
      heartRateCount = 0;
    }
  }
}
