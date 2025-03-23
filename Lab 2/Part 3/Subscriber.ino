#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "winterfell";
const char* password = "RaV?=27jn";
const char* mqtt_server = "broker.hivemq.com";
const char* topic = "test/topic";

// LED pin (change to your chosen GPIO, e.g., 2 for built-in LED)
const int ledPin = 13;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message Received on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  
  // Convert byte array to string
  String msg = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    msg += (char)message[i];
  }
  Serial.println();

  // Control LED based on message
  if (msg == "ON") {
    digitalWrite(ledPin, HIGH); // Turn LED ON
    Serial.println("LED turned ON");
  } else if (msg == "OFF") {
    digitalWrite(ledPin, LOW);  // Turn LED OFF
    Serial.println("LED turned OFF");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Set LED pin as output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP32_Subscriber")) {
        client.subscribe(topic);
        Serial.println("Connected and Subscribed");
      } else {
        delay(5000);
      }
    }
  }
  client.loop(); // Process incoming messages
}