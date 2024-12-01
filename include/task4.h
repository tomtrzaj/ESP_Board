#ifndef task4_h
#define task4_h

#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "definicje.h"
#include "struktury_def.h"

#define CYKL_TASK4 5000 // cykl działania co ile ms

BH1750 lightMeter1(0x23); // czujnik światła na I2C
BH1750 lightMeter2(0x5C); // czujnik światła na I2C



/// TASK  obsługa cykliczna co cykl_tasku czujników temperatur DS1820  do 4 sztuk na jednej linii 1-wire

void task4(void *parameters)
{
    Wire.begin();
   if (lightMeter1.begin(BH1750::CONTINUOUS_LOW_RES_MODE,0x23,&Wire))   {Serial.println(F("Ligh Sensor BH1750  ID 1 - OK"));LuxBH1750.connect_LUX1=1;}
   else {Serial.println(F("Error initialising Ligh Sensor 1  BH1750")); LuxBH1750.connect_LUX1=0;}

   if (lightMeter2.begin(BH1750::CONTINUOUS_LOW_RES_MODE,0x5C,&Wire))   {Serial.println(F("Ligh Sensor BH1750  ID 2 - OK"));LuxBH1750.connect_LUX2=1;}
   else {Serial.println(F("Error initialising Ligh Sensor 2  BH1750")); LuxBH1750.connect_LUX2=0;}
  


  for(;;)
  {
     // --SENSOR ŚWIATŁA na I2C   BH1750 sprawdzanie cykliczne
    if(LuxBH1750.connect_LUX1)     // mierz tylko kiedy foto2 jest podłaczony 
     {if(lightMeter1.measurementReady())  {LuxBH1750.lux[0] = lightMeter1.readLightLevel();}
      if(LuxBH1750.lux[0]  < 0 ) {LuxBH1750.connect_LUX1=0;} 
     } 
    else // ponownie próbujemy podłaczyc
      {
        if(lightMeter1.begin(BH1750::CONTINUOUS_LOW_RES_MODE))   LuxBH1750.connect_LUX1=1;    // ponowna inicjacja 
        else LuxBH1750.connect_LUX1=0;
      } 
    // lux --2-----
    if(LuxBH1750.connect_LUX2)     // mierz tylko kiedy foto2 jest podłaczony 
     {
      if(lightMeter2.measurementReady())  {LuxBH1750.lux[1] = lightMeter2.readLightLevel();}
      if(LuxBH1750.lux[1]  < 0 ) {LuxBH1750.connect_LUX2=0;} 
     } 
    else // ponownie próbujemy podłaczyc
      {
        if(lightMeter2.begin(BH1750::CONTINUOUS_LOW_RES_MODE))   LuxBH1750.connect_LUX2=1;    // ponowna inicjacja 
        else LuxBH1750.connect_LUX2=0;
      }    

  
    vTaskDelay(CYKL_TASK4/portTICK_PERIOD_MS);
  }
}

#endif