const int ledpin = 13;
const int btnpin = 15;

void setup() {
  pinMode(ledpin, OUTPUT);  // Set PIR as input
  pinMode(btnpin,INPUT_PULLUP);// Set button pin as input with pull-up
  digitalWrite(ledpin,LOW);
  
}

void loop() {
   int btnstate = digitalRead(btnpin);
   if (btnstate == LOW) {
    digitalWrite(ledpin, HIGH); // LED ON
  } else {
    digitalWrite(ledpin, LOW);  // LED OFF
  }
}