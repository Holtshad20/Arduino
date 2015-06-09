/*
Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.
*/
#ifndef alarm_clock_h
#define alarm_clock_h
#include <WProgram.h>

#define Max_alarms 4

class alarm_clock
{
public:
  alarm_clock(boolean noo);
  byte run(); // Returns which alarm is triggered, 0-Max_alarms, 255 means no alarm is triggered.
  byte set_alarm(byte alarm_num, byte hr, byte mnt, byte dow); // Returns 255 if parameters are invalid. Returns 0 if valid.
  void turn_on(byte alarm_num);
  void turn_off(byte alarm_num);
  void alarm();
  boolean within(byte a, byte dow);
  
  typedef struct {
  byte hr;
  byte mnt;
  byte dow;
  boolean on_off; // This variable is not used. Instead dow is used to turn the alarm off or on.
  } entry;

  entry alarms[Max_alarms];
  boolean alarm_is_on; // Alarm is on. alarm() will be called if this is true.
  byte snooze;
};
#endif
