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
AccelStepper stepper1(2,2,3); // 

void setup()
{  
    
    stepper1.setMaxSpeed(300.0);
    stepper1.setAcceleration(300.0);
    stepper1.moveTo(800);
    
}

void loop()
{
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
	stepper1.moveTo(-stepper1.currentPosition());

    stepper1.run();    
}
