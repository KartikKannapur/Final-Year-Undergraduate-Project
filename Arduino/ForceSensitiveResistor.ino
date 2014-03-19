int FSR_Pin = A0; //analog pin 0

void setup(){
  Serial.begin(9600);
}

void loop(){
  int FSRReading = analogRead(FSR_Pin); 
if(FSRReading != 0){
  Serial.println();
  Serial.println();
Serial.println("Arduino");
Serial.println();
Serial.println();
Serial.println();
Serial.println();

  delay(250); 
}//just here to slow down the output for easier reading
}
