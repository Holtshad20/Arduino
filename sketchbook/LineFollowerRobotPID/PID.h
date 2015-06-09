#ifndef PID_H
#define PID_H

#include "Arduino.h"
#include <avr/io.h>
#include <util/delay.h>
class PID
{
  public:
	long pid(long target,long posi,long Kp,long Ki,long Kd,long dt);
   
  
	long error;
	long integral;
	long derivative;
	long propotional;
	long last_error;
	long pid_sum;

};

#endif
