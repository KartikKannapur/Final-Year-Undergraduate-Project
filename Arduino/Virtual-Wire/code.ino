#include <VirtualWire.h>

void setup(){
  Serial.begin(9600);
 // vw_setup(2000);  // Bits per sec
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
}

void loop(){
  int FSR_L  = 0;
  int FSR_R = 0;
  long int R=0;
  long int L=0;
  long int temp;
  
  while(1)
  {
  //Right=A0, Left=A2;
  int threshold = 300;
  
   while(1)
   {
     delay(100);
     repeat_1:
   
     while(1)
     {
       int temp_L=0, temp_R=0 ;
       temp_R=analogRead(A0);
       delay(10);
       temp_L=analogRead(A2);
       delay(10);
       if(temp_R > threshold && temp_L > threshold)
       {
         continue;
       }
       
       else if(temp_R > threshold && temp_L < threshold)
       {
         for(int h=0;h<4;h++)
         {
         delay(100);
         temp_L=analogRead(A2);
         if (temp_L>threshold)
         {
           goto repeat_1;
         }
         else if(temp_L<threshold && h==3) // left foot risen for atleast 0.4 sec
           {
             while(1)
             {
              temp_L=analogRead(A2); 
             if (temp_L>threshold)
             {
               goto gait_start;
             }
             }
           }
           
         }
       }
       
   }
   }
   
  gait_start:
   
   while(1)
   {
   FSR_R = analogRead(A0);
  if (FSR_R > threshold){
   temp= millis();
    if((800)<=temp-R)   //debouncing for 0.8 sec
   {
    //Serial.print("FSR_Heel: ");
   R=millis();
    const char *msg = "1";
 vw_send((uint8_t *)msg, strlen(msg));
   }
   delay(700);
  }
  
FSR_L = analogRead(A2);
  if (FSR_L> threshold){
    temp=millis();
    if((800)<=temp-L)  //debouncing for 0.8 sec
      {
     L=millis();
      const char *msg= "2";
 vw_send((uint8_t *)msg, strlen(msg));
      delay(700);
    }
  

  }
   }
}
}
