# esp32-mqtt

Run an MQTT protocol on arduino

## How to setup

### Running arduino
sudo ./arduino


Very important to run as sudo!

### ESP32 Board
Preferances -> Add this URL: https://dl.espressif.com/dl/package_esp32_index.json


Tools -> Board Manager.. -> Search 'esp32' -> Install


### Selecting board
Select 'WEMOS LILON32'

### Install libraries
Tools -> Manage libraries -> search 'PubSubClient' - by Nick 'O Leary (important to instal this spesific library)



# Running subscriber

mosquitto_pub -t mytopic

# Running publisher

mosquitto_pub -t mytopic -m "my message"

# User autherntication

## Create user authentication

mosquitto_passwd -c passwordfile user


## Use authentication

http://www.steves-internet-guide.com/mqtt-username-password-example/

# Links

Tutorial: http://www.iotsharing.com/2017/05/how-to-use-mqtt-to-build-smart-home-arduino-esp32.html
MQTT Lib: https://github.com/knolleary/pubsubclient/
MQTT Code: https://techtutorialsx.com/2017/04/24/esp32-publishing-messages-to-mqtt-topic/
