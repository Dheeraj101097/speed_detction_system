#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

// IR Sensor Pins
int ir_s1 = 21; // IR sensor 1 connected to GPIO 21
int ir_s2 = 19; // IR sensor 2 connected to GPIO 19

int timer1;
int timer2;

float Time;
float distance = 0.1; // Distance in meters (10 cm = 0.1 m)
float speed;

int flag1 = 0;
int flag2 = 0;

// WiFi Credentials
const char* ssid = "wifi-home";      // Replace with your WiFi SSID
const char* password = "12345678"; // Replace with your WiFi Password

// Backend Link
const char* serverUrl = "";

void setup() {
  Serial.begin(115200);  
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (digitalRead(ir_s1) == LOW && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }

  if (digitalRead(ir_s2) == LOW && flag2 == 0) {
    timer2 = millis();
    flag2 = 1;
  }

  if (flag1 == 1 && flag2 == 1) {
    // Calculate the time difference in seconds
    if (timer1 > timer2) {
      Time = (timer1 - timer2) / 1000.0;
    } else {
      Time = (timer2 - timer1) / 1000.0;
    }

    // Calculate speed in km/h
    speed = (distance / Time) * 3600.0;

    // Convert speed to JSON and post to the backend
    sendSpeedToBackend(speed);

    // Reset flags and speed
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }
}

void sendSpeedToBackend(float speed) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Create JSON payload
    StaticJsonDocument<200> doc;
    doc["speed"] = speed;
    String jsonPayload;
    serializeJson(doc, jsonPayload);

    // Send POST request
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);

    // Print response
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // End connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
