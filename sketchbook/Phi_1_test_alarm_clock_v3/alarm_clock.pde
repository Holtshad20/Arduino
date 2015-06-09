/*
Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.
*/
#include "alarm_clock.h"

alarm_clock::alarm_clock(boolean noo) // This parameter is need otherwise the compiler won't work with a constructor without parameters.
{
  alarm_is_on=false;
  snooze=10;
}

byte alarm_clock::run()
{
  int rtc[7];
  render_RTC(0);
  RTC.get(rtc,true);
  for (int i=0;i<4;i++)
  {
    if ((alarms[i].hr==rtc[2])&&(alarms[i].mnt==rtc[1])&&(rtc[0]==0)&&within(i,rtc[3]))
    {
      alarm_is_on=true;
    }
  }
  if (alarm_is_on) alarm();
}

byte alarm_clock::set_alarm(byte alarm_num, byte hr, byte mnt, byte dow)
{
  if (alarm_num>=Max_alarms) return 255;
  if (hr>23) return 255;
  if (mnt>59) return 255;
  alarms[alarm_num].hr=hr;
  alarms[alarm_num].mnt=mnt;
  alarms[alarm_num].dow=dow;
  return 0;
}

void alarm_clock::alarm()
{
  int temp1;
  for (int i=0;i<4;i++)
  {
    tone(buzzer,770);
    digitalWrite(led,HIGH);
    temp1=wait_on_escape(75);
    if ((temp1==5)||(temp1==6))
    {
      noTone(buzzer);
      digitalWrite(led,LOW);
      alarm_is_on=false; // Turn off alarm
      return;
    }
    noTone(buzzer);
    digitalWrite(led,LOW);
    temp1=wait_on_escape(75);
    if ((temp1==5)||(temp1==6))
    {
      noTone(buzzer);
      digitalWrite(led,LOW);
      alarm_is_on=false; // Turn off alarm
      return;
    }
  }
  temp1=wait_on_escape(150);
  if ((temp1==5)||(temp1==6))
  {
    noTone(buzzer);
    digitalWrite(led,LOW);
    alarm_is_on=false; // Turn off alarm
    return;
  }
}

boolean alarm_clock::within(byte a, byte dow) // This function checks if the day of the week is within alarm clock's trigger setting.
{
  switch(alarms[a].dow)
  {
    case 0: // Alarm is off
    return false;
    break;
    
    case 1: // Alarm is on daily
    return true;
    break;
    
    case 2: // Alarm is on M-F
    if ((dow>1)&&(dow<7)) return true;
    else return false;
    break;
    
    case 3: // Alarm is on Weekend
    if ((dow==1)||(dow==7)) return true;
    else return false;
    
    case 4: // Alarm is on once so set it to off and return true
    alarms[a].dow=0;
    return true;
    break;
    
    default:
    return false;
    break;
  }
}

void alarm_clock::turn_on(byte alarm_num) // This function is not used.
{
  if (alarm_num<Max_alarms) alarms[alarm_num].on_off=true;
} 

void alarm_clock::turn_off(byte alarm_num) // This function is not used.
{
  if (alarm_num<Max_alarms) alarms[alarm_num].on_off=false;
} 

