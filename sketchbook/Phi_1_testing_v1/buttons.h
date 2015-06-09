/*
Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.
*/
#ifndef buttons_h
#define buttons_h
#include <WProgram.h>

#define buttons_up 0 // no-transitional
//#define buttons_pressed 1 // transitional, no longer in use
#define buttons_down 2 // no-transitional
#define buttons_held 3
#define buttons_released 4
#define buttons_debounce 5 // One needs to wait till debounce is over to become pressed.
#define buttons_hold_time 1000
#define buttons_debounce_time 50
#define buttons_dash_threshold 10
#define buttons_repeat_time 250
#define buttons_dash_time 50
#define buttons_disabled 99

class buttons
{
  public:
  byte stat;
  byte pin;
  byte pressed; // Polarity of buttons. For those with push-up resistors enabled, button pressed corresponds to low so this is LOW. 
  byte counts;
  boolean holding;
  unsigned long t_down;
  byte sense();
  void do_up();
  void do_debounce();
  void do_down();
  void do_released();
  void do_held();
  buttons(byte p);
  buttons(byte p, byte pd);
};
#endif
// pressed=HIGH means pressing buttons gives HIGH. A pull-down resistor is needed. pressed=LOW means pressing buttons gives LOW. One can use the internal pull-up resistor.
