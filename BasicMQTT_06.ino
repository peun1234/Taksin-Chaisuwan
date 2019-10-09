#include <LedControl.h>
#include "DHTesp.h"
#define dhtPin 15
#define Pin_DIN 18
#define Pin_cs  21
#define Pin_CLK  19
DHTesp dht;
LedControl lc = LedControl(Pin_DIN,Pin_CLK,Pin_cs,1);

unsigned long delaytime=250;
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "pt";                                                 //ชื่อ HP.
const char *pass = "1234567890";
const char *Anto_URL = "https://api.anto.io/request/";
const char *token = "lnPgWzND9d0rHkoXRDv2dCr1AuQx7vYPxhe5leBw";
const char *Data1 = "/gas.ulg95.price";
const char *Data2 = "/gas.hsd_premium.price";
//const char *Data3 = "/gas.95e20.price";
//const char *Data4 = "/gas.hsd.price";

char get_URL1[150];
char get_URL2[150];
//char get_URL3[150];
//char get_URL4[150];
int pp;
int p1;
int p2;
int p3;
int p4;

void setup()
{
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  dht.setup(dhtPin, DHTesp::DHT22);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
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
}



void loop()
{
  float Price_GAS95;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    strcpy(get_URL1, Anto_URL);
    strcat(get_URL1, token);
    strcat(get_URL1, Data1);
    strcpy(get_URL2, Anto_URL);
    strcat(get_URL2, token);
    strcat(get_URL2, Data2);
//    strcpy(get_URL3, Anto_URL);
//    strcat(get_URL3, token);
//    strcat(get_URL3, Data3);
//    strcpy(get_URL4, Anto_URL);
//    strcat(get_URL4, token);
//    strcat(get_URL4, Data4);
    http.begin(get_URL1);
    int httpCode1 = http.GET();
    if (httpCode1 > 0) {
      String payload1 = http.getString();
      Serial.println(payload1);
      int nLoct1 = payload1.indexOf(',');
      String cString1 = payload1.substring(nLoct1 + 10, nLoct1 + 15);
      Price_GAS95 = cString1.toFloat();
      Serial.println(Price_GAS95);
  pp = Price_GAS95*100;
  p1 = pp%10;
  p2 = (pp%100 - p1)/10;
  p3 = (pp%1000 - p1 - p2)/100;
  p4 = pp/1000;
  lc.setDigit(0,7,6,false);
  lc.setChar(0,6,9,false);
  lc.setDigit(0,5,5,false);
  lc.setRow(0,4,B00000001);
  lc.setDigit(0,3,p4,false);
  lc.setDigit(0,2,p3,false);
  lc.setDigit(0,1,p2,false);
  lc.setDigit(0,0,p1,false);
  delay(500);
      
    }

    http.begin(get_URL2);
    int httpCode2 = http.GET();
    if (httpCode2 > 0) {
      String payload2 = http.getString();
      Serial.println(payload2);
      int nLoct2 = payload2.indexOf(',');
      String cString2 = payload2.substring(nLoct2 + 10, nLoct2 + 15);
      Price_GAS95 = cString2.toFloat();
      Serial.println(Price_GAS95);
  pp = Price_GAS95*100;
  p1 = pp%10;
  p2 = (pp%100 - p1)/10;
  p3 = (pp%1000 - p1 - p2)/100;
  p4 = pp/1000;
  lc.setRow(0,7,B00110111);
  lc.setRow(0,6,B01011011);
  lc.setRow(0,5,B00111101);
  lc.setRow(0,4,B00000001);
  lc.setDigit(0,3,p4,false);
  lc.setDigit(0,2,p3,false);
  lc.setDigit(0,1,p2,false);
  lc.setDigit(0,0,p1,false);
  delay(500);
    
    }
 
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
 {
lc.setRow(0,7,B00011111);
 lc.setRow(0,6,B01010111);
 lc.setRow(0,5,B01110110);
 lc.setRow(0,4,B01100111);
 lc.setRow(0,3,B00000001);
 lc.setDigit(0,2,Tempp2,false);
 lc.setDigit(0,1,Tempp1,true);
 lc.setDigit(0,0,Tempp0,false);
 
delay(1000);
 }{
 lc.setRow(0,7,B00110111);
 lc.setRow(0,6,B00011100);
 lc.setRow(0,5,B01110110);
 lc.setRow(0,4,B00111101);
 lc.setRow(0,3,B00000001);
 lc.setDigit(0,2,Humi2,false);
 lc.setDigit(0,1,Humi1,true);
 lc.setDigit(0,0,Humi0,false);

    delay(1000);
 }
    http.end();
  }


}
