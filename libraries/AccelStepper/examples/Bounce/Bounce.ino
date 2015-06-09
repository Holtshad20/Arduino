// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit to another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Definição para controle STEP + DIR, DRIVER(1), STEP, DIR
AccelStepper stepper(1,2,3); 

void setup()
{  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(100);
  stepper.setAcceleration(20);
  stepper.moveTo(500);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}

