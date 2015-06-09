/*
  COntador ótico
  
  Contador ótico de passagem

*/
volatile int state1 = HIGH;

long CONTA1 = 0;

String msg;

void setup() {
  Serial.begin(9600);
  
  attachInterrupt(0, blink1, FALLING);
  
}

void loop() { 
  msg = "";
  
  if (state1 == LOW) {
      CONTA1+=1;
      state1 = HIGH;
      msg = String(CONTA1);
      Serial.println(msg);      
  }      
}


void blink1() {
  state1 = LOW;
}

