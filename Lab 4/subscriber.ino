#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Your WiFi Name";
const char* password = "Your WiFi Password";

// MQTT broker details
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "test/lab4";          // Topic to subscribe to for motion status
const char* feedback_topic = "test/lab4/feedback";  // Topic to publish feedback

// LED configuration
#define LED_PIN 2  // GPIO pin for the LED (GPIO 2 is often the built-in LED on ESP32)
bool ledState = false;  // Track the current state of the LED

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// Callback function to handle incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  // Convert the payload to a string
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(message);

  // Control the LED based on the message
  if (message == "MOTION") {
    digitalWrite(LED_PIN, HIGH);  // Turn LED ON
    if (ledState != true) {  // Only send feedback if the state changes
      ledState = true;
      client.publish(feedback_topic, "LED turned ON: Motion detected");
      Serial.println("LED turned ON: Motion detected");
    }
  } else if (message == "NO_MOTION") {
    digitalWrite(LED_PIN, LOW);  // Turn LED OFF
    if (ledState != false) {  // Only send feedback if the state changes
      ledState = false;
      client.publish(feedback_topic, "LED turned OFF: No motion");
      Serial.println("LED turned OFF: No motion");
    }
  }
}

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(100);  // Small delay to let Serial initialize

  // Initialize the LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED OFF

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  // Set MQTT server and callback function
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected to MQTT
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32_Subscriber")) {
      Serial.println("Connected to MQTT");
      // Subscribe to the topic
      client.subscribe(mqtt_topic);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic);
    } else {
      Serial.println("Failed to connect to MQTT, retrying...");
      delay(1000);
    }
  }
}

void loop() {
  // Ensure MQTT connection is active
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Process incoming MQTT messages
}
