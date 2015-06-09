/*
Phi-1 shield for Arduino
Test program: Testing

Programmed by Dr. John Liu
Revision: 11/21/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.

List of functions:
* Tests LCD
* Tests all buttons
* Tests buzzer
* Tests LED
* Tests RTC
* Tests optional EEPROM (24LC256 or compatible I2C EEPROMs)

* TODO - tests GPS, well, I don't have a GPS yet.

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

// Messages used in program.
PROGMEM prog_char keys_00[]=" UP  ";
PROGMEM prog_char keys_01[]="DOWN ";
PROGMEM prog_char keys_02[]="LEFT ";
PROGMEM prog_char keys_03[]="RIGHT";
PROGMEM prog_char keys_04[]="--B--";
PROGMEM prog_char keys_05[]="--A--";

PROGMEM prog_char dow_00[]="SUN";
PROGMEM prog_char dow_01[]="MON";
PROGMEM prog_char dow_02[]="TUE";
PROGMEM prog_char dow_03[]="WED";
PROGMEM prog_char dow_04[]="THU";
PROGMEM prog_char dow_05[]="FRI";
PROGMEM prog_char dow_06[]="SAT";
PROGMEM prog_char dow_07[]="Day of the Week";

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
  RTC.stop();
  RTC.set(DS1307_SEC,0);
  RTC.set(DS1307_MIN,50);
  RTC.set(DS1307_HR,0);
  RTC.set(DS1307_DOW,3); // value from 1 to 7. User define whether 1 is sun or mon.
  RTC.set(DS1307_DATE,16);
  RTC.set(DS1307_MTH,11);
  RTC.set(DS1307_YR,10);
  RTC.start();
#endif
}

void loop()
{
  char somedata[] = "This message was read   from the EEPROM."; // data to write
  int temp1=0, all_keys=0;
  int addr=0; //first address
  byte b=1; // access the first address from the memory
  byte temp2=0;
  char temp[sizeof(somedata)];

// Testing LCD
  lcd.clear();
  lcd.print("Adjust contrast!");
  lcd.setCursor(0,1);
  lcd.print("anykey when done");
  while(temp1==0)
  {
    temp1=wait_on_escape(1000);
  }
  
// Testing keys
  temp1=0;
  lcd.clear(); 
  lcd.print("Testing all keys");
  lcd.setCursor(0,1);
  lcd.print("Key:");

  while(1)
  {
    while(temp1==0)
    {
      temp1=wait_on_escape(1000);
    }
    lcd.setCursor(5,1);
    msg_lcd(keys_00+6*(temp1-1));
    all_keys=(all_keys|(1<<(temp1-1)));
    delay(250);
    temp1=0;
    if (all_keys==B111111) break;
  }
  
  lcd.clear();
  lcd.print("All keys passed");
  lcd.setCursor(0,1);
  lcd.print("the test.");
  wait_on_escape(5000);
  
// Testing the buzzer and the LED
  temp1=0;
  lcd.clear();
  lcd.print("If board restart");
  lcd.setCursor(0,1);
  lcd.print("Increase R value");
  wait_on_escape(5000);
  while(temp1==0)
  {
    tone(buzzer,770);
    digitalWrite(led,HIGH);
    temp1=wait_on_escape(75);
    noTone(buzzer);
    digitalWrite(led,LOW);
    temp1+=wait_on_escape(75);
    tone(buzzer,770);
    digitalWrite(led,HIGH);
    temp1+=wait_on_escape(75);
    noTone(buzzer);
    digitalWrite(led,LOW);
    temp1+=wait_on_escape(75);
    temp1+=wait_on_escape(850);
  }  

// Testing RTC. This will display 2000 and not advance the clock if the RTC is missing or not properly installed.
  temp1=0;
  lcd.clear(); 
  lcd.print("Testing DS1307");
  wait_on_escape(2000);
  while(temp1==0)
  {
    for(int i=0;i<6;i++)
    {
      render_RTC(0); // Displays the clock with full information on the display
      temp1=wait_on_escape(500);
      if (temp1!=0) break;
    }
    lcd.clear();
    lcd.print("Press any key");
    lcd.setCursor(0,1);
    lcd.print("to continue");
    temp1=wait_on_escape(2000);
  }
  
// Testing EEPROM. This will hang the program if the EEPROM is missing, incompatible or not properly installed.
  temp1=0;
  lcd.clear(); 
  lcd.print("Writing EEPROM");
  i2c_eeprom_write_page(0x50, 0, (byte *)somedata, 30); // write to EEPROM. Arduino chip limits to 30 bytes per time.
  delay(10); // Wait for EEPROM to finish writing.
  i2c_eeprom_write_page(0x50, 30, (byte *)somedata+30, sizeof(somedata)-30); // write to EEPROM. Arduino chip limits to 30 bytes per time. 
  delay(10); // Wait for EEPROM to finish writing.

  lcd.setCursor(0,1);
  while (addr<sizeof(somedata)) 
  {
    b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    temp[addr]=(char)b;
    addr++; //increase address
  }
  lcd.clear();
  lcd.autoscroll();
  while(temp1==0)
  {
    temp1=0, temp2=0;
    lcd.setCursor(16,1);
    while((temp[temp2]!=0)&&(temp1==0))
    {
      lcd.print(temp[temp2]);
      temp1=wait_on_escape(250);
      temp2++;
    }
    temp1=wait_on_escape(500);
    lcd.clear();
  }
  lcd.noAutoscroll();
  lcd.clear(); 
  lcd.print("Restarting test");
  wait_on_escape(1000);
}
