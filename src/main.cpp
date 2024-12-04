#include <Arduino.h>
#include <main.h>



TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;




//---------------------------------------------------



void setup() 
{
   Serial.begin(921600);
   //Serial.begin(115200);
   Serial.println("Hello ESPDev Board v1.0");

  //msg_queue = xQueueCreate(msg_queue_len, sizeof(int)); // stworzyliśmy kolejkę
 
  



  //xTaskCreate(task1,"TASK 1", 2000,NULL,1,&Task1); // on core 0
  xTaskCreatePinnedToCore(task1,"TASK1",2000,NULL,1,&Task1,0); //Task on Core 0      accelerometr ADXL
  xTaskCreatePinnedToCore(task2,"TASK2",10000,NULL,4,&Task2,0); //Task on Core 0      DHT22
  xTaskCreatePinnedToCore(task3,"TASK3",10000,NULL,2,&Task3,0); //Task on Core 0   - czujniki temperatur DS1820
  xTaskCreatePinnedToCore(task4,"TASK4",10000,NULL,2,&Task4,0); //Task on Core 0  // BH1750
  //xTaskCreate(task4,"TASK 4", 2000,NULL,3,NULL);                                       // 
  

  akcja.attach(0,250,zadanie0);            // zadania na core1
  akcja.attach(1,3000,zadanie1);         
  akcja.attach(2,0,zadanie2);          
  akcja.attach(3,0,zadanie3);         
 //----------------
  pinMode(TOUCH4_pin, INPUT_PULLUP); pinMode(TOUCH3_pin, INPUT_PULLUP); pinMode(TOUCH2_pin, INPUT_PULLUP); pinMode(TOUCH1_pin, INPUT_PULLUP);
  
  pinMode(LED1_pin, OUTPUT); pinMode(LED2_pin, OUTPUT); pinMode(LED3_pin, OUTPUT); pinMode(LED4_pin, OUTPUT);
  pinMode(KOMP_pin, OUTPUT);
  pinMode(PK1_pin, OUTPUT); pinMode(PK2_pin, OUTPUT);pinMode(PK3_pin, OUTPUT);pinMode(PK4_pin, OUTPUT);
  pinMode(DOOR1_pin, INPUT_PULLUP); pinMode(DOOR2_pin, INPUT_PULLUP); 
 
 // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  webConnect();
  delay(1000);
  TouchNormalTreshold[0]=touchRead(TOUCH1_pin); TouchNormalTreshold[1]=touchRead(TOUCH2_pin);TouchNormalTreshold[2]=touchRead(TOUCH3_pin);TouchNormalTreshold[3]=touchRead(TOUCH4_pin);
   Serial.print("Touch reference Level:  "); Serial.print( TouchNormalTreshold[0]);  Serial.print(" / "); Serial.print(TouchNormalTreshold[1]); Serial.print(" / ");
   Serial.print( TouchNormalTreshold[2]);  Serial.print(" / "); Serial.println(TouchNormalTreshold[3]);


// zmiana na branchu webowa 
    Serial.print( TouchNormalTreshold[2]);  Serial.print(" / "); Serial.println(TouchNormalTreshold[3]);





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


   //delay(1000);
  // put your main code here, to run repeatedly:
 
}
//#######################################################################

// odświerzamy stany na WY i   WE 
void zadanie0()
  { 
    if(touchRead(TOUCH1_pin)<(TouchNormalTreshold[0]-8)) KomponentSTATE.PrzyciskiLED[0]=true; else KomponentSTATE.PrzyciskiLED[0]=false;
    if(touchRead(TOUCH2_pin)<(TouchNormalTreshold[1]-8)) KomponentSTATE.PrzyciskiLED[1]=true; else KomponentSTATE.PrzyciskiLED[1]=false;
    if(touchRead(TOUCH3_pin)<(TouchNormalTreshold[2]-8)) KomponentSTATE.PrzyciskiLED[2]=true; else KomponentSTATE.PrzyciskiLED[2]=false;
    if(touchRead(TOUCH4_pin)<(TouchNormalTreshold[3]-8)) KomponentSTATE.PrzyciskiLED[3]=true; else KomponentSTATE.PrzyciskiLED[3]=false;
    

    if(KomponentSTATE.pk[0]) {digitalWrite(PK1_pin,HIGH);} else {digitalWrite(PK1_pin,LOW);} // WY Przekaźnika 1
    if(KomponentSTATE.pk[1]) {digitalWrite(PK2_pin,HIGH);} else {digitalWrite(PK2_pin,LOW);} // WY Przekaźnika 2
    if(KomponentSTATE.pk[2]) {digitalWrite(PK3_pin,HIGH);} else {digitalWrite(PK3_pin,LOW);} // WY Przekaźnika 3
    if(KomponentSTATE.pk[3]) {digitalWrite(PK4_pin,HIGH);} else {digitalWrite(PK4_pin,LOW);} // WY Przekaźnika 4
    
    if(digitalRead(DOOR1_pin)) KomponentSTATE.drzwi[0]=false; else KomponentSTATE.drzwi[0]=true;
    if(digitalRead(DOOR2_pin)) KomponentSTATE.drzwi[1]=false; else KomponentSTATE.drzwi[1]=true;

    if(digitalRead(TOUCH1_pin)) {KomponentSTATE.przyciski[0]=false;} else {KomponentSTATE.przyciski[0]=true;}
    if(digitalRead(TOUCH2_pin)) {KomponentSTATE.przyciski[1]=false;} else {KomponentSTATE.przyciski[1]=true;}
    if(digitalRead(TOUCH3_pin)) {KomponentSTATE.przyciski[2]=false;} else {KomponentSTATE.przyciski[2]=true;}
    if(digitalRead(TOUCH4_pin)) {KomponentSTATE.przyciski[3]=false;} else {KomponentSTATE.przyciski[3]=true;}

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



