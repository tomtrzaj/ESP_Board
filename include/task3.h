#ifndef task3_h
#define task3_h

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "definicje.h"
#include "struktury_def.h"
//#include <main.h>

#define CYKL_TASK3 4000 // cykl działania co ile ms

OneWire *onewire1=new OneWire(DS1820_pin); //Podłączenie do D6
DallasTemperature *TempDS1820;



/// TASK  obsługa cykliczna co cykl_tasku czujników temperatur DS1820  do 4 sztuk na jednej linii 1-wire

void task3(void *parameters)
{
  TempDS1820=new DallasTemperature(onewire1);
  TempDS1820->begin();
  TempDS1820->setResolution(9);  // rozdzielczość 10bitów  0.2 C
  float Temp2;

  for(;;)
  {
    //Serial.print("task3  Core: ");  Serial.println(xPortGetCoreID());
    
    Temp_DS1820.amount_of_ds1820=TempDS1820->getDeviceCount();
    if(Temp_DS1820.amount_of_ds1820) Temp_DS1820.connect=1;
    else Temp_DS1820.connect=0;
    if(Temp_DS1820.amount_of_ds1820) TempDS1820->requestTemperatures(); // żądanie temperatury 
    
    //for (int i=0; i<4;i++) msg_ds1820.temp[i]=-127.0;// czyscimy dane na początek
    for (int i=0; i<Temp_DS1820.amount_of_ds1820;i++)
       {
         if(i>3) continue; //tylko 4 czujniki 
         Temp2=TempDS1820->getTempCByIndex(i);
         if(Temp2!=-127.0) 
         {
           Temp_DS1820.temp[i]=Temp2;
           //Serial.print("DS1820: [");Serial.print(i);Serial.print("] "); Serial.println(msg_ds1820.temp[i]);
         }  
       }
    
  
    vTaskDelay(CYKL_TASK3/portTICK_PERIOD_MS);
  }
}



#endif