// Adafruit Motor shield library
// Teste com motor de passo de DVD

#include <AFMotor.h>

#define PASSOS 250
#define SPEED 300

// 48 passos, 7,5°
AF_Stepper motor1(48, 1); // port #1 (M1 e M2)
AF_Stepper motor2(48, 2); // port #2 (M3 e M4)

void setup() {
  Serial.begin(9600); 
  Serial.println("Stepper test!");

  motor1.setSpeed(SPEED);
  motor2.setSpeed(SPEED);   
}

void loop() {
  Serial.println("Single coil steps");
  motor1.step(PASSOS, FORWARD, SINGLE); 
  motor2.step(PASSOS, FORWARD, SINGLE); 
  motor1.step(PASSOS, BACKWARD, SINGLE); 
  motor2.step(PASSOS, BACKWARD, SINGLE); 

  delay(300);

}
