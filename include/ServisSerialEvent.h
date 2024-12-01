#ifndef ServiSerialEvent_h
#define ServisSerialEvent_h



#include <Arduino.h>
#include <struktury_def.h>


#define SERIAL_BUF_MAX_LENGTH 30 // max rozmiar bufora odbiornika serwisowego

int serial_buf_length;
char serial_buf[SERIAL_BUF_MAX_LENGTH-1];

TEMP_DHT *TempDHT_ptr1 = &Temp_DHT;
TEMP_DS1820 *Temp_DS_ptr1 =&Temp_DS1820;
ALARM *Alarm_ptr1 = &Alarm;
MSG_LUX *BH1750_ptr1 =&LuxBH1750; 

void EkranStartowyInterfejsSerwisowy();
void SerwisMonitor(void);


//-------------------------------------------------------------------
// funkcja odpowiedzialna za obsługę termina serwisowego Obsługa koment 1- 2- 3- bajtowych i ENTER
//-------------------------------------------------------------------
void ServisSerialEvent()
{
  while (Serial.available())
  {
    
    char zn = (char)Serial.read();
    if (zn == '\r' || zn == '\n') // odebrany znak końca lini
    {
      //--------------------komendy 4 bajtowe
      if (serial_buf_length == 4)
      {
        serial_buf[serial_buf_length] = '\0';
        switch (serial_buf[0])
        {
        case 'a': Serial.print("K4 first a");        break;
        case 'b': Serial.print("K4 first b");        break;
        case 'c': Serial.print("K4 first c");        break;
        case 'd': Serial.print("K4 first d ");       break;
        default:
          break;
        }
        serial_buf_length = 0;
        return;
      }
      //----------------------komendy 3 bajtowe
      if (serial_buf_length == 3)
      {
        serial_buf[serial_buf_length] = '\0';
        switch (serial_buf[0])
        {
        case 'a':  Serial.print("K3 first a ");       break;
        case 'b':  Serial.print("K3 first b ");    break;
        case 'c':  Serial.print("K3 first c ");        break;
        case 'd':  Serial.print("K3 first d ");        break;
        default:
          break;
        }
        serial_buf_length = 0;
        return;
      }

      //--------------------komendy 2 bajtowe-----------
      if (serial_buf_length == 2)
      {
        serial_buf[serial_buf_length] = '\0';
        switch (serial_buf[0])
        {
        case 'a': Serial.print("K2 first a");        break;
        case 'b': Serial.print("K2 first b");        break;
        case 'c': Serial.print("K2 first c ");       break;
        case 'd': Serial.print("K2 first d ");       break;
        default:
          break;
        }
        serial_buf_length = 0;
        return;
      }

      //--------------------- komendy 1 bajtowe----------
      if (serial_buf_length > 0)
      {
        serial_buf[serial_buf_length] = '\0';
        //--przetwarzanie otrzymanej komendy 1 bajtowej
        switch (serial_buf[0])
        {
        case 'A':
          break;
        case 'a':  Serial.print("K1 a ");    break;
        case 'b':  Serial.print("K2 b ");     break;
        case 'c':  Serial.print("K3 c ");    break;
        case 'd':  Serial.print("K3 d ");    break;
        
        case 's':  break;
        case 'm':  break;
        case '0':  break;
        case '1':  break;
        case '2':  break;
        case '3':  break;
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        default:
          Serial.println("Nierozpoznana komenda ");
          break;
        }
        serial_buf_length = 0;
      }
      else if (zn == '\r')
        EkranStartowyInterfejsSerwisowy();
    }
    else
    {
      if (serial_buf_length >= SERIAL_BUF_MAX_LENGTH)   serial_buf_length = 0; // przepełnienie bufora
      serial_buf[serial_buf_length++] = zn;
    }
  }
}




//---------------------------------------------------------------------------------
//--- ekran startowy interfejsu serwisowego
void EkranStartowyInterfejsSerwisowy()
{
  // wdt_reset();
  Serial.println(F("---------------------------------------------------------------"));
  Serial.println(F(" ST_ESP32 Board    @2024 by KZŁ Bydgoszcz Sp. z o.o."));
  // Serial.println(Soft_ver);
  Serial.println(F("---------------------------------------------------------------"));
  // Serial.print("ADRES MODUŁU: ");  Serial.print(Adres_CZW); Serial.print("  MQ07 R0 = ");  Serial.println(tabMq7);
  Serial.println(F("---------------------------------------------------------------"));
  // wdt_reset();
  // Serial.println(F("    ENTER - Zał/Wył Tryb podgladu serwisowego danych"));
  // Serial.println(F("r    ENTER - Zał/Wył Tryb podgladu odbiornika RS485"));
  // Serial.println(F("g    ENTER - Zał/Wył czujnika CO2 MQ07"));
  // Serial.println(F("m    ENTER - Odczyt rezystancji referencyjnej R0 czujnika  MQ07"));
  // Serial.println(F("mz   ENTER - Odczyt referencyjnej R0 MQ07 i zapis do EEPROM"));
  // Serial.println(F("ms   ENTER - Podgląd odczytów i stanów z czujnika CO2  MQ07"));
  // wdt_reset();
  Serial.println(F("axxx ENTER - Ustawienie PWN monitora 1:     [gdzie: xxx - 1 do 255]"));
  Serial.println(F("bxxx ENTER - Ustawienie PWN monitora 2:     [gdzie: xxx - 1 do 255]"));
  Serial.println(F("cxxx ENTER - Ustawienie PWN wyjscia 1:      [gdzie: xxx - 1 do 255]"));
  Serial.println(F("dxxx ENTER - Ustawienie PWN wyjscia 2:      [gdzie: xxx - 1 do 255]"));
  // Serial.println(F("p    ENTER - Zał/Wył Czujnika kurzu"));
  // Serial.println(F("1..4 ENTER - Zał/Wył Przekaźnik 1..4"));
  Serial.println(F("---------------------------------------------------------------"));
}
//---------------------------------------------------
void SerwisMonitor(void)
{
  char Text[200];
 

  snprintf(Text,sizeof(Text),"Temp:%.1f H:%.0i DS1820:[%.1f/%.1f/%.1f/%.1f/] LUX: [%.0f/%.0f] ALARM: %i ",
            TempDHT_ptr1->temp,                 // DHT1 temeratura 1
            TempDHT_ptr1->hum,                  // DHT1 wilgotność
            Temp_DS_ptr1->temp[0],Temp_DS_ptr1->temp[1],Temp_DS_ptr1->temp[2],Temp_DS_ptr1->temp[3],
            BH1750_ptr1->lux[0],BH1750_ptr1->lux[1],
            Alarm_ptr1->Alarm
            );

  Serial.println(Text);

}

#endif