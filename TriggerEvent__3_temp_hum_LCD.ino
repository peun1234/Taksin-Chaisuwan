#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHTesp.h"
#include "LedControl.h"
#define dhtPin 15
#define Pin_DIN 18
#define Pin_cs  21
#define Pin_CLK  19
char ssid[] = "pt"; // your network SSID (name)
char pass[] = "1234567890"; // your network password
unsigned long myChannelnumber = 880555;
const char* myWriteAPIKey = "2C7XWQTTED434I11";
const char* myReadAIKey = "YUGAHZSXL5YL7APU";
WiFiClient client;
DHTesp dht;
LedControl lc = LedControl(Pin_DIN,Pin_CLK,Pin_cs,1);
void setup() {
  Serial.begin(115200);
  lc.shutdown(0,false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);
  dht.setup(dhtPin, DHTesp::DHT22);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
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
  
  Serial.print("\nTemperature('C)= ");
  Serial.print(t, 1);
  Serial.print("\tHumidity(%)= ");
  Serial.print(h, 1);
  Serial.println();
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.writeFields(880555, myWriteAPIKey);
  for (int i = 'Z'; i >= 'A'; i--) {
    Serial.write(i);
    delay(60000);
  }
}
