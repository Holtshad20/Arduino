/*
 * nunchuck_lines sketch
 * sends data to Processing to draw line that follows nunchuk movement
 */

#include <Wire.h> // initialize wire
#include "nunchuck_funcs.h"


byte accx;

void setup()
{
  Serial.begin(9600);
  nunchuck_setpowerpins();
  nunchuck_init();
}

void loop()
{
  nunchuck_get_data();
  accx  = nunchuck_accelx();
  if( accx >= 75 && accx <= 185)
  {
    // map returns a value from 0 to 63 for values from 75 to 185
    byte  y = map(accx, 75, 185, 0, 63);
    Serial.print(y);
  }
  delay(100); // the time in milliseconds between redraws
}
      
    
