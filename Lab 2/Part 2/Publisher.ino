#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "winterfell";
const char* password = "RaV?=27jn";
const char* mqtt_server = "broker.hivemq.com";
const char* topic = "test/topic";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
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
  // Alternate between ON and OFF
  client.publish(topic, "ON");
  Serial.println("Published: ON");
  delay(2000);
  
  client.publish(topic, "OFF");
  Serial.println("Published: OFF");
  delay(2000);
}