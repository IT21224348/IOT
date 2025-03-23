#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "winterfell";
const char* password = "RaV?=27jn";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
const char* topic = "test/topic";

void callback(char* topic, byte* message, unsigned int length){
   Serial.print("Message Recevied on the topic: ");
   Serial.println(topic);
   Serial.print("Message: ");
   for(int i =0; i < length; i++){
     Serial.print((char)message[i]);
   }
   Serial.println();
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);

}

void loop() {
 if(!client.connected()){
      while(!client.connected()){
        Serial.println("Connecting to MQTT...");
        if(client.connect("ESP32_Subscriber")){
          client.subscribe(topic);
          Serial.println("Connected and Subscribed");
        }else{
          delay(5000);
        }
      }
   }
  client.loop();
}
