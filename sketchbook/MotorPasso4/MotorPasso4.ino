// Blocking.pde
// -*- mode: C++ -*-
//
// Shows how to use the blocking call runToNewPosition
// Which sets a new target position and then waits until the stepper has 
// achieved it.
//
// Copyright (C) 2009 Mike McCauley
// $Id: HRFMessage.h,v 1.1 2009/08/15 05:32:58 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(2, 2, 3);

void setup()
{  
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
    stepper.setMaxSpeed(1000.0);
    stepper.setAcceleration(1000.0);
    
}

void loop()
{    
    stepper.runToNewPosition(0);
    stepper.runToNewPosition(5000);
    stepper.runToNewPosition(-1100);
    stepper.runToNewPosition(0);
    stepper.runToNewPosition(-300);
    stepper.runToNewPosition(6200);
}
