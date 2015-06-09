/*
Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.
*/
void msg_lcd(char* msg_line)
{
  char msg_buffer[17];
  strcpy_P(msg_buffer,msg_line); 
  lcd.print(msg_buffer);
}

void set_clock()
{
  int temp1;
  render_RTC(0);
  RTC.get(rtc,true);
  temp1=hmi_with_update_2(rtc[5]-1, 0, 11, 1, 0, 0, 3, render_month_in_place);
  rtc[5]=(temp1==-1)?rtc[5]:temp1+1; // In case escape was triggered, value doesn't change.
  delay(100);
  temp1=hmi_with_update_2(rtc[4], 1, 31, 1, 4, 0, 2, render_00number_in_place);
  rtc[4]=(temp1==-1)?rtc[4]:temp1; // In case escape was triggered, value doesn't change.
  delay(100);
  temp1=hmi_with_update_2(rtc[6], 2000, 2099, 1, 7, 0, 4, render_number_in_place); // RTC library already adds 2000 to returned value in rtc[6].
  rtc[6]=(temp1==-1)?rtc[6]:temp1-2000; // In case escape was triggered, value doesn't change. Setting the year requires just two digits like 10 instead of 2010.
  delay(100);
  temp1=hmi_with_update_2(rtc[3]-1, 0, 6, 1, 12, 0, 3, render_DOW_in_place);
  rtc[3]=(temp1==-1)?rtc[3]:temp1+1; // In case escape was triggered, value doesn't change.
  delay(100);
  temp1=hmi_with_update_2(rtc[2], 0, 23, 1, 4, 1, 2, render_00number_in_place);
  rtc[2]=(temp1==-1)?rtc[2]:temp1; // In case escape was triggered, value doesn't change.
  delay(100);
  temp1=hmi_with_update_2(rtc[1], 0, 59, 1, 7, 1, 2, render_00number_in_place);
  rtc[1]=(temp1==-1)?rtc[1]:temp1; // In case escape was triggered, value doesn't change.
  delay(100);
  temp1=hmi_with_update_2(rtc[0], 0, 59, 1, 10, 1, 2, render_00number_in_place);
  rtc[0]=(temp1==-1)?rtc[0]:temp1; // In case escape was triggered, value doesn't change.

  RTC.stop();
  RTC.set(DS1307_SEC,rtc[0]); //rtc[0]
  RTC.set(DS1307_MIN,rtc[1]); //rtc[1]
  RTC.set(DS1307_HR,rtc[2]);  //rtc[2]
  RTC.set(DS1307_DOW,rtc[3]); //rtc[3] value from 1 to 7. User define whether 1 is sun or mon.
  RTC.set(DS1307_DATE,rtc[4]);//rtc[4]
  RTC.set(DS1307_MTH,rtc[5]); //rtc[5] Month
  RTC.set(DS1307_YR,rtc[6]);  //rtc[6]
  RTC.start();

  delay(100);
}

void set_alarm(byte ala_num)
{
  int temp1, ala=0;
  char msg[17];
  lcd.clear();
  sprintf(msg,"Alarm %02d:",0);
  lcd.print(msg);

  temp1=hmi_with_update_2(ala, 0, Max_alarms-1, 1, 6, 0, 2, render_00number_in_place);
  ala=(temp1==-1)?ala:temp1; // In case escape was triggered, value doesn't change.

  lcd.setCursor(9,0); // Render alarm date
  render_alarm_in_place(clock1.alarms[ala].dow);
  lcd.setCursor(4,1); // Render alarm clock
  sprintf(msg,"%02d:%02d", clock1.alarms[ala].hr,clock1.alarms[ala].mnt);
  lcd.print(msg);

  temp1=hmi_with_update_2(clock1.alarms[ala].dow, 0, 4, 1, 9, 0, 7, render_alarm_in_place);
  clock1.alarms[ala].dow=(temp1==-1)?clock1.alarms[ala].dow:temp1; // In case escape was triggered, value doesn't change.
  EEPROM.write(alarm_EEPROM_storage+3*ala+2,clock1.alarms[ala].dow); // Alarm frequency M-F
  delay(100);

  temp1=hmi_with_update_2(clock1.alarms[ala].hr, 0, 23, 1, 4, 1, 2, render_00number_in_place);
  clock1.alarms[ala].hr=(temp1==-1)?clock1.alarms[ala].hr:temp1; // In case escape was triggered, value doesn't change.
  EEPROM.write(alarm_EEPROM_storage+3*ala,clock1.alarms[ala].hr); // Alarm hour
  delay(100);

  temp1=hmi_with_update_2(clock1.alarms[ala].mnt, 0, 59, 1, 7, 1, 2, render_00number_in_place);
  clock1.alarms[ala].mnt=(temp1==-1)?clock1.alarms[ala].mnt:temp1; // In case escape was triggered, value doesn't change.
  EEPROM.write(alarm_EEPROM_storage+3*ala+1,clock1.alarms[ala].mnt); // Alarm minute
  delay(100);
}

int hmi_with_update_2(int current, int lower, int upper, byte inc, byte column, byte row, byte space, void (*update_function)(int))
/*
HMI with one update function and value wrap-around capability.
Function traps until the update is finalized by the confirm button or forfeited by the esc button.
Returns updated value when enter is pressed. Returns -1 if escape is pressed.
*/
{
  int number=current;
  byte temp1;
  char msg[space+1];
  for (byte i=0;i<space;i++) msg[i]=' '; // Create mask the size of the output space.
  msg[space]=0;
  lcd.setCursor(column,row); // Write mask to erase previous content.
  lcd.print(msg);
  lcd.setCursor(column,row); // Write initial value.
  (*update_function)(number);
  lcd.setCursor(column,row); // Write initial value.
  lcd.cursor();
//  lcd.blink(); // Blinking makes it too hard to read.
  while(true)
  {
    temp1=wait_on_escape(50);
    switch (temp1)
    {
      case 1:
      if (number+inc<=upper) number+=inc;
      else number=lower;
      lcd.setCursor(column,row);
      lcd.print(msg);
      lcd.setCursor(column,row);
      (*update_function)(number);
      lcd.setCursor(column,row);
      break;
      
      case 2:
      if (number-inc>=lower) number-=inc;
      else number=upper;
      lcd.setCursor(column,row);
      lcd.print(msg);
      lcd.setCursor(column,row);
      (*update_function)(number);
      lcd.setCursor(column,row);
      break;
      
      case 4:
      case 5:
      case 6:
      lcd.noCursor();
      return(number);
      break;
      
      default:
      break;
    }
  }
}

