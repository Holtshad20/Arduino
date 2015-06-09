/*
  LED8SegDisplay595_h - Library to manage "n" 8 segment displays using 595 CI
  Created by Mauro Assis in 21/jul/2012
  Released into the public domain.
*/

#include "Arduino.h"
#include "math.h"
#include "LED8SegDisplay595.h"

byte digits[11] = 
//  ABCDEFGd
  {B11111100, // 0
   B01100000, // 1
   B11011010, // 2
   B11110010, // 3
   B01100110, // 4
   B10110110, // 5
   B10111110, // 6
   B11100000, // 7
   B11111110, // 8
   B11110110, // 9
   B00000001}; // dot  

byte err[2] =
  {B10011110,  // E
   B00001011}; // r.

LED8SegDisplay595::LED8SegDisplay595(int _displayCount, int _clockPin, int _latchPin, int _dataPin)
{
  displayCount = _displayCount; 
  clockPin=_clockPin;
  dataPin=_dataPin;
  latchPin = _latchPin;  
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  for (int i = 0; i < displayCount; i++)
  {
    displayMask[i] = 0;
  }
  errMilissecs=1000;
}
void LED8SegDisplay595::setDisplayMask(byte display)
{
   displayMask[display]=B11111111;
}

void LED8SegDisplay595::clearDisplayMask(byte display)
{
   displayMask[display]=B00000000;
}

void LED8SegDisplay595::errorMessage(int millissecs)
{
  digitalWrite(latchPin, LOW);
  for (byte i=(displayCount-1); i>2; i--)
  {
      shiftOut(B00000000 ^ displayMask[i]);
  }
  shiftOut(err[0] ^ displayMask[0]);       
  if (displayCount > 1)
     shiftOut(err[1] ^ displayMask[1]);       
  digitalWrite(latchPin, HIGH);
  delay(millissecs);
}

void LED8SegDisplay595::write(byte value)
{
  if (value >= pow(10,displayCount)) // doesn´t fits    
  {
     errorMessage(errMilissecs);
     return;
  }    
  byte divisor; 
  digitalWrite(latchPin, LOW);
  for (byte i=(displayCount-1); i>0; i--)
  {   
    divisor=pow(10,i);  
    shiftOut(digits[value / divisor] ^ displayMask[i]);
  }    
  shiftOut(digits[value % 10] ^ displayMask[0]);
  digitalWrite(latchPin, HIGH);
}

void LED8SegDisplay595::write(float value, int decimalPlaces)
{
   if (value >= pow(10,displayCount)) // doesn´t fits    
   {
      errorMessage(errMilissecs);
      return;
   }     
   dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf); 
   int i=0;
   int digit=0;
   digitalWrite(latchPin, LOW);
   while (1)
   {
      if (i > displayCount)
         break;   
      if (((buf[i] < '0') || (buf[i] > '9')) && (buf[i] != '.'))
         break;
      if ((buf[i] >= '0') && (buf[i] <= '9'))
      {
         digit=digits[buf[i] - 48] ^ displayMask[i];
         if (buf[i+1] == '.')
         {
            digit=digit | (digits[10] ^ displayMask[i]); // dot
            i++;
         }   
      }
      shiftOut(digit);
      i++;
   }   
   digitalWrite(latchPin, HIGH);
}

void LED8SegDisplay595::shiftOut(byte dataOut) {
  boolean pinState;
  digitalWrite(dataPin, LOW); 
  digitalWrite(clockPin, LOW);
  for (int i=0; i<=7; i++) { 
    digitalWrite(clockPin, LOW); 
    if ( dataOut & (1<<i) ) {
      pinState = HIGH;
    }
    else {
      pinState = LOW;
    }
    digitalWrite(dataPin, pinState); 
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(clockPin, LOW); 
}


