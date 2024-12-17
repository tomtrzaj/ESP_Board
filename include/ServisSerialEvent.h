#ifndef ServiSerialEvent_h
#define ServisSerialEvent_h


//#include <main.h>

#include <Arduino.h>
#include <struktury_def.h>
#include <definicje.h>
#include <ArduinoJson.h>



#define SERIAL_BUF_MAX_LENGTH 600 // max rozmiar bufora odbiornika serwisowego
#define SERIAL_BUF_CRITICAL_LENGTH 400  //  krytyczny rozmiar po którym czyścimy dane serial_buf_length=0;

int serial_buf_length;
char serial_buf[SERIAL_BUF_MAX_LENGTH-1];

TEMP_DHT *TempDHT_ptr1 = &Temp_DHT;
TEMP_DS1820 *Temp_DS_ptr1 =&Temp_DS1820;
ALARM *Alarm_ptr1 = &Alarm;
MSG_LUX *BH1750_ptr1 =&LuxBH1750; 

JsonDocument JSON_doc;
    


void EkranStartowyInterfejsSerwisowy();
void SerwisMonitor(void);


//-------------------------------------------------------------------
// funkcja odpowiedzialna za obsługę termina serwisowego Obsługa koment 1- 2- 3- bajtowych i ENTER
//-------------------------------------------------------------------
void ServisSerialEvent()
{
  if (serial_buf_length>SERIAL_BUF_CRITICAL_LENGTH)  serial_buf_length=0;  // czyscimy jeśli coś nam zaśmieciło // zebezpieczenie przed crytycznym resetem urzadzenia
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
        case 'L': 
           switch (serial_buf[1])
           {
            case '1': if(serial_buf[2]=='#') {if(serial_buf[3]=='1') KomponentSTATE.pk[1]=true;  if(serial_buf[3]=='0') KomponentSTATE.pk[1]=false; } break; //L1#1, L1#0  ENTER    załącz lub wyłącz zasilanie panelu LED1
            case '2': if(serial_buf[2]=='#') {if(serial_buf[3]=='1') KomponentSTATE.pk[2]=true;  if(serial_buf[3]=='0') KomponentSTATE.pk[2]=false; } break; //L2#1, L2#0   ENTER  załącz lub wyłącz zasilanie panelu LED1break;
            default : break;
           } break;
        case 'F': 
           switch (serial_buf[1])
           {
            case 'a': if(serial_buf[2]=='#') {if(serial_buf[3]=='1') {KomponentSTATE.fan_zas=true; Serial.println("WENTYLATORY - ON");}  
                                              if(serial_buf[3]=='0') {KomponentSTATE.fan_zas=false;Serial.println("WENTYLATORY - OFF");} } break; //Fa#1, Fa#0  ENTER    załącz lub wyłącz zasilanie wentylatorów
            case '2': break;
            default : break;
           } break;
      
        case '$': 
            switch (serial_buf[1])
            {
            case 's': if(serial_buf[2]=='#') {if(serial_buf[3]=='1') {PodgladSerwisowy=true;Serial.println("Serwis Monitorowy - ON");}
                                              if(serial_buf[3]=='0') {PodgladSerwisowy=false;Serial.println("Serwis Monitorowy - OFF");}} break; 
            case '2': break;
            default : break;
            } break;  
        break;
        case 'd':   break;
        default:    break;
        }
        serial_buf_length = 0;
        return;
      }
      //----------------------komendy 3 bajtowe
      if (serial_buf_length == 3)
      {
        serial_buf[serial_buf_length] = '\0';

        if(!PodgladSerwisowy) {serial_buf_length = 0;break;}  // nieanalizuj 1-bajtowych komend w trybie nie serwisowym  kiedy wysyłamy JSONy
        
        switch (serial_buf[0])
        {
        case 'p':  if(serial_buf[1]=='k') // przekaźniki 'pk1' 'pk2' 'pk3' 'pk4'
                    {
                      switch (serial_buf[2])
                      {
                       case '1': if(KomponentSTATE.pk[0]) {KomponentSTATE.pk[0]=false; Serial.println("PK1 - OFF");}
                                 else                     {KomponentSTATE.pk[0]=true;  Serial.println("PK1 - ON");} break;
                       case '2': if(KomponentSTATE.pk[1]) {KomponentSTATE.pk[1]=false; Serial.println("PK2 - OFF");}
                                 else                     {KomponentSTATE.pk[1]=true;  Serial.println("PK2 - ON");} break;
                       case '3': if(KomponentSTATE.pk[2]) {KomponentSTATE.pk[2]=false; Serial.println("PK3 - OFF");}
                                 else                     {KomponentSTATE.pk[2]=true;  Serial.println("PK3 - ON");} break;
                       case '4': if(KomponentSTATE.pk[3]) {KomponentSTATE.pk[3]=false; Serial.println("PK4 - OFF");}
                                 else                     {KomponentSTATE.pk[3]=true;  Serial.println("PK4 - ON");} break;
                       default: Serial.print("nieznana komenda!!!");break;
                      } 
   
                    } break;
        
        case 'b':  break;
        case 'c':     break;
        case 'd':      break;
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

        if(!PodgladSerwisowy) {serial_buf_length = 0;break;}  // nieanalizuj 1-bajtowych komend w trybie nie serwisowym  kiedy wysyłamy JSONy

        switch (serial_buf[0])
        {
        case 'a':    break;
        case 'b':     break;
        case 'c':     break;
        case 'd':    break;
        case 'l': 
                  switch (serial_buf[1])
                      {
                       case '1': if(KomponentSTATE.PrzyciskiLED[0]) {KomponentSTATE.PrzyciskiLED[0]=false; Serial.println("Przycisk 1 LED -OFF");}
                                 else                               {KomponentSTATE.PrzyciskiLED[0]=true;  Serial.println("Przycisk 1 LED -ON");}   break;
                       case '2': if(KomponentSTATE.PrzyciskiLED[1]) {KomponentSTATE.PrzyciskiLED[1]=false; Serial.println("Przycisk 2 LED -OFF");}
                                 else                               {KomponentSTATE.PrzyciskiLED[1]=true;  Serial.println("Przycisk 2 LED -ON");}   break;          
                       case '3': if(KomponentSTATE.PrzyciskiLED[2]) {KomponentSTATE.PrzyciskiLED[2]=false; Serial.println("Przycisk 1 LED -OFF");}
                                 else                               {KomponentSTATE.PrzyciskiLED[2]=true;  Serial.println("Przycisk 3 LED -ON");}   break;
                       case '4': if(KomponentSTATE.PrzyciskiLED[3]) {KomponentSTATE.PrzyciskiLED[3]=false; Serial.println("Przycisk 4 LED -OFF");}
                                 else                               {KomponentSTATE.PrzyciskiLED[3]=true;  Serial.println("Przycisk 4 LED -ON");}   break;          
                       default: Serial.print("nieznana komenda!!!");break;
                      }      
          break;
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
        
        if(!PodgladSerwisowy) {serial_buf_length = 0;break;}  // nieanalizuj 1-bajtowych komend w trybie nie serwisowym  kiedy wysyłamy JSONy
        //--przetwarzanie otrzymanej komendy 1 bajtowej
        switch (serial_buf[0])
        {
        case 'A':
          break;
        case 'a':    Serial.print("komp ON");  digitalWrite(PWN_5V_pin,HIGH); break;
        case 'b':    Serial.print("komp OFF"); digitalWrite(PWN_5V_pin,LOW);  break;
        case 'c':    break;
        case 'd':    break;
        case 'f':  
                   if(KomponentSTATE.fan_zas) {KomponentSTATE.fan_zas=false; Serial.println("WENTYLATORY - OFF");}  
                   else{KomponentSTATE.fan_zas=true;  Serial.println("WENTYLATORY - ON");}      break;
        
        case 's':  if(PodgladSerwisowy) {PodgladSerwisowy=false;Serial.println("Serwis Monitorowy - OFF");} else {PodgladSerwisowy=true;Serial.println("Serwis Monitorowy - ON");} break;
        case 'm':  break;
        case '0':  break;
        case '1':  PWN_Fan1+=20; analogWrite(PWN_FAN1_pin,PWN_Fan1); Serial.print("FAN 1-  ");   Serial.print(PWN_Fan1);   break;
        case '2':  PWN_Fan1-=20; analogWrite(PWN_FAN1_pin,PWN_Fan1); Serial.print("FAN 1-  ");   Serial.print(PWN_Fan1);   break;
        case '3':  PWN_Fan2+=20; analogWrite(PWN_FAN2_pin,PWN_Fan2); Serial.print("FAN 2-  ");   Serial.print(PWN_Fan2);   break;
        case '4':  PWN_Fan2-=20; analogWrite(PWN_FAN2_pin,PWN_Fan2); Serial.print("FAN 2-  ");   Serial.print(PWN_Fan2);   break;
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
  Serial.println(F(" ST_ESP32 Board soft_ver 1.1   @2024 by KZŁ Bydgoszcz Sp. z o.o."));
 
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
  //Serial.println(F("axxx ENTER - Ustawienie PWN monitora 1:     [gdzie: xxx - 1 do 255]"));
  //Serial.println(F("bxxx ENTER - Ustawienie PWN monitora 2:     [gdzie: xxx - 1 do 255]"));
  //Serial.println(F("cxxx ENTER - Ustawienie PWN wyjscia 1:      [gdzie: xxx - 1 do 255]"));
  //Serial.println(F("dxxx ENTER - Ustawienie PWN wyjscia 2:      [gdzie: xxx - 1 do 255]"));
  // Serial.println(F("p    ENTER - Zał/Wył Czujnika kurzu"));
  // Serial.println(F("1..4 ENTER - Zał/Wył Przekaźnik 1..4"));
  Serial.println(F("---------------------------------------------------------------"));
}
//---------------------------------------------------
void SerwisMonitor(void)
{
  if(PodgladSerwisowy)   // nie wyświetlaj jeśli jest zablokowany
  {
   char Text[200];
 
   snprintf(Text,sizeof(Text)," [T:%.1f H:%.0i] DS1820:[%.1f/%.1f/%.1f/%.1f] LUX:[%.0f/%.0f] AL:[%i] DRZWI:[%i] GRZ:[%i] PK:[%i/%i/%i/%i] TACHO:[%i/%i obr/min]",
            TempDHT_ptr1->temp,                 // DHT1 temeratura 1
            TempDHT_ptr1->hum,                  // DHT1 wilgotność
            Temp_DS_ptr1->temp[0],Temp_DS_ptr1->temp[1],Temp_DS_ptr1->temp[2],Temp_DS_ptr1->temp[3],
            BH1750_ptr1->lux[0],BH1750_ptr1->lux[1],
            Alarm_ptr1->Alarm,
            (int)KomponentSTATE.drzwi,
            (int)KomponentSTATE.grzalka,
            (int)KomponentSTATE.pk[0],
            (int)KomponentSTATE.pk[1],
            (int)KomponentSTATE.pk[2],
            (int)KomponentSTATE.pk[3],
            KomponentSTATE.Tacho[0],
            KomponentSTATE.Tacho[1]
            
            );
   Serial.print(stany_string[STAN_PRACY]);
   Serial.println(Text);
  }
  else   // przesyłamy w JONSON
  {
    JSON_doc["STAN"] = stany_string[STAN_PRACY];
    JSON_doc["TEMP"] = TempDHT_ptr1->temp;
    JSON_doc["HUMIDITY"] = TempDHT_ptr1->hum;
    for(int i=0;i<4;i++) JSON_doc["DS1820"][i] =Temp_DS_ptr1->temp[i];
    JSON_doc["LED_POW"][0] = KomponentSTATE.pk[1];  // status zasilania na matrycę LED 1
    JSON_doc["LED_POW"][1] = KomponentSTATE.pk[2];  // status zasilania na matrycę LED 2

    //for(int i=0;i<4;i++) JSON_doc["PK"][i] =KomponentSTATE.pk[i];
    JSON_doc["LUX"][0] = (int)BH1750_ptr1->lux[0];
    JSON_doc["LUX"][1] = (int)BH1750_ptr1->lux[1];
    JSON_doc["DOOR"] = KomponentSTATE.drzwi;
    JSON_doc["HEAT"] = KomponentSTATE.grzalka;
    JSON_doc["SHOCK"] = Alarm_ptr1->Alarm;

    JSON_doc["TACHO"][0] = KomponentSTATE.Tacho[0];
    JSON_doc["TACHO"][1] = KomponentSTATE.Tacho[1];
    
      serializeJson(JSON_doc, Serial);
      Serial.println();
      //serializeJsonPretty(JSON_doc, Serial);
  } 

}

#endif