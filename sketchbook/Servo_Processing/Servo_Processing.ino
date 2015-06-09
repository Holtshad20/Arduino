#include <Servo.h>

Servo servo1; 
Servo servo2; 


void setup() {

  pinMode(1,OUTPUT);
  servo1.attach(14); //analog pin 0
  //servo1.setMaximumPulse(2000);
  //servo1.setMinimumPulse(700);

  servo2.attach(15); //analog pin 1
  Serial.begin(19200);
  Serial.println("Ready");

}

void loop() {

  static int v = 0;

  if ( Serial.available()) {
    char ch = Serial.read();

    switch(ch) {
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
      case 's':
        servo1.write(v);
        v = 0;
        break;
      case 'w':
        servo2.write(v);
        v = 0;
        break;
      case 'd':
        servo2.detach();
        break;
      case 'a':
        servo2.attach(15);
        break;
    }
  }

} 
