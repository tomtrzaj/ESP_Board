#ifndef task2_h
#define task2_h

#include <Arduino.h>
#include <DHT.h>

#include "definicje.h"
#include "struktury_def.h"

#define DHTTYPE    DHT22     // DHT 22 (AM2302)

//#include <main.h>

#define CYKL_TASK2 3000 // cykl działania co ile ms


//DHT *dht22 = new DHT(DHT_pin, DHTTYPE);
//DHT dht22(DHT_pin, DHTTYPE);
//dht1wire *dht = 0;
//dht1wire *dht=new dht1wire(DHT_pin, dht::DHT22);
 DHT *dht22;




/// TASK  obsługa cykliczna co cykl_tasku czujnika temperatury i zapisywanie do CzujnikTempDHT

void task2(void *parameters)
{
   
   float t;
   float h;
  //dht22->begin();
  for(;;)
  {
    dht22 = new DHT(DHT_pin, DHTTYPE);
     dht22->begin();
     vTaskDelay(100/portTICK_PERIOD_MS);
    t = dht22->readTemperature();
    h = dht22->readHumidity();
     
    //Serial.print("task2  Core: ");  Serial.println(xPortGetCoreID());
    //Sprawdzamy poprawność danych

   if(!isnan(t))
   {
     Temp_DHT.connect=1;
     Temp_DHT.temp=t;
     Temp_DHT.hum=h;
   }
   else
   {
     Temp_DHT.connect=0;
     Temp_DHT.temp=-100;
     Temp_DHT.hum=-100;
   }
   delete dht22;
    vTaskDelay(CYKL_TASK2/portTICK_PERIOD_MS);
  }
}



#endif