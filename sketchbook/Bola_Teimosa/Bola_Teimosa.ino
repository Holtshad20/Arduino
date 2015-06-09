/*
  Bola Teimosa
  
  Projeto usando Chave Ótica e Servo Motor.
  

*/
#include <Servo.h>

volatile int state1 = HIGH;
volatile int state2 = HIGH;

long CONTA1 = 0;
long CONTA2 = 0;

String msg;

Servo Servo1;

void setup() {
  Serial.begin(9600);
  
  Servo1.attach(9);
  Servo1.write(90);
  delay(1000);
  
  attachInterrupt(0, blink1, FALLING);
  //attachInterrupt(1, blink2, RISING);
  
}

void loop() { 
  msg = "";
  
  if (state1 == LOW) {
      CONTA1+=1;
      state1 = HIGH;
      msg = String(CONTA1) + " - " + String(CONTA2);
      Serial.println(msg); 
      delay(300);
      Servo1.write(0);
      delay(300);
      Servo1.write(90);           

  }
  
  if (state2 == LOW) {
      CONTA2+=1;
      state2 = HIGH;
      msg = String(CONTA1) + " - " + String(CONTA2);
      Serial.println(msg);
  }
  
      
}


void blink1() {
  state1 = LOW;
}

void blink2() {
  state2 = LOW;
}

