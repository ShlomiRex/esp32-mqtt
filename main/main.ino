/* 
------------------ PROJECT CONFIGURATIONS ------------------
*/
//Edit this section to enable/disable services on esp32
#define USE_WIFI 0
#define USE_MQTT 0
#define USE_BLT 0
#define USE_SENSORS 0

/* 
------------------ DEFINES ------------------
*/
#define DHTTYPE DHT11
#define DHTPin 12
#define InPin 14 // input for light sensor

/* 
------------------ WIFI CONFIGURATION ------------------
*/
#if(USE_WIFI == 1)
#include <WiFi.h>

const char* ssid = "shlomi";
const char* password =  "12345678";
WiFiClient espClient;

#endif
/* 
------------------ MQTT CONFIGURATION ------------------
*/

#if(USE_MQTT == 1)
#include <PubSubClient.h>
const char* mqttServer = "192.168.43.250";
const int mqttPort = 1883;
const char* mqttUser = "user1";
const char* mqttPassword = "pass1";
PubSubClient mqtt_client(espClient);
const char* mqtt_topic_sensors = "esp/sensors";
#endif

/* 
------------------ BLT CONFIGURATION ------------------
*/

#if(USE_BLT == 1)
#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial SerialBT;
const char* BLT_NAME = "ESP32 Bluetooth";

#endif


/* 
------------------ SENSORS CONFIGURATION ------------------
*/

#if(USE_SENSORS == 1)
#include "DHT.h"

float temperature = 0;
float fahrenheit = 0;
float humidity = 0;
int lightValue; // stores the light sensor input


char tempString[8];
char humString[8];
char fahrString[8];

DHT dht(DHTPin, DHTTYPE);

#endif





 
void setup() {
 
  Serial.begin(115200);

  ble_setup();
  dht_setup();
  wifi_setup();
  mqtt_setup();
}

/**
 * Setup sensors.
 */ 
void dht_setup() {
  #if(USE_SENSORS == 1) 
    dht.begin();
  #endif
}

/**
 * Setup bluetooth client.
 */ 
void ble_setup() {
  #if(USE_BLT == 1)
    SerialBT.begin(BLT_NAME); //Bluetooth device name
    Serial.println("The device " + BLT_NAME  + " started, now you can pair it with bluetooth!");
  #endif
}

/**
 * Setup wifi client.
 */ 
void wifi_setup() {
  #if(USE_WIFI == 1)
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
  
    Serial.println("Connected to the WiFi network");
  #endif
}

/**
 * Setup mqtt client.
 */
void mqtt_setup() {
  #if(USE_MQTT == 1)
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
  
    mqtt_client.publish(mqtt_topic_sensors, "Hello from ESP32");
  #endif
}

/**
 * Read sensors data, store in variable. This func is called in main loop function.
 */ 
void read_sensors() {
  #if(USE_SENSORS == 1)
    // Read temperature in celsius
  temperature = dht.readTemperature();
  // Read temperature as fahrenheit
  fahrenheit = dht.readTemperature(true);
  // Read humidity
  humidity = dht.readHumidity();

   //light sensor:
  lightValue = analogRead( InPin );

  Serial.println("Light Value: " + String(lightValue));


  float hic = dht.computeHeatIndex(temperature, humidity, false);
  Serial.print("Humidity: "); 
  Serial.print(humidity); 
  Serial.print(" %\t"); 
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C \t"); 
  Serial.print("Heat index: "); 
  Serial.print(hic); 
  Serial.print(" *C \n");

  if (!isnan(humidity) && !isnan(temperature) & !isnan(fahrenheit)) {
    Serial.printf("\n Humidity: %.2f , Temperature: %.2fC, %.2fF , ", humidity, temperature, fahrenheit);
    Serial.printf("\n LightSensor: %d , ",lightValue);


    // Convert the value to a char array
    dtostrf(temperature, 4, 2, tempString);
    dtostrf(fahrenheit, 4, 2, fahrString);
    dtostrf(humidity, 4, 2, humString);
  }
  #endif
}

/**
 * Read bluetooth serial and print to console.
 */
void read_blt() {
  #if(USE_BLT == 1)
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  #endif
}


/**
 * Publish sensors data to mqtt.
 */
void publish_sensors_to_mqtt() {
  #if(USE_MQTT == 1)
  String json =
    "{"
    "\"celsius\": \"" + String(temperature) + "\","
    "\"fahrenheit\": \"" + String(fahrenheit) + "\","
    "\"humidity\": \"" + String(humidity) + "\","
    "\"lightSensor\": \"" + lightValue + "\"}";

  // Convert JSON string to character array
  char jsonChar [100];
  json.toCharArray(jsonChar, json.length() + 1);

  // Publish JSON character array to MQTT topic
  client.publish(mqtt_topic_sensors, jsonChar);
  delay(1000);
  #endif
}
 
/**
 * Main loop.
 */ 
void loop() {
  #if(USE_MQTT == 1)
  mqtt_client.loop();
  #endif

  read_blt();
  read_sensors();
  publish_sensors_to_mqtt();
}
