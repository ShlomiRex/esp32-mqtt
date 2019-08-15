#include <WiFi.h>
#include <PubSubClient.h>

/* 
------------------ NETWORK CONFIGURATION ------------------
*/
const char* ssid = "Shark";
const char* password =  "0542557736";

/* 
------------------ MQTT CONFIGURATION ------------------
*/
const char* mqttServer = "10.0.0.1";
const int mqttPort = 1883;
const char* mqttUser = "user1";
const char* mqttPassword = "pass1";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP32");
 
}
 
void loop() {
  client.loop();
}
