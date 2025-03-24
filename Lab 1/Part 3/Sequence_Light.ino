void setup() {
  pinMode(2,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);

}

void loop() {
   digitalWrite(2,HIGH);
   delay(3000);
   digitalWrite(2,LOW);

   digitalWrite(18,HIGH);
   delay(3000);
   digitalWrite(18,LOW);

   digitalWrite(19,HIGH);
   delay(3000);
   digitalWrite(19,LOW);

}
