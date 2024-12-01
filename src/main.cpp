#include <Arduino.h>
#include <main.h>



TaskHandle_t Task1;
TaskHandle_t Task2;



//---------------------------------------------------



void setup() 
{
   Serial.begin(921600);
   //Serial.begin(115200);
   Serial.println("Hello ESPDev Board v1.0");

  //msg_queue = xQueueCreate(msg_queue_len, sizeof(int)); // stworzyliśmy kolejkę
 
  



  //xTaskCreate(task1,"TASK 1", 2000,NULL,1,&Task1); // on core 0
  xTaskCreatePinnedToCore(task1,"TASK1",2000,NULL,0,&Task1,0); //Task on Core 0
  xTaskCreate(task2,"TASK 2", 2000,NULL,2,&Task2);                                     // Task 2 - czujnik temperatury i wilgoci DHT
  xTaskCreate(task3,"TASK 3", 2000,NULL,4,NULL);                                       // Task 3 - czujniki temperatur DS1820
  xTaskCreate(task4,"TASK 4", 2000,NULL,3,NULL);                                       // Task 3 - czujniki temperatur DS1820
  

  akcja.attach(0,1000,zadanie0);            // zadania na core1
  akcja.attach(1,3000,zadanie1);         
  akcja.attach(2,0,zadanie2);          
  akcja.attach(3,0,zadanie3);         
 //----------------
  pinMode(TOUCH4_pin, INPUT_PULLUP);
  pinMode(TOUCH3_pin, INPUT_PULLUP);
  pinMode(TOUCH2_pin, INPUT_PULLUP);
  pinMode(TOUCH1_pin, INPUT_PULLUP);

 
 // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  webConnect();



  
}


//################################################################
//##############   LOOP   #######################################
//################################################################
void loop() 
{
  akcja.process(); // wątki
  ServisSerialEvent(); // obsługa koment z zewnątrz 

   //Serial.print("Loop  Core: ");  Serial.println(xPortGetCoreID());
   //Serial.print("Remaining Stack Memory: "); Serial.print(uxTaskGetStackHighWaterMark(NULL));
   //Serial.print("  Free heap  Memory: "); Serial.println(xPortGetFreeHeapSize());


   //delay(1000);
  // put your main code here, to run repeatedly:
 
}
//#######################################################################

// put function definitions here:
void zadanie0()
  { 
    //TODO
    //if(LED_STATE) {digitalWrite(LED,LOW); LED_STATE=false;}
    //else {digitalWrite(LED,HIGH); LED_STATE=true;}

    Serial.print("Touch: "); Serial.print(touchRead(TOUCH1_pin));
    Serial.print(" / "); Serial.print(touchRead(TOUCH2_pin));
    Serial.print(" / "); Serial.print(touchRead(TOUCH3_pin));
    Serial.print(" / "); Serial.println(touchRead(TOUCH4_pin));
    
  }
//-----  
void zadanie1()
  { 
    SerwisMonitor();
    
      
    
  } 
//-----  
void zadanie2()
  { 
    

  }   
 //-----  
void zadanie3()
  { 
  } 



