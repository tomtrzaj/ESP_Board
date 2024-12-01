#ifndef task2_h
#define task2_h

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include "definicje.h"
#include "struktury_def.h"
//#include <main.h>

#define CYKL_TASK2 2000 // cykl działania co ile ms


DHT *dht = new DHT();



/// TASK  obsługa cykliczna co cykl_tasku czujnika temperatury i zapisywanie do CzujnikTempDHT

void task2(void *parameters)
{
   
   float t;
   float h;
  dht->setup(DHT_pin);
  for(;;)
  {
   
    t = dht->getTemperature();
    h = dht->getHumidity();
    
    
    //Serial.print("task2  Core: ");  Serial.println(xPortGetCoreID());
   

   // Sprawdzamy poprawność danych
   if (dht->getStatus())
   {
    // Jeśli nie, wyświetlamy informację o błędzie
    Temp_DHT.connect=0;
    Serial.println("Blad odczytu danych z czujnika");
   } 
   else
    { 
      Temp_DHT.connect=1;
      Temp_DHT.temp=t;
      Temp_DHT.hum=h;

    // Jeśli tak, wyświetlamy wyniki pomiaru
     //Serial.print("Wilgotnosc: ");
     //Serial.print(Temp_DHT.hum);
     //Serial.print(" % ");
     //Serial.print("Temperatura: ");
     //Serial.print(CzujnikTempDHT.temp);
     //Serial.println(" *C ");

     //Serial.print("Remaining Stack Memory: "); Serial.println(uxTaskGetStackHighWaterMark(NULL));
     //Serial.print("Free heap  Memory: "); Serial.println(xPortGetFreeHeapSize());
    }
  
    vTaskDelay(CYKL_TASK2/portTICK_PERIOD_MS);
  }
}



#endif