/*
  SevenDisp.h - Library for 2 display for 7 segments
  Created by Luís Bulek July, 2012.
  Released into the public domain.
*/
#ifndef SevenDisp_h
#define SevenDisp_h

#include "Arduino.h"
#include <inttypes.h>

// definição dos padrões para o display seguindo a ordem dos bits, 
// da esquerda para a direita abcdefg. 



class SevenDisp
{
  public:
    SevenDisp(uint8_t datapin, uint8_t latchpin, uint8_t clockpin);
	void Show(int x);
	void Base(uint8_t base);
	
  private:	
    uint8_t _datapin;
    uint8_t _latchpin;
    uint8_t _clockpin;
	int _x;
	int _a;
	int _b;
	uint8_t _base;
};
	

#endif
