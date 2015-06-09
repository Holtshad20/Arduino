/*
 * MultiRX sketch
 * Receive data from two software serial ports
 */

#include <NewSoftSerial.h>
const int rxpin1 = 2;
const int txpin1 = 3;
const int rxpin2 = 4;
const int txpin2 = 5;
NewSoftSerial gps(rxpin1, txpin1);  // gps device connected to pins 2 and 3
NewSoftSerial xbee(rxpin2, txpin2); // gps device connected to pins 2 and 3

void setup()
{
  gps.begin(4800);
  xbee.begin(9600);
}

void loop()
{
  if (xbee.available() > 0) // xbee is active.  Any characters available?
  {
    if (xbee.read() == 'y') // if xbee received a 'y' character
    {
      unsigned long start = millis(); // begin listening to the GPS
      while (start + 100000 > millis())
      // listen for 10 seconds
      {
        if (gps.available() > 0) // now gps device is active
        {
          char c = gps.read();
          // *** process gps data here
        }
      }
    }
  }
}
      
    
