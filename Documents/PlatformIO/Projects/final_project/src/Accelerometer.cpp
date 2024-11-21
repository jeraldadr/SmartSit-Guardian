#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM6DSO.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Pin 
const int LED = 26;

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B

// BLE variables
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLEService *pService = NULL;

// Time process
unsigned long startTime; 
unsigned long runTime;
const unsigned long calibrationTime = 10000; // 10s calibration


float min_X;
float max_X;
float x_val; 
const int THRESHOLD_VALUE = map(1.5, -2, 2, 0, 255);   
uint32_t stepCount = 0;

LSM6DSO initAccel() {
  Serial.begin(9600);
 
  // LSM6DSO Sensor Set-Up
  delay(500); 

  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");
}

  // BLE Device Set-Up
  Serial.println("Starting BLE work!");
  BLEDevice::init("CS147");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setValue("Server Starts"); //What gets transfered
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // Calibration Phrase Needed
  startTime = millis(); 
  min_X = 0.0; 
  max_X = 0.0; 
  pinMode(LED, OUTPUT); 
}

void loop() {
  runTime = millis();

  // calibration phrase
  if ((runTime - startTime) < calibrationTime) {
      digitalWrite(LED, HIGH);
      float x = myIMU.readFloatAccelX();
      min_X = min(min_X, x);
      max_X = max(max_X, x);
  }
  else {
    digitalWrite(LED, LOW);
    x_val = myIMU.readFloatAccelX();
    x_val = (x_val - min_X) * (255 - 0) / (max_X - min_X); 

    bool stepDetected = false; 
    if (x_val > THRESHOLD_VALUE && !stepDetected) {
        stepCount++;
        stepDetected = true;
        Serial.println("Step detected! Total steps: " + String(stepCount));
        
        String stepCountString = "Steps: " + String(stepCount);
        
    pCharacteristic->setValue(stepCountString.c_str());
    pCharacteristic->notify();

    } else if (x_val < THRESHOLD_VALUE) {
        stepDetected = false;  // Reset step detection
    }
    delay(20);  
  }
  
  delay(1000);
}