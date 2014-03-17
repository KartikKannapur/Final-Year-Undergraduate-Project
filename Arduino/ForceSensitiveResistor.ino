void setup(){
  Serial.begin(9600);
}

void loop(){
  int FSR_Heel = analogRead(A0);
  int FSR_Toe = analogRead(A1);
  
  int threshold = 200;

  if ((FSR_Heel > threshold) || (FSR_Toe > threshold)){
    Serial.print("FSR_Toe: ");
    Serial.println(FSR_Toe);
    
    Serial.print("FSR_Heel: ");
    Serial.println(FSR_Heel);
    
    delay(250);
  }
}
