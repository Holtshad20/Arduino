/*

Teste para driver motor de passo baseado no L297

pins : 2 - Step
         3 - Dir
         8 - Enable

*/

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepperX(2, 2, 3);


void setup()
{  
    stepperX.setMaxSpeed(800.0);
    stepperX.setAcceleration(600.0);
    stepperX.runToNewPosition(0);
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
}

void loop()
{    
    stepperX.runToNewPosition(100);
    stepperX.runToNewPosition(0);
    stepperX.runToNewPosition(200);
    stepperX.runToNewPosition(0);
    stepperX.runToNewPosition(1200);
    stepperX.runToNewPosition(0);
    stepperX.runToNewPosition(500);
    stepperX.runToNewPosition(0);
    stepperX.runToNewPosition(1500);
    stepperX.runToNewPosition(0);    
}
