#ifndef task1_h
#define task1_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
//#include "definicje.h"
#include "struktury_def.h"
//#include <main.h>

#define CYKL_TASK1 1000 // cykl działania co ile ms

//static TEMP_DHT CzujnikTempDHT;
 Adafruit_ADXL345_Unified *accel = new Adafruit_ADXL345_Unified(12345);
 //bool  accelOK =false;

 
void displaySensorDetails(void);
void displayDataRate(void);
void displayRange(void);

/// TASK  obsługa cykliczna co cykl_tasku axelerometr jako czujnika wstrząsu
//--------------------------------------------------------------
void task1(void *parameters)
{
   uint8_t regr;

   // Inicjalizacja ADXL345
  Serial.println("Inicjalizacja ADXL345:  ");
  if(!accel->begin(0x1D)) //0x1D dla ADXL wbudowanego na płytę  a dla begin() - axelerometr na zewnętrznej płytce
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    Alarm.connect=0;
  }
  else Alarm.connect=1;
  /* Set the range to whatever is appropriate for your project */
  accel->setRange(ADXL345_RANGE_16_G);

  /* Display some basic information on this sensor */
  displaySensorDetails();
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  //displayRange();
  Serial.println("");

    accel->writeRegister(0x1D,180);  // Threshold Tap   Czułość         1- 62,5mg/LSB   255   - 16gram
    accel->writeRegister(0x21,10);   // Duration (szerokość impulsu)    625us/LSB    100*625us  = 62ms
    accel->writeRegister(0x22,100);  // Latency (czas stabilizacji) 1.25ms/LSB 
    accel->writeRegister(0x23,100);  // Time Windows for second Tap (czas pomiedzy stuknięciami)  1.25ms/LSB
    accel->writeRegister(0x2A,0x07); // 0,0,0,0, Suppress, TAP_X enable, TAP_Y enable, TAP_Z enable
    accel->writeRegister(0x2E,0x60); // ENABLE TAP INterrupts
    //accelOK =true;

     //Serial.println(xPortGetCoreID());

  for(;;)
  {
     if (Alarm.connect) 
      {regr=accel->readRegister(0x30);
       if(regr==0xC3||regr==0xE3)  Alarm.Alarm=true;
       else Alarm.Alarm=false; 
      } 

      //Serial.print("task1  Core: ");  Serial.println(xPortGetCoreID());
      //Serial.print("Remaining Stack Memory: "); Serial.println(uxTaskGetStackHighWaterMark(NULL));
      //Serial.print("Free heap  Memory: "); Serial.println(xPortGetFreeHeapSize());
   
    vTaskDelay(CYKL_TASK1/portTICK_PERIOD_MS);
   
  }
}
//----------------------------------------------------------

//----------------------------
void displaySensorDetails(void)
{
  sensor_t sensor;
  accel->getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2"); 
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
//---------------------------
void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
 
  switch(accel->getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  } 
  Serial.println(" Hz"); 
}

void displayRange(void)
{
  Serial.print ("Range:         +/- ");
 
  switch(accel->getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  } 
  Serial.println(" g"); 
}


#endif