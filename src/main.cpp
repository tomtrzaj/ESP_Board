#include <Arduino.h>
#include <main.h>



TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;




//---------------------------------------------------
//Przerwanie od TAHO wentylatorów
void IRAM_ATTR zliczTacho1() { licznikTacho1++; }
void IRAM_ATTR zliczTacho2() { licznikTacho2++; }
//-------------------------------------------------
// zlicza co 1s obroty wentylatorów TACHO
void IRAM_ATTR onTimer1()
{
  StaneTime++;   // zliczamy czas trwania STANu w [s] 
  KomponentSTATE.Tacho[0]=licznikTacho1*30;  // Tacho dają po dwa impulsy na [s] czyli /2  mnożymy *60  co w sumie daje *30  
  licznikTacho1=0;
  KomponentSTATE.Tacho[1]=licznikTacho2*30;  // Tacho dają po dwa impulsy na [s] czyli /2  mnożymy *60  co w sumie daje *30  
  licznikTacho2=0;
}
//------------------------------------------


void setup() 
{
   //Serial.begin(921600);
   Serial.begin(115200);
   Serial.println("Hello ESPDev Board v1.0");

  //msg_queue = xQueueCreate(msg_queue_len, sizeof(int)); // stworzyliśmy kolejkę


  //xTaskCreate(task1,"TASK 1", 2000,NULL,1,&Task1); // on core 0
  xTaskCreatePinnedToCore(task1,"TASK1",2000,NULL,1,&Task1,0); //Task on Core 0      accelerometr ADXL
  xTaskCreatePinnedToCore(task2,"TASK2",10000,NULL,4,&Task2,0); //Task on Core 0      DHT22
  xTaskCreatePinnedToCore(task3,"TASK3",10000,NULL,3,&Task3,0); //Task on Core 0   - czujniki temperatur DS1820
  xTaskCreatePinnedToCore(task4,"TASK4",10000,NULL,1,&Task4,0); //Task on Core 0  // BH1750
  

  akcja.attach(0,250,zadanie0);            // zadania na core1
  akcja.attach(1,3000,zadanie1);         
  akcja.attach(2,0,zadanie2);              
  akcja.attach(3,0,zadanie3);         
 //----------------
  //pinMode(TOUCH4_pin, INPUT_PULLUP); pinMode(TOUCH3_pin, INPUT_PULLUP); pinMode(TOUCH2_pin, INPUT_PULLUP); pinMode(TOUCH1_pin, INPUT_PULLUP);
 
  pinMode(LED1_pin, OUTPUT); pinMode(LED2_pin, OUTPUT); pinMode(LED3_pin, OUTPUT); pinMode(LED4_pin, OUTPUT);
  pinMode(KOMP_pin, OUTPUT);   
  pinMode(PWN_5V_pin, OUTPUT); digitalWrite(PWN_5V_pin,LOW);
  pinMode(PK1_pin, OUTPUT); pinMode(PK2_pin, OUTPUT);pinMode(PK3_pin, OUTPUT);pinMode(PK4_pin, OUTPUT);
  pinMode(DOOR1_pin, INPUT_PULLUP); pinMode(DOOR2_pin, INPUT_PULLUP); 
  pinMode(PWN_FAN1_pin, OUTPUT);  pinMode(PWN_FAN2_pin, OUTPUT);
  pinMode(TACHO1_pin, INPUT_PULLUP); pinMode(TACHO2_pin, INPUT_PULLUP);
  //analogWriteFrequency(25000);
  analogWrite(PWN_FAN1_pin,255);  analogWrite(PWN_FAN2_pin,255);
    
 // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  
  //webConnect();
  //delay(1000);
  //TouchNormalTreshold[0]=touchRead(TOUCH1_pin); TouchNormalTreshold[1]=touchRead(TOUCH2_pin);TouchNormalTreshold[2]=touchRead(TOUCH3_pin);TouchNormalTreshold[3]=touchRead(TOUCH4_pin);
  // Serial.print("Touch reference Level:  "); Serial.print( TouchNormalTreshold[0]);  Serial.print(" / "); Serial.print(TouchNormalTreshold[1]); Serial.print(" / ");
  // Serial.print( TouchNormalTreshold[2]);  Serial.print(" / "); Serial.println(TouchNormalTreshold[3]);
  
  // do TACHO uruchamiamy precyzyjny TIMER
    My_timer1 = timerBegin(0, 80, true); // przerwanie co sekunde
    timerAttachInterrupt(My_timer1, &onTimer1, true);
    timerAlarmWrite(My_timer1, 1000000, true);
    attachInterrupt(TACHO1_pin, zliczTacho1, FALLING); attachInterrupt(TACHO2_pin, zliczTacho2, FALLING);  // zliczanie impulsów od tacho
    timerAlarmEnable(My_timer1);       // timer do zliczania impulsów w ciągu minuty


}

