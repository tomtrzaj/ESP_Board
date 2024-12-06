#ifndef main_h
#define main_h
#include <Arduino.h>
#include <Timers.h>
#include "ServisSerialEvent.h"
#include "freertos/FreeRTOS.h"
#include <WiFi.h>
#include "webowySerwer.h"
//#include "ESPAsyncWebServer.h"
#include <SPIFFS.h>

#include <struktury_def.h>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"





#define SERIAL_BUF_MAX_LENGTH 30 // max rozmiar bufora odbiornika serwisowego


//int serial_buf_length;
//char serial_buf[SERIAL_BUF_MAX_LENGTH-1];

//zmienne globalne

TEMP_DHT *TempDHT_ptr = &Temp_DHT;
TEMP_DS1820 *Temp_DS_ptr =&Temp_DS1820;
ALARM *Alarm_ptr = &Alarm;
MSG_LUX *BH1750_ptr =&LuxBH1750; 

hw_timer_t *My_timer1 = NULL;

int licznikTacho1,licznikTacho2;
bool LED_STATE = false;
//bool  KOMP_STATE =false;
int TouchNormalTreshold[4];  // zapamietany poziom czczułosci przycisków

Timers <20> akcja;// max 20 wątków




void zadanie0();
void zadanie1();
void zadanie2();
void zadanie3();

//void ServisSerialEvent();
void displaySensorDetails(void);
void displayDataRate(void);
void displayRange(void);

//String processor(const String& var)








#endif