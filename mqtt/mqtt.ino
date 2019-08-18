#include <WiFi.h>
#include <PubSubClient.h>

/* 
------------------ NETWORK CONFIGURATION ------------------
*/
const char* ssid = "shlomi";
const char* password =  "12345678";

/* 
------------------ MQTT CONFIGURATION ------------------
*/
const char* mqttServer = "192.168.43.250";
const int mqttPort = 1883;
const char* mqttUser = "user1";
const char* mqttPassword = "pass1";

/* 
------------------ MQTT CONFIGURATION ------------------
*/
#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino
BluetoothSerial ESP_BT; //Object for Bluetooth


WiFiClient espClient;
PubSubClient mqtt_client(espClient);
 
void setup() {
 
  Serial.begin(115200);

  Serial.println("Hello World");

  ESP_BT.register_callback(callback);
  
  if(!  ESP_BT.begin("ESP32 ")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth Device is Ready to Pair");
  }

  /*
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  
  mqtt_client.setServer(mqttServer, mqttPort);
 
  while (!mqtt_client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (mqtt_client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(mqtt_client.state());
      delay(2000);
 
    }
  }
 
  mqtt_client.publish("esp/sensors", "Hello from ESP32");
 */
}
 
void loop() {
  //mqtt_client.loop();

}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
// Callback function implementation
if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
  }
  Serial.println("Event: ");
  Serial.println(event);
}
