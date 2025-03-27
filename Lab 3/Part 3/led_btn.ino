#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Type your WiFi Name";
const char* password = "Type Your Password";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic = "lab3/test";

const int ledpin = 12;

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
  if (msg == "true") {
    digitalWrite(ledpin, HIGH); // Turn LED ON
    Serial.println("LED turned ON");
  } else if (msg == "false") {
    digitalWrite(ledpin, LOW);  // Turn LED OFF
    Serial.println("LED turned OFF");
  }
}


void setup() {
  Serial.begin(115200);

    // Set LED pin as output
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW); // Start with LED off

  WiFi.begin(ssid,password);
    while(WiFi.status()!= WL_CONNECTED){
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
      Serial.println("WiFi Connected");
  
    client.setServer(mqtt_server,mqtt_port);
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
