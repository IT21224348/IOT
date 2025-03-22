#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WASEEK";
const char* password = "Waseekmlm";
const char* mqtt_server = "192.168.1.2"; // Your computer’s IP
const int mqtt_port = 1883;

const int trigPin = 5;
const int echoPin = 18;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  char msg[10];
  dtostrf(distance, 6, 2, msg);
  Serial.print("Publishing: "); // Debug line
  Serial.println(msg);          // Debug line
  client.publish("sensor/distance", msg);

  delay(2000);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 2 seconds");
      delay(2000);
    }
  }
}