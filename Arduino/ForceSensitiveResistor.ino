void setup(){
  Serial.begin(9600);
}

void loop(){
  int FSR_Heel = analogRead(A0);
  int FSR_Toe = analogRead(A1);

  if ((FSR_Heel > 100) || (FSR_Toe > 100)){
    Serial.println(FSR_Toe);
    Serial.println(FSR_Heel);
    
    delay(250);
  }
}
