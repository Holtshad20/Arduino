/*
 * WiichuckSerial
 *
 * based on code from Tod E. Kurt, http://thingm.com/
 * Modified to send serial packets to Processing
 *
 */


#include <Wire.h>
#include "nunchuck_funcs.h"

int loop_cnt=0;
const byte header = 254;               // a value to indicate start of message

byte accx,accy,joyx,joyy,buttons;

// set the current coordinates as the center points

void setup()
{
    Serial.begin(9600);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initialization handshake
    nunchuck_get_data(); // ignore the first time
    delay(50);
}

void loop()
{
    if( loop_cnt > 50 ) { // every 50 msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();

        accx  = nunchuck_accelx();
        accy  = nunchuck_accely();
        buttons = nunchuck_zbutton() * 2;
        buttons = buttons + nunchuck_cbutton(); // cbutton is least significant bit

        Serial.print(header);          // value indicating start of message
        Serial.print((byte)accx);
        Serial.print((byte)accy);
        Serial.print((byte)buttons);

    }
    loop_cnt++;
    delay(1);
}
      
    
