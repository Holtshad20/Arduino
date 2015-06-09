// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

#define PASSOS 200

// Teste com motor Epson EM-323, 200 passos, 1,8º
// to motor port #2 (M3 and M4)
AF_Stepper motor(200, 2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  motor.setSpeed(10);  // 10 rpm   
}

void loop() {
  Serial.println("Single coil steps");
  motor.step(PASSOS, FORWARD, SINGLE); 
  motor.step(PASSOS, BACKWARD, SINGLE); 

  delay(1000);

  Serial.println("Double coil steps");
  motor.step(PASSOS, FORWARD, DOUBLE); 
  motor.step(PASSOS, BACKWARD, DOUBLE);

  delay(1000);

  Serial.println("Interleave coil steps");
  motor.step(PASSOS, FORWARD, INTERLEAVE); 
  motor.step(PASSOS, BACKWARD, INTERLEAVE); 

  delay(1000);

  Serial.println("Micrsostep steps");
  motor.step(PASSOS, FORWARD, MICROSTEP); 
  motor.step(PASSOS, BACKWARD, MICROSTEP); 
}
