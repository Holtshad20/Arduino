/*
  LED8SegDisplay595_h - Library to manage "n" 8 segment displays using 595 CI
  Created by Mauro Assis in 21/jul/2012
  Released into the public domain.
*/

#ifndef LED8SegDisplay595_h
#define LED8SegDisplay595_h

#include "Arduino.h"

class LED8SegDisplay595
{
   public:
// Constructor
//   int _displayCount: number of 8 segment displays to be managed
//   int _clockPin: number of Arduino port that´s connected the clock pin of the first 595 CI (CI pin 12)
//   int _latchPin: number of Arduino port that´s connected the latch pin of the first 595 CI (CI pin 11)
//   int _dataPin: number of Arduino port that´s connected the data pin of the first 595 CI (CI pin 14)
     LED8SegDisplay595(int _displayCount, int _clockPin, int _latchPin, int _dataPin);
//   set the corresponding display mask (from now on, it will be considered common cathod)
     void setDisplayMask(byte display);
//   clear display mask
     void clearDisplayMask(byte display);     
//   show error message Er.      
     void errorMessage(int millissecs);
//   write a value in the displays
     void write(byte value);
     void write(float value, int decimalPlaces);
//   switch off the RGB LED corresponding colors      
     int debug;    //Indicates debug mode  
     byte displayMask[8]; //=  {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};        
     char buf[33];
     long errMilissecs;
   private:
     int displayCount;  //Number of LEDs to manage  
     int clockPin;  //Pin connected to Pin 11 of 74HC595 (Clock)
     int latchPin;  //Pin connected to Pin 12 of 74HC595 (Latch)
     int dataPin;   //Pin connected to Pin 14 of 74HC595 (Data) 
     void shiftOut(byte dataOut); 
};
#endif
