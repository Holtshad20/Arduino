// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: HRFMessage.h,v 1.1 2009/08/15 05:32:58 mikem Exp mikem $

#include <AccelStepper.h>

// Define some steppers and the pins the will use
AccelStepper stepper1(2,2,3); // Defaults to 4 pins on 2, 3, 4, 5
AccelStepper stepper2(2,4,5);


void setup()
{  
    stepper1.setMaxSpeed(100.0);
    stepper1.setAcceleration(100.0);
    stepper1.moveTo(100);
    
    stepper2.setMaxSpeed(300.0);
    stepper2.setAcceleration(100.0);
    stepper2.moveTo(500);
    
}

void loop()
{
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
	stepper1.moveTo(-stepper1.currentPosition());
    if (stepper2.distanceToGo() == 0)
	stepper2.moveTo(-stepper2.currentPosition());

    stepper1.run();    
    stepper2.run();
}
