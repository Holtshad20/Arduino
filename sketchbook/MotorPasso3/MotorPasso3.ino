// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
#include <Servo.h>

#define PASSOS 500

Servo servo1;

// Teste com motor Epson EM-323, 200 passos, 1,8º
AF_Stepper motor1(100, 1);
AF_Stepper motor2(100, 2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  servo1.attach(10);
  servo1.write(0);
  motor1.setSpeed(100);  // 100 rpm   
  motor2.setSpeed(100);
  
}

void loop() {
  Serial.println("Single coil steps");
  motor1.step(PASSOS, FORWARD, SINGLE); 
  motor2.step(PASSOS, FORWARD, SINGLE);
  
  motor1.step(PASSOS, BACKWARD, SINGLE); 
  motor2.step(PASSOS, BACKWARD, SINGLE);
   
  for (int i=0; i<181; i++) { 
    servo1.write(i);   
    delay(10);
  };
   
  delay(1000);
}
