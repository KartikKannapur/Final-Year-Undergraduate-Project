void setup(){
  Serial.begin(9600);
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
  FSR_R = analogRead(A0);
  
  int threshold = 300;

  if (FSR_R > threshold){
   temp= millis();
    if((800)<=temp-R)   //debouncing for 0.8 sec
   {
   // Serial.print("FSR_Heel: ");
   R=millis();
    Serial.print("1");
   }
   delay(10);
  }
  
FSR_L = analogRead(A2);
  if (FSR_L> threshold){
    temp=millis();
    if((800)<=temp-L)  //debouncing for 0.8 sec
   {
   L=millis();
   // Serial.print("FSR_Toe: ");
    Serial.print("2");
    delay(10);
  }
  
}
}
}
