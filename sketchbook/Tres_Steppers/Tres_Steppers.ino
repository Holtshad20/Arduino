// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit to another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Defini��o para controle STEP + DIR, DRIVER(1), STEP, DIR
AccelStepper stepper1(1,2,3); 
AccelStepper stepper2(1,4,5); 
AccelStepper stepper3(1,6,7); 

void setup()
{  
  // Change these to suit your stepper if you want
  stepper1.setMaxSpeed(200);
  stepper1.setAcceleration(50);
  stepper1.moveTo(300);

  stepper2.setMaxSpeed(400);
  stepper2.setAcceleration(200);
  stepper2.moveTo(-700);

  stepper3.setMaxSpeed(300);
  stepper3.setAcceleration(250);
  stepper3.moveTo(400);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper1.distanceToGo() == 0)
      stepper1.moveTo(-stepper1.currentPosition());
      
    if (stepper2.distanceToGo() == 0)
      stepper2.moveTo(-stepper2.currentPosition());

    if (stepper3.distanceToGo() == 0)
      stepper3.moveTo(-stepper3.currentPosition());

    stepper1.run();
    stepper2.run();
    stepper3.run();    
}

