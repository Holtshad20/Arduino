#ifndef PID_H
#define PID_H

#include "Arduino.h"
#include <avr/io.h>
#include <util/delay.h>

	
	long error;
	long integral;
	long derivative;
	long propotional;
	long last_error;
	long pid_sum;



long pid(long target,long posi,long Kp,long Ki,long Kd,long dt)
{
	error = target - posi ;

	last_error = error ;

        integral = integral + ( error * dt ) ;

	derivative = ( error - last_error ) / dt ;
	
	pid_sum = ( Kp * error ) + ( Ki * integral ) + ( Kd * derivative ) ;
	
        _delay_ms(dt);
		
	return pid_sum;
}


#endif
