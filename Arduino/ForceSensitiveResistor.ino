void setup(){
  Serial.begin(9600);
}

void loop(){
  int FSR_Toe = 0;
  int FSR_Heel = 0;
  FSR_Heel = analogRead(A0);
  FSR_Toe = analogRead(A2);
  

  
  int threshold = 300;

  if (FSR_Heel > threshold){
    FSR_Toe = 0;
    Serial.print("FSR_Heel: ");
    Serial.println(FSR_Heel);
    FSR_Heel = 0;
    FSR_Toe = 0;
    
    delay(500);
   
  }


  if (FSR_Toe > threshold){
    FSR_Heel = 0;
    Serial.print("FSR_Toe: ");
    Serial.println(FSR_Toe);
    
    FSR_Toe = 0;
    FSR_Heel = 0;
    delay(500);
  }
  

}
