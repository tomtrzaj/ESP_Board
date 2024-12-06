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
  if(var == "STATE")
  {
    if(Alarm_ptr2->Alarm){wynik = "ON";}
    else{wynik = "OFF";}
    //Serial.print(wynik);
    return wynik;
  }
  if(var == "DHT_TEMP")
   {
    wynik = String(TempDHT_ptr2->temp);
    return wynik;
   }
  if(var == "HUM")
   {
    wynik = String(TempDHT_ptr2->hum);
    return wynik;
   } 
  if(var == "DS1_TEMP")
   {
    wynik = String(Temp_DS_ptr2->temp[0]);
    return wynik;
   }
  if(var == "DS2_TEMP")
   {
    wynik = String(Temp_DS_ptr2->temp[1]);
    return wynik;
   } 
  if(var == "LUX1")
   {
    wynik = String(BH1750_ptr2->lux[0]);
    return wynik;
   }


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

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    //digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });


  server.begin();  Serial.println("Web Serwer Started");
  //server.close();

}




#endif