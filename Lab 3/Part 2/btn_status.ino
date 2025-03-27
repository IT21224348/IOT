#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Type your WiFi Name";
const char* password = "Type Your Password";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic = "test/btn";

const int btnpin = 14;
int lastBtnStatus = HIGH;

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200);

  pinMode(btnpin,INPUT_PULLUP);

  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED){
       delay(1000);
       Serial.println("Connecting to WiFI....");
  }
  Serial.println("WiFi Connected");

  client.setServer(mqtt_server,mqtt_port);
}

void loop() {
  if(!client.connected()){
     while(!client.connected()){
       Serial.println("Connected to MQTT...");
       if(client.connect("ESP32_Publisher")){
        Serial.println("Connected");
       }else{
        delay(1000);
       }
     }
  }

  int btnStatus = digitalRead(btnpin);

  if(btnStatus != lastBtnStatus){
    if(btnStatus == LOW){
      client.publish(topic, "ON");
      Serial.println("Published: ON");
    }else{
      client.publish(topic, "OFF");
      Serial.println("Published: OFF");
    }
    delay(50);
  }
  lastBtnStatus = btnStatus;
  client.loop();
}
