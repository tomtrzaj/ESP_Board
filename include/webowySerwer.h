#ifndef webowySerwer_h
#define webowySerwer_h

#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>
#include "struktury_def.h"

// Replace with your network credentials
const char* ssid = "ESP32";
const char* password = "kzl87654";
IPAddress apSUBNET(255,255,255,0);
IPAddress apGATE(192,168,4,1);
IPAddress apIP(192,168,4,10);
IPAddress getIP;
//WiFiServer server(80);   // Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String wynik;


TEMP_DHT *TempDHT_ptr2 = &Temp_DHT;
TEMP_DS1820 *Temp_DS_ptr2 =&Temp_DS1820;
ALARM *Alarm_ptr2 = &Alarm;
MSG_LUX *BH1750_ptr2 =&LuxBH1750; 


//-------------------------------------------
String processor(const String& var)   // Replaces placeholder with LED state value
{
  //Serial.println(var);
  //char buff[10];
  if(var == "STATE")   {if(Alarm_ptr2->Alarm) {wynik = "ON";}    else{wynik = "OFF";}   return wynik;}
  if(var == "DHT_TEMP"){ wynik = String(TempDHT_ptr2->temp);    return wynik;}
  if(var == "HUM")     { wynik = String(TempDHT_ptr2->hum);     return wynik;} 
  if(var == "DS1"){ wynik = String(Temp_DS_ptr2->temp[0]); return wynik;}
  if(var == "DS2"){ wynik = String(Temp_DS_ptr2->temp[1]); return wynik;} 
  if(var == "DS3"){ wynik = String(Temp_DS_ptr2->temp[2]); return wynik;}
  if(var == "DS4"){ wynik = String(Temp_DS_ptr2->temp[3]); return wynik;} 
  if(var == "LUX1")    { wynik = String(BH1750_ptr2->lux[0]);   return wynik;}
  if(var == "LUX2")    { wynik = String(BH1750_ptr2->lux[1]);   return wynik;}
  if(var == "FAN1")    { wynik = String(KomponentSTATE.Tacho[0]);   return wynik;}
  if(var == "FAN2")    { wynik = String(KomponentSTATE.Tacho[1]);   return wynik;}

  return String();
}
// obsługa połączeń webowych 
//--------------------------------------------------------------------------------
void webConnect(void)
{
  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAPConfig(apIP,apGATE,apSUBNET); // ustawienia adresów
  WiFi.softAP(ssid, password);

  getIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(getIP);


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load icon file--------
  server.on("/temp_icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/temp_icon.png", "image/jpeg");
  });
   //------------------------------------
  server.on("/lux_icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/lux_icon.png", "image/jpeg");
  });
   //--------------------------------------
  server.on("/fan_icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/fan_icon.png", "image/jpeg");
  });
   //------------------------------------


  // Route to set GPIO to HIGH
  server.on("/PK1", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  //----------
  server.on("/PK2", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  //-------
  server.on("/PK3", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  //-------------
  server.on("/PK4", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  //--------------------------- 

  server.begin();  Serial.println("Web Serwer Started");
  //server.close();

}




#endif