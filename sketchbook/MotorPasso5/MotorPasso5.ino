// Random.pde
// -*- mode: C++ -*-
//
// Make a single stepper perform random changes in speed, position and acceleration
//
// Copyright (C) 2009 Mike McCauley
// $Id: HRFMessage.h,v 1.1 2009/08/15 05:32:58 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(2, 2, 3); // Defaults to 4 pins on 8, 9, 10, 11

void setup()
{  
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
}

void loop()
{
    if (stepper.distanceToGo() == 0)
    {
	// Random change to speed, position and acceleration
	// Make sure we dont get 0 speed or accelerations
	delay(1000);
	stepper.moveTo(rand() % 5000);
	stepper.setMaxSpeed((rand() % 3000) + 1);
	stepper.setAcceleration((rand() % 2000) + 1);
    }
    stepper.run();
}
