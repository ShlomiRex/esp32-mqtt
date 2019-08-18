/* 
------------------ INCLUDES ------------------
*/
#include <WiFi.h>
#include <PubSubClient.h>
#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

/* 
------------------ NETWORK CONFIGURATION ------------------
*/
//const char* ssid = "shlomi";
//const char* password =  "12345678";
WiFiClient espClient;
/* 
------------------ MQTT CONFIGURATION ------------------
*/
//const char* mqttServer = "192.168.43.250";
//const int mqttPort = 1883;
//const char* mqttUser = "user1";
//const char* mqttPassword = "pass1";
PubSubClient mqtt_client(espClient);
/* 
------------------ BLT CONFIGURATION ------------------
*/

BluetoothSerial SerialBT;



 
void setup() {
 
  Serial.begin(115200);

  Serial.println("Hello World");
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

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
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  //delay(20);
}
