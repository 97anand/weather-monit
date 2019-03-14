#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "your ssid"
#define WLAN_PASS       "your password"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "your adaf username"
#define AIO_KEY         "your aio key"
int x = 1;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temp1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish gas1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gas");
Adafruit_MQTT_Publish sound1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sound");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Weather Station"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


}



void loop() {
  MQTT_connect();

  float temp = analogRead(A0);
  temp = temp*3*0.097;
  int sound = digitalRead(D2);
  int gas = digitalRead(D1);  
  Serial.print(".");    

 delay(1500);
  
    if (! temp1.publish(temp))
    {
      Serial.println(F("Failed"));
    } else 
    {
      Serial.println(F("OK!"));
    }
    
delay(1500);
  
    if (! gas1.publish(gas)){
          Serial.println(F("Failed"));
     } else
    {
      Serial.println(F("OK!"));
    }
    
   delay(1500);
  
    if (! sound1.publish(sound))
    {
    Serial.println(F("Failed"));
    } else
    {
    Serial.println(F("OK!"));
     } 
}
















void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
