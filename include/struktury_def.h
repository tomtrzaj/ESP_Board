#ifndef struktury_def_h
#define struktury_def_h

#include <Arduino.h>
/// @brief definicje structur uzytych w oprogramowaniu
struct TEMP_DHT
{
  byte  connect;      // czy jest podlączony
  float temp;           // pomiar  temperatury w stC  czujnika 1 (z dokładnąścią do 0,1 stopnia) zakres od -40 do +125
  float devp;           // temp punktu rosy czujnika 1  
  byte hum;             // pomiar wilgotności względnej czujnika 1 (w %)  zakres od 0 do 100%
}Temp_DHT;


//-------------------------------------------------
struct TEMP_DS1820
{
  byte  connect;        // czy podłaczony
  byte  amount_of_ds1820;             // ilość aktywnych czujników na linii
  byte  ilosc;                     // ilosc znalezionych DS1820 - max 4
  float temp[4];                   // temperatura w stC  czujnika z czujników(z dokładnąścią do 0,1 stopnia) zakres od -40 do +125
}Temp_DS1820;
//------------------------------------------------------

struct MSG_LUX
{  
  byte  connect_LUX1;
  byte  connect_LUX2;
  byte  amount_of_BH1750;      // ilość aktywnych czujników na linii
  float    lux[2];             // dwa czujniki 
} LuxBH1750;
//---------------------------------------------------------


struct ALARM
{
  byte  connect;        // czy podłaczony
  bool  Alarm;          // czy wystapił alarm
}Alarm;
//------------------------------------------------------



#endif
