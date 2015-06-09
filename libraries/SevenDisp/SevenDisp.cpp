/*
  SevenDisp.cpp - Library for 2 display for 7 segments
  Created by Luís Bulek July, 2012.
  Released into the public domain.
*/

#include "Arduino.h"
#include "SevenDisp.h"

    byte _LEDDSP [] = {           
		B00000011, // 0
		B10011111, // 1
		B00100101, // 2
		B00001101, // 3
        B10011001, // 4
		B01001001, // 5 
		B01000001, // 6
		B00011111, // 7
		B00000001, // 8
		B00001001, // 9
		B00010001, // a
		B11000001, // b
		B01100011, // c
		B10000101, // d
		B01100001, // e
		B01110001, // f
	};	



SevenDisp::SevenDisp(uint8_t datapin, uint8_t latchpin, uint8_t clockpin)
{
  _datapin  = datapin;
  _latchpin = latchpin;
  _clockpin = clockpin;
  _base = 10;
  
   pinMode(_datapin, OUTPUT);
   pinMode(_latchpin, OUTPUT);
   pinMode(_clockpin, OUTPUT);
}

void SevenDisp::Show(int x)
{
  _x = x; 
  _a = (_x / _base);
  _b = _x - (_a * _base);
  
  digitalWrite(_latchpin, LOW);   
  shiftOut(_datapin, _clockpin, LSBFIRST, _LEDDSP[_b]); // envia primeiro a unidade
  shiftOut(_datapin, _clockpin, LSBFIRST, _LEDDSP[_a]); // envia a dezena 
  digitalWrite(_latchpin, HIGH);
}

void SevenDisp::Base(uint8_t base)
{
  _base = base;
}



