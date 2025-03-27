#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Type your WiFi Name";
const char* password = "Type Your Password";
const char* mqtt_server = "broker.hivemq.com";
const char* topic = "test/topic";

// Button pin (change if using a different GPIO)
const int buttonPin = 14;
int lastButtonState = HIGH; // Track previous state for debouncing

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Set button pin as input with internal pull-up
  pinMode(buttonPin, INPUT_PULLUP);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP32_Publisher")) {
        Serial.println("Connected");
      } else {
        delay(1000);
      }
    }
  }

  // Read button state
  int buttonState = digitalRead(buttonPin);
  
  // Check for state change (basic debouncing)
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) { // Button pressed (LOW due to GND)
      client.publish(topic, "ON");
      Serial.println("Published: ON");
    } else { // Button released (HIGH due to pull-up)
      client.publish(topic, "OFF");
      Serial.println("Published: OFF");
    }
    delay(50); // Simple debounce delay
  }
  
  lastButtonState = buttonState; // Update last state
  client.loop(); // Keep MQTT alive
}
