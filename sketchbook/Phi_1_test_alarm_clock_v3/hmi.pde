/*
Programmed by Dr. John Liu
Revision: 11/19/2010
Commerical use without authorization is prohibited.
Find details of the Phi-1 shield or contact Dr. Liu at http://liudr.wordpress.com
All rights reserved.
*/
void render_RTC(int temp)
{
  int rtc[7];
  char msg[17];
  RTC.get(rtc,true);
  lcd.clear();
  switch(temp)
  {
    case 0:
    render_month_in_place(rtc[5]-1);//  render_00number_in_place(rtc[5]);
    sprintf(msg,"/%02d/%4d ",rtc[4],rtc[6]);//  lcd.print("JAN/01/2000 SUN");
    lcd.print(msg);
    render_DOW_in_place(rtc[3]-1);//  render_00number_in_place(rtc[3]);
    lcd.setCursor(4,1);
    sprintf(msg,"%2d:%02d:%02d",rtc[2],rtc[1],rtc[0]);
    lcd.print(msg);
    break;
    
    case 1:
    lcd.setCursor(4,1);
    sprintf(msg,"%2d:%02d:%02d",rtc[2],rtc[1],rtc[0]);
    lcd.print(msg);
    break;
    
    default:
    break;
  }
}

void render_DOW_in_place(int number)
{
  msg_lcd(dow_00+(number%8)*4); // *4 because DOW is 3-character+ zero
}

void render_alarm_in_place(int number)
{
  msg_lcd(alarm_00+(number%5)*8); // *4 because alarm is 7-character+ zero
}

void render_month_in_place(int number)
{
  msg_lcd(month_00+(number%12)*4); // *4 because month is 3-character+ zero
}

void render_number_in_place(int number)
{
  char msg[11];
  sprintf(msg,"%d",number);
  lcd.print(msg);
}

void render_00number_in_place(int number)
{
  char msg[11];
  sprintf(msg,"%02d",number);
  lcd.print(msg);
}

int wait_on_escape(int ref_time) // Returns key pressed or 0 if time expires before any key was pressed.
{
//Wait on button push.
  long temp0;
  byte temp1, temp2, temp3, temp4, temp5, temp6;
  temp0=millis();
  do
  {
    temp1=btn_1.sense();
    temp2=btn_2.sense();
    temp3=btn_3.sense();
    temp4=btn_4.sense();
    temp5=btn_5.sense();
    temp6=btn_6.sense();

    if((temp1==buttons_released)||(temp1==buttons_held))
    {
      return(1);
    }
    if((temp2==buttons_released)||(temp2==buttons_held))
    {
      return(2);
    }
    if((temp3==buttons_released)||(temp3==buttons_held))
    {
      return(3);
    }
    if ((temp4==buttons_released)||(temp4==buttons_held))
    {
      return(4);
    }
    if((temp5==buttons_released)||(temp5==buttons_held))
    {
      return(5);
    }
    if((temp6==buttons_released)||(temp6==buttons_held))
    {
      return(6);
    }
  }   while ((millis()-temp0<ref_time));

  return (0);
}


