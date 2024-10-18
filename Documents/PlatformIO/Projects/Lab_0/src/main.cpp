#include <Arduino.h>
#include <Servo.h>

// Declare variables for pins
const int lightSensorPin = 32;
const int ServoPin = 2;
const int LED = 13;

Servo myServo;
int maxVal = 0;
int minVal = 1023; 
int lightVal = 0; 
unsigned long startTime = 0; 
unsigned long calibrationTime = 10000;

void setup() {
  myServo.attach(ServoPin);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  startTime = millis(); 
}

void loop() {
  unsigned long runTime = millis();

  // Calibration Phrase (10s long) 
  if (runTime - startTime <= calibrationTime) {
    // Turn on the blinking light
    digitalWrite(LED, HIGH);
    lightVal = analogRead(lightSensorPin);
    int maxVal = max(lightVal, maxVal);
    int minVal = min(lightVal, minVal);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
  // Operation Phrase
  else {
    // Turn off the blinking light
    digitalWrite(LED, LOW);// move this outside of the current for loop 
    lightVal = analogRead(lightSensorPin);
    lightVal = map(lightVal, minVal, maxVal, 0, 179); 
    myServo.write(lightVal);

    // delay for letting motor take a break
    delay(3000); 
  }
}