//################################################################
//##############   LOOP   #######################################
//################################################################
void loop() 
{
  akcja.process(); // wątki
  ServisSerialEvent(); // obsługa koment z zewnętrznego terminala serwisowego 

   //Serial.print("Loop  Core: ");  Serial.println(xPortGetCoreID());
   //Serial.print("Remaining Stack Memory: "); Serial.print(uxTaskGetStackHighWaterMark(NULL));
   //Serial.print("  Free heap  Memory: "); Serial.println(xPortGetFreeHeapSize());
 // -----STANY PRACY  TABLICY --------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------------------
   switch(STAN_PRACY)  
    { //------ Stan po załączeniu tablicy ---------------------------------------------
      case START: 
          switch (StaneTime)
           {
            case 1: {ilosc_wykonan=1;break;}
            case 2: {if(ilosc_wykonan>0) {/* TO DO*/ ilosc_wykonan--;} break;} // po 2s
            case 3: {ilosc_wykonan=2; break;}
            case 6: {if(ilosc_wykonan>0) {/* TO DO*/ ilosc_wykonan--;} break;}
            case 10: { /* TO DO*/ STAN_PRACY=DOGRZEWANIE; StaneTime=0; break;}  // przejście do innego stanu
            default: break;
           }  
          break;
      case DOGRZEWANIE: 
          switch (StaneTime)
           {
            case 1: {ilosc_wykonan=1;break;}
            case 2: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 3: {ilosc_wykonan=3;break;}
            case 6: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 10: {/* TO DO*/  STAN_PRACY=PRACA;StaneTime=0; break;}
            default: break;
           }  
          break;       
                     

      case PRACA: 
         switch (StaneTime)
           {
            case 1: {ilosc_wykonan=1;break;}
            case 2: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 3: {ilosc_wykonan=4;break;}
            case 6: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 10: { /* TO DO*/  STAN_PRACY=WYLACZANIE;StaneTime=0; break;}
            default: break;
           }  
          break;       
      case WYLACZANIE: 
          switch (StaneTime)
           {
            case 1: {ilosc_wykonan=1;break;}
            case 2: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 3: {ilosc_wykonan=5;break;}
            case 6: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 10: { /* TO DO*/  STAN_PRACY=AWARIA;StaneTime=0; break;}
            default: break;
           }  
          break;       
      
      case AWARIA: 
          switch (StaneTime)
           {
            case 1: {ilosc_wykonan=1;break;}
            case 2: { if(ilosc_wykonan>0) {/* TO DO*/  ilosc_wykonan--;} break;}
            case 3: {ilosc_wykonan=6;break;}
            case 6: { if(ilosc_wykonan>0) {/* TO DO*/   ilosc_wykonan--;} break;}
            case 10: { /* TO DO*/  STAN_PRACY=START;StaneTime=0; break;}
            default: break;
           }  
          break;       
      default: break;
    }

 
}
//#######################################################################

