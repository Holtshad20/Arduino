/*
 * GoogleEarthPSX
 *
 * Send joystick data from PSX to Processing
 * uses PSX library discussed in Recipe 6.17
 */


#include <Psx.h>                      // Includes the Psx Library

Psx Psx;                              // Create an instance of the Psx library
const int dataPin = 5;
const int cmndPin = 4;
const int attPin = 3;
const int clockPin = 2;
const int psxDelay = 10; // this determines the clock delay in microseconds

const byte nudge = 64;   // the amount of movement to be sent when stick 
                         // is pushed
const byte HEADER = 255; // this value is sent as the header
unsigned int data;
byte x,y, buttons;

void setup()
{
  Serial.begin(9600);
  Psx.setupPins(dataPin, cmndPin, 
                attPin, clockPin, psxDelay);  // initialize Psx
}


void loop()
{

  data = Psx.read();   // get the psx controller button data
  x = y = 127;         // center x & y values, offsets are added 
                       // if buttons pressed
  buttons = 0;

 if(data & psxLeft || data & psxSqu)
    x = x - nudge;
 if(data & psxDown ||data & psxX)
    y = y + nudge;
 if(data & psxRight ||data & psxO)
    x = x + nudge;
 if(data & psxUp ||data & psxTri)
    y = y - nudge;
  if(data & psxStrt)  // (Z button)
     buttons = buttons + 2;
  if(data & psxSlct)  // (C button)
     buttons = buttons + 1;


 Serial.print(HEADER);
 Serial.print(x);
 Serial.print(y);
 Serial.print(buttons);

 delay(20); // send position 50 times a second
}
      
    
