/* 
------------------ PROJECT CONFIGURATIONS ------------------
*/
//Edit this section to enable/disable services on esp32
#define USE_WIFI 1
#define USE_MQTT 1
#define USE_BLT 1
#define USE_SENSORS 1
/* 
------------------ WIFI CONFIGURATION ------------------
*/
#if(USE_WIFI == 1)
#include <WiFi.h>

//NOT const because BLT can change it. (wifi_ssid can change based on BLT requests)
char* wifi_ssid = "Ariel_University"; //Please leave this as default
char* wifi_password =  "";
WiFiClient espClient;

#endif
/* 
------------------ MQTT CONFIGURATION ------------------
*/

#if(USE_MQTT == 1)
#include <PubSubClient.h>
const char* mqttServer = "192.168.43.12";
const int mqttPort = 1883;  
const char* mqttUser = "";
const char* mqttPassword = "";
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


/* 
------------------ BLT PROTOCOL CONFIGURATION ------------------
*/

//PC = Protocol Code
const char PC_CHANGE_WIFI = '5';


#endif


/* 
------------------ SENSORS CONFIGURATION ------------------
*/

#if(USE_SENSORS == 1)
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPin 12
#define InPin 14 // input for light sensor

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

  blt_setup();
  dht_setup();
  wifi_setup();
  mqtt_setup();
}

/**
 * Setup sensors.
 */ 
void dht_setup() {
  #if(USE_SENSORS == 1) 
    Serial.println("DHT Setup...");
    dht.begin();
  #endif
}

/**
 * Setup bluetooth client.
 */ 
void blt_setup() {
  #if(USE_BLT == 1)
    Serial.println("BLT Setup...");
    SerialBT.begin(BLT_NAME); //Bluetooth device name
    Serial.println("BLT Device Name: " + String(BLT_NAME));
  #endif
}

/**
 * Setup wifi client.
 */ 
void wifi_setup() {
  #if(USE_WIFI == 1)
    WiFi.disconnect();

    
    Serial.println("WiFi Setup...");
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.println("Connecting to " + String(wifi_ssid) + " and password " + String(wifi_password)); 

    Serial.println("Connecting to WiFi..");
    
    while (WiFi.status() != WL_CONNECTED) {
      //delay(500);
      //Serial.println("Connecting to WiFi..");


      //If you accidently picked wrong ssid/password you need to change it.
      //This lines prevents a deadlock of infinite loop to connect to non-existing ssid.
      String res = read_blt();
      process_blt(res);
    }
  
    Serial.println("Connected to the WiFi network");

    Serial.println("local ip: ");
    Serial.println(WiFi.localIP());
  #endif
}

/**
 * Setup mqtt client.
 */
void mqtt_setup() {
  #if(USE_MQTT == 1)
    Serial.println("MQTT Setup...");
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
  Serial.println("Reading sensors...");
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
 * @return String that bluetooth got.
 */
String read_blt() {
  #if(USE_BLT == 1)
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if(SerialBT.available()) {
    Serial.println("Reading BLT...");
  }
  String blt_str = "";
  
  while(SerialBT.available()) {
    char blt_char = SerialBT.read();
    blt_str += blt_char;
    //Serial.write(blt_char);
  }
  if(blt_str != "") {
    //Serial.println("GOT: " + blt_str);
  }
  return blt_str;
  #endif
}


/**
 * Publish sensors data to mqtt.
 */
void publish_sensors_to_mqtt() {
  #if(USE_MQTT == 1 && USE_SENSORS == 1)
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
 * Process BLT string request.  
 */
void process_blt(String str) {
#if(USE_BLT == 1)
  if(str == "") return;

  char str_len = str.length();
  
  Serial.println("BLT String: " + str);
  
  char protocol_code = str[0];
  
  if(protocol_code == PC_CHANGE_WIFI) {
    Serial.println("BLT: Change wifi requested");

    int ind = 1; //Start digesting request
    char tmp;
    
    tmp = str[ind++];
    int tens = (int)tmp - 48; //Convert to ascii
    
    tmp = str[ind++];
    int ones = (int)tmp - 48;

    //Serial.println("Tens = " + String(tens) + "  Ones = " + String(ones));
    //Serial.println("ind = " + String(ind));

    int ssid_len = tens*10 + ones; //SSID len can be from 00 to 99 (2 chars)
    //Serial.println("SSID len: " + String(ssid_len));
    
    String ssid = "";

    int tmp_int = ind;
    
    for(; ind < tmp_int + ssid_len; ind++) {
      ssid += str[ind];
    }
    Serial.println("SSID: " + ssid);

    //Read password (2 chars) (reuse variables names)
    tens = str[ind++];
    ones = str[ind++];
    tmp_int = ind;
    int pass_len = tens*10 + ones;
    String pass = "";
    for(; ind < tmp_int + pass_len; ind++) {
      pass += str[ind];
    }

    Serial.println("PASSWORD: " + pass);




    #if(USE_WIFI == 1)
      int ssid_str_len = ssid.length() + 1; //Null terminator (+1)
      char ssid_char_array[ssid_str_len];
      ssid.toCharArray(ssid_char_array, ssid_str_len);

      int pass_str_len = pass.length() + 1; //Null terminator (+1)
      char pass_char_array[pass_str_len];
      pass.toCharArray(pass_char_array, pass_str_len);
      
      wifi_ssid = ssid_char_array;
      wifi_password = pass_char_array;
      
      wifi_setup(); //Restart WiFi
    #endif
  }

#endif
}
 
/**
 * Main loop.
 */ 
void loop() {
  #if(USE_MQTT == 1)
  mqtt_client.loop();
  #endif

  String res = read_blt();
  process_blt(res);
  
  read_sensors();
  publish_sensors_to_mqtt();
}
