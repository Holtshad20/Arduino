/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 
 */
#define LEDPIN 3       // Led na saida 9

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

void setup()  { 
  // declare pin ledpin to be an output:
  pinMode(LEDPIN, OUTPUT);
} 

void loop()  { 
  // set the brightness of pin ledpin:
  analogWrite(LEDPIN, brightness);    

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);                            
}