// odświerzamy stany na WY i   WE 
void zadanie0()
  { 
    // przyciski są nieużywane w wersji dla tablicy LED
    
    //if(touchRead(TOUCH1_pin)<(TouchNormalTreshold[0]-8)) KomponentSTATE.PrzyciskiLED[0]=true; else KomponentSTATE.PrzyciskiLED[0]=false;
    //if(touchRead(TOUCH2_pin)<(TouchNormalTreshold[1]-8)) KomponentSTATE.PrzyciskiLED[1]=true; else KomponentSTATE.PrzyciskiLED[1]=false;
    //if(touchRead(TOUCH3_pin)<(TouchNormalTreshold[2]-8)) KomponentSTATE.PrzyciskiLED[2]=true; else KomponentSTATE.PrzyciskiLED[2]=false;
    //if(touchRead(TOUCH4_pin)<(TouchNormalTreshold[3]-8)) KomponentSTATE.PrzyciskiLED[3]=true; else KomponentSTATE.PrzyciskiLED[3]=false;
    

    if(KomponentSTATE.pk[0]) {digitalWrite(PK1_pin,HIGH);} else {digitalWrite(PK1_pin,LOW);} // WY Przekaźnika 1
    if(KomponentSTATE.pk[1]) {digitalWrite(PK2_pin,HIGH);} else {digitalWrite(PK2_pin,LOW);} // WY Przekaźnika 2
    if(KomponentSTATE.pk[2]) {digitalWrite(PK3_pin,HIGH);} else {digitalWrite(PK3_pin,LOW);} // WY Przekaźnika 3
    if(KomponentSTATE.pk[3]) {digitalWrite(PK4_pin,HIGH);} else {digitalWrite(PK4_pin,LOW);} // WY Przekaźnika 4
    
    if(digitalRead(DOOR1_pin)) KomponentSTATE.drzwi=false; else KomponentSTATE.drzwi=true;
    if(digitalRead(DOOR2_pin)) KomponentSTATE.grzalka=false; else KomponentSTATE.grzalka=true;

    if(KomponentSTATE.fan_zas) {digitalWrite(KOMP_pin,HIGH);} else {digitalWrite(KOMP_pin,LOW);}  // załączenie zasilania pod wentylatory

    //if(digitalRead(TOUCH1_pin)) {KomponentSTATE.przyciski[0]=false;} else {KomponentSTATE.przyciski[0]=true;}
    //if(digitalRead(TOUCH2_pin)) {KomponentSTATE.przyciski[1]=false;} else {KomponentSTATE.przyciski[1]=true;}
    //if(digitalRead(TOUCH3_pin)) {KomponentSTATE.przyciski[2]=false;} else {KomponentSTATE.przyciski[2]=true;}
    //if(digitalRead(TOUCH4_pin)) {KomponentSTATE.przyciski[3]=false;} else {KomponentSTATE.przyciski[3]=true;}

    if(KomponentSTATE.PrzyciskiLED[0]) {digitalWrite(LED1_pin,HIGH);} else {digitalWrite(LED1_pin,LOW);} // WY podświetlenie przycisków
    if(KomponentSTATE.PrzyciskiLED[1]) {digitalWrite(LED2_pin,HIGH);} else {digitalWrite(LED2_pin,LOW);}
    if(KomponentSTATE.PrzyciskiLED[2]) {digitalWrite(LED3_pin,HIGH);} else {digitalWrite(LED3_pin,LOW);}
    if(KomponentSTATE.PrzyciskiLED[3]) {digitalWrite(LED4_pin,HIGH);} else {digitalWrite(LED4_pin,LOW);}

     //Serial.print(touchRead(TOUCH1_pin));  Serial.print("/");
     //Serial.print(touchRead(TOUCH2_pin));  Serial.print("/");
     //Serial.print(touchRead(TOUCH3_pin));  Serial.print("/");
     //Serial.println(touchRead(TOUCH4_pin));
  }
//-----  

void zadanie1()
  { 
    //Serial.print("Loop  Core: ");  Serial.println(xPortGetCoreID());
    SerwisMonitor();
    digitalWrite(LED1_pin,HIGH);
      
    
  } 
//-----  
void zadanie2()
  { 

  

  }   
 //-----  
void zadanie3()
  { 
  } 



