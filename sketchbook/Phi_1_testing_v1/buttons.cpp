/*
Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.
*/
#include "buttons.h"
buttons::buttons(byte p, byte pd)
{
  pressed=pd;
  pin=p;
  stat=buttons_up;
  counts=0;
  holding=false;
  pinMode(p, INPUT);
  digitalWrite(p, HIGH);
}

byte buttons::sense()
{
  switch(stat)
  {
    case buttons_up:
    do_up();
    break;
    
    case buttons_debounce:
    do_debounce();
    break;
    
    case buttons_down:
    do_down();
    break;
    
    case buttons_held:
    do_held();
    break;
    
    case buttons_released:
    do_released();
    break;
    
    default:
    break;
  }
  return stat;
}

void buttons::do_up()
{
  holding=false;
  counts=0;
  if (digitalRead(pin)==pressed)
  {
    stat=buttons_debounce;
    t_down=millis();
  }
}

void buttons::do_debounce()
{
  if (digitalRead(pin)==pressed)
  {
    if ((millis()-t_down)>buttons_debounce_time) stat=buttons_down;
  }
  
  else stat=buttons_up;
}

void buttons::do_down()
{
  int thre;
  if (digitalRead(pin)!=pressed)
  {
    stat=holding?buttons_up:buttons_released;
  }
  else
  {
    if (holding)
    {
      if (counts>buttons_dash_threshold) thre=buttons_dash_time;
      else thre=buttons_repeat_time;
    }
    else thre=buttons_hold_time;
    
    
    if (millis()-t_down>thre)
    {
      stat=buttons_held;
      if (counts<=buttons_dash_threshold) counts++;
    }
    else
    {
      stat=buttons_down;
    }
  }
}

void buttons::do_released()
{
  stat=buttons_up;
  holding=false;
  counts=0;
}

void buttons::do_held()
{
  holding=true;
  if (digitalRead(pin)==pressed)
  {
    stat=buttons_down;
    t_down=millis();
  }
  else
  {
    stat=buttons_released;
  }
}

