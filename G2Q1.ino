#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include "LedControl.h"
#define Pin_DIN 19
#define Pin_cs  18
#define Pin_CLK  5
#define Pin_DHT22 15
LedControl lc = LedControl(Pin_DIN,Pin_CLK,Pin_cs,1);
const char* ssid = "HUAWEI nova 3i";
const char* password = "0950580791";
const char* mqtt_server = "mqtt.eclipse.org";  //test.mosquitto.org  //broker.mqtt-dashboard.com
const char* topic = "IOT";
DHTesp dht;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish(topic, "0");
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.setup(15);
}

void loop() {
float t = dht.getTemperature();
float h= dht.getHumidity();
 float t2 =t*10;
  float h2 =h*10;
  Serial.print("temperature:");
  Serial.print(t); Serial.print(" *c\t");
  Serial.print("Humidity:");
  Serial.print(h); Serial.print("%\n");

 int Temp =(int)t;
 int Temp2 = (int)t2;
 int hu = (int)h;
 int hu2 = (int)h2;
 int Tempp2 = (Temp/10)%10;
 int Tempp1 = Temp%10;
 int Tempp0 = Temp2%10;
 int Humi2 = (hu/10)%10;
 int Humi1 = hu%10;
 int Humi0 =hu2%10;

 lc.setDigit(0,7,Tempp2,false);
 lc.setDigit(0,6,Tempp1,true);
 lc.setDigit(0,5,Tempp0,false);
 lc.setRow(0,4,B00001101);
 lc.setDigit(0,3,Humi2,false);
 lc.setDigit(0,2,Humi1,true);
 lc.setDigit(0,1,Humi0,false);
 lc.setRow(0,0,B00010111);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "temperature: #%lf.2 Humidity:  #%lf.2", t,h);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic, msg);
  }
  

}
