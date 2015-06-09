/*
Phi-1 shield for Arduino
Test program: Alarm clock

Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.

List of functions:
* Display real time clock with Year month, date, day of week, hour, minute, and second.
* Four individual alarms (change Max_alarms to get more alarms)
* Alarms are stored on Arduino EEPROM and won't be lost when the power is out.
* Alarms Daily, Weekday, Weekend, Once, Off.
* Adjustable alarms. Press B to choose alarm and adjust the time.
* Adjustable clock and date. Press A to adjust clock and date.
* TODO - Add snooze function.
* TODO - Add special function on the alarm to drive outputs other than the buzzer and LED. You can be creative!

Instructions:

You need to set the clock before you can use it.
1. Uncomment this line "#define setRTC" among the #define lines.
2. Update the time in setup() to the current time.
3. Upload the sketch to arduino to set the time.
4. Then comment the line "#define setRTC" and recompile and upload to arduino.
5. If you don't do step 4, the clock will be reset every time arduino resets.
----------------------------------------
Thanks:

RTC created by matt.joyce@gmail.com

EEPROM Author: hkhijhe

*/
#include <LiquidCrystal.h>
#include <WProgram.h>
#include <Wire.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "buttons.h"
#include "DS1307_1.h" 
#include "alarm_clock.h"

int hmi_with_update_2(int current, int lower, int upper, byte inc, byte column, byte row, byte space, void (*update_function)(int));

// Defining pins used
//#define setRTC
#define buzzer 16
#define ch1 16
#define ch1_analog 2
#define led 17
#define ch2 17
#define ch2_analog 3
#define btn_u 5
#define btn_d 10
#define btn_l 11
#define btn_r 3
#define btn_b 14
#define btn_a 15
#define alarm_EEPROM_storage 1023-3*Max_alarms // Where the alarm is saved.
// Messages used in program.
PROGMEM prog_char dow_00[]="SUN";
PROGMEM prog_char dow_01[]="MON";
PROGMEM prog_char dow_02[]="TUE";
PROGMEM prog_char dow_03[]="WED";
PROGMEM prog_char dow_04[]="THU";
PROGMEM prog_char dow_05[]="FRI";
PROGMEM prog_char dow_06[]="SAT";
PROGMEM prog_char dow_07[]="M-F";
PROGMEM prog_char dow_08[]="Day of the week";

PROGMEM prog_char alarm_00[]="Off    ";
PROGMEM prog_char alarm_01[]="Daily  ";
PROGMEM prog_char alarm_02[]="Weekday";
PROGMEM prog_char alarm_03[]="Weekend";
PROGMEM prog_char alarm_04[]="Once   ";

PROGMEM prog_char month_00[]="JAN";
PROGMEM prog_char month_01[]="FEB";
PROGMEM prog_char month_02[]="MAR";
PROGMEM prog_char month_03[]="APR";
PROGMEM prog_char month_04[]="MAY";
PROGMEM prog_char month_05[]="JUN";
PROGMEM prog_char month_06[]="JUL";
PROGMEM prog_char month_07[]="AUG";
PROGMEM prog_char month_08[]="SEP";
PROGMEM prog_char month_09[]="OCT";
PROGMEM prog_char month_10[]="NOV";
PROGMEM prog_char month_11[]="DEC";
PROGMEM prog_char month_12[]="Month";

// Defines button objects
buttons btn_1(btn_u, LOW);
buttons btn_2(btn_d, LOW);
buttons btn_3(btn_l, LOW);
buttons btn_4(btn_r, LOW);
buttons btn_5(btn_b, LOW);
buttons btn_6(btn_a, LOW);

int rtc[7];
LiquidCrystal lcd(8, 9, 7, 6, 2, 4); //Phi-1 shield setting
DS1307 RTC=DS1307(); // Create RTC object
alarm_clock clock1(false); // Create an alarm clock

void setup()
{
  lcd.begin(16, 2);
  Wire.begin(); // initialize wire

// Set button pins to input with internal pull-up resistors
  pinMode(btn_u,INPUT);
  digitalWrite(btn_u,HIGH);
  pinMode(btn_d,INPUT);
  digitalWrite(btn_d,HIGH);
  pinMode(btn_l,INPUT);
  digitalWrite(btn_l,HIGH);
  pinMode(btn_r,INPUT);
  digitalWrite(btn_r,HIGH);
  pinMode(btn_b,INPUT);
  digitalWrite(btn_b,HIGH);
  pinMode(btn_a,INPUT);
  digitalWrite(btn_a,HIGH);

// Set the two channels to output to drive the buzzer and LED.
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);

#ifdef setRTC
// Set/init RTC
  RTC.stop();
  RTC.set(DS1307_SEC,0);
  RTC.set(DS1307_MIN,50);
  RTC.set(DS1307_HR,0);
  RTC.set(DS1307_DOW,3); // value from 1 to 7. User define whether 1 is sun or mon.
  RTC.set(DS1307_DATE,16);
  RTC.set(DS1307_MTH,11);
  RTC.set(DS1307_YR,10);
  RTC.start();

//Set alarms
  EEPROM.write(alarm_EEPROM_storage,6); // Alarm 0 hour
  EEPROM.write(alarm_EEPROM_storage+1,50); // Alarm 0 minute
  EEPROM.write(alarm_EEPROM_storage+2,2); // Alarm 0 frequency M-F

  EEPROM.write(alarm_EEPROM_storage+3,7); // Alarm 1 hour
  EEPROM.write(alarm_EEPROM_storage+4,00); // Alarm 1 minute
  EEPROM.write(alarm_EEPROM_storage+5,2); // Alarm 1 frequency M-F

  EEPROM.write(alarm_EEPROM_storage+6,7); // Alarm 2 hour
  EEPROM.write(alarm_EEPROM_storage+7,10); // Alarm 2 minute
  EEPROM.write(alarm_EEPROM_storage+8,2); // Alarm 2 frequency M-F

  EEPROM.write(alarm_EEPROM_storage+9,9); // Alarm 3 hour
  EEPROM.write(alarm_EEPROM_storage+10,30); // Alarm 3 minute
  EEPROM.write(alarm_EEPROM_storage+11,3); // Alarm 3 frequency Weekend
#else
// Set alarm on the clock from EEPROM.
  clock1.set_alarm(0, EEPROM.read(alarm_EEPROM_storage), EEPROM.read(alarm_EEPROM_storage+1), EEPROM.read(alarm_EEPROM_storage+2));
  clock1.set_alarm(1, EEPROM.read(alarm_EEPROM_storage+3), EEPROM.read(alarm_EEPROM_storage+4), EEPROM.read(alarm_EEPROM_storage+5));
  clock1.set_alarm(2, EEPROM.read(alarm_EEPROM_storage+6), EEPROM.read(alarm_EEPROM_storage+7), EEPROM.read(alarm_EEPROM_storage+8));
  clock1.set_alarm(3, EEPROM.read(alarm_EEPROM_storage+9), EEPROM.read(alarm_EEPROM_storage+10), EEPROM.read(alarm_EEPROM_storage+11));
#endif

}

void loop()
{
  int temp1;
  clock1.run();
  if (!clock1.alarm_is_on)
  {
    temp1=wait_on_escape(250);
    switch (temp1)
    {
      case 1:
      lcd.clear();
      lcd.print("liudr.wordpress");
      lcd.setCursor(0,1);
      lcd.print(".com for details");
      wait_on_escape(5000);
      break;
      
      case 5:
      set_alarm(0);
      break;
      
      case 6:
      set_clock();
      break;
      
      default:
      break;
    }
  }  
}
