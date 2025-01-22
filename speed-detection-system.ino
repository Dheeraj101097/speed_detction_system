#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

int ir_s1 = 25; // IR sensor 1 connected to GPIO 25
int ir_s2 = 26; // IR sensor 2 connected to GPIO 26

unsigned long timer1 = 0; // Timestamp for IR sensor 1
unsigned long timer2 = 0; // Timestamp for IR sensor 2

float Time = 0.0;
float distance = 0.1; // Distance in meters (10 cm = 0.1 m)
float speed = 0.0;

bool objectPassedSensor1 = false; // Tracks if object passed sensor 1
bool objectPassedSensor2 = false; // Tracks if object passed sensor 2

void setup() {
  Serial.begin(115200);
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
}

void loop() {
  // Check IR sensor 1
  if (digitalRead(ir_s1) == LOW && !objectPassedSensor1) {
    timer1 = millis(); // Record the time when IR sensor 1 is triggered
    objectPassedSensor1 = true; // Set flag to true
    Serial.println("Sensor 1 triggered");
  }

  // Check IR sensor 2 (only if sensor 1 was triggered first)
  if (digitalRead(ir_s2) == LOW && objectPassedSensor1 && !objectPassedSensor2) {
    timer2 = millis(); // Record the time when IR sensor 2 is triggered
    objectPassedSensor2 = true; // Set flag to true
    Serial.println("Sensor 2 triggered");
  }

  // Calculate speed if both sensors are triggered in the correct sequence
  if (objectPassedSensor1 && objectPassedSensor2) {
    // Calculate the time difference in seconds
    Time = abs((float)(timer2 - timer1)) / 1000.0;

    if (Time > 0) { // Ensure Time is not zero to avoid division by zero
      // Calculate speed in km/h
      speed = (distance / Time) * 3600.0;
      Serial.print("Speed: ");
      Serial.print(speed);
      Serial.println(" km/h");
    } else {
      Serial.println("Error: Time is zero. Check sensor readings.");
    }

    // Reset flags for the next object detection
    objectPassedSensor1 = false;
    objectPassedSensor2 = false;
    speed = 0.0;
    Time = 0.0;
  }

  // Reset if no object moves through both sensors within a reasonable time
  if (objectPassedSensor1 && (millis() - timer1 > 5000)) {
    Serial.println("Timeout: Object did not pass sensor 2. Resetting.");
    objectPassedSensor1 = false;
    timer1 = 0;
  }
}
