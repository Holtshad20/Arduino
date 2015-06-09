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


