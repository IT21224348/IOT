#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "winterfell";
const char* password = "RaV?=27jn";

// MQTT broker details
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic = "test/lab4";  // Topic to publish motion status

// PIR sensor configuration
#define PIR_PIN 5  // GPIO where the PIR sensor is connected

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(100);  // Small delay to let Serial initialize

  // Initialize PIR sensor pin
  pinMode(PIR_PIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  // Set MQTT server
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Reconnect to MQTT if disconnected
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP32_Publisher")) {
        Serial.println("Connected to MQTT");
      } else {
        Serial.println("Failed to connect to MQTT, retrying...");
        delay(1000);
      }
    }
  }

  // Read motion from PIR sensor
  int motion = digitalRead(PIR_PIN);

  // Publish motion status
  if (motion == HIGH) {
    client.publish(topic, "MOTION");
    Serial.println("Published: MOTION");
  } else {
    client.publish(topic, "NO_MOTION");
    Serial.println("Published: NO_MOTION");
  }

  delay(2000);  // Wait 2 seconds before next reading
  client.loop();  // Keep MQTT connection alive
}