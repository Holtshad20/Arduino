/*
  Demo of: 
  LED8SegDisplay595_h - Library to manage "n" 8 segment displays using 595 CI
  Created by Mauro Assis in 21/jul/2012
  Released into the public domain.
*/

#include <Arduino.h>
#include <LED8SegDisplay595.h>
#define latchPin 8  //Pin connected to Pin 12 of 74HC595 (Latch)
#define clockPin 12 //Pin connected to Pin 11 of 74HC595 (Clock)
#define dataPin 11  //Pin connected to Pin 14 of 74HC595 (Data)

LED8SegDisplay595 ls(2,clockPin,latchPin,dataPin);

void setup() {
}

void writeSomeNumbers()
{
  ls.write(1);
  delay(500);
  ls.write(87);
  delay(500);
  ls.write(34);
  delay(500);
  ls.write(22);
  delay(500);
}

void reverseNumbers()
{
  ls.setDisplayMask(0);  
  ls.setDisplayMask(1);  
// write some numbers
  ls.write(1);
  delay(500);
  ls.write(33);
  delay(500);
  ls.write(0);
  delay(500);
  ls.write(22);
  delay(500);
  ls.clearDisplayMask(0);  
  ls.clearDisplayMask(1);  
}

void reverseCounter()
{
// reverse counter
  for (int i=99; i>=0; i--)
  {
    ls.write(i);
    delay(100);
  }  
}

void errorMessageOverflow()
{
  // Error message (overflow)  
  ls.write(1000);
}  

void writeFloat()
{
  ls.write(8.3,2);
  delay(1000);
  ls.write(2.12323,5);
  delay(1000);
  ls.write(4.45,2);
  delay(1000);
  ls.write(9.99,2);
  delay(1000);
  ls.write(1.0,1);
  delay(1000);
}

void temperatureAlarm()
{
  while (1)
  {
// thermometer with above 30 oC alarm
    int sensorValue = analogRead(0);    
    byte temp = map(sensorValue,600,700,15,36);
    if (temp > 30)
      tone(5,880);
    else
      noTone(5);  
    ls.write(temp);
    delay(300);
  }
}

void loop() {
  writeSomeNumbers();
  reverseCounter();
  reverseNumbers();
  writeFloat();
  errorMessageOverflow();
  temperatureAlarm();
}

