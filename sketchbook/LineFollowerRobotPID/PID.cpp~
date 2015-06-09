#include "Arduino.h"
#include "PID.h"
#include <avr/io.h>
#include <util/delay.h>
long PID::pid(long target,long posi,long Kp,long Ki,long Kd,long dt)
{
	error = target - posi ;

	last_error = error ;

        integral = integral + ( error * dt ) ;

	derivative = ( error - last_error ) / dt ;
	
	pid_sum = ( Kp * error ) + ( Ki * integral ) + ( Kd * derivative ) ;
	
        _delay_ms(dt);
		
	return pid_sum;
}
