// DS1307_LCD (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// A quick demo of how to use my DS1307-library to make a quick
// clock using a DS1307 and a 16x2 LCD.
//
// I assume you know how to connect the DS1307 and LCD.
// DS1302:  SDA pin   -> Arduino Digital 4
//          SCL pin   -> Arduino Digital 5
// LCD:     DB7       -> Arduino Digital 6
//          DB6       -> Arduino Digital 7
//          DB5       -> Arduino Digital 8
//          DB4       -> Arduino Digital 9
//          E         -> Arduino Digital 10
//          RS        -> Arduino Digital 11

#include <LiquidCrystal595.h>
#include <DS1307.h>

// Init the DS1307
DS1307 rtc(A4, A5);

// Init the LCD
LiquidCrystal595 lcd(2,3,4);

void setup()
{
  // Set the clock to run-mode
  rtc.halt(false);
  
  // Setup LCD to 16x2 characters
  lcd.begin(16, 2);

}

void loop()
{
  // Display time centered on the upper line
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
  
  // Display abbreviated Day-of-Week in the lower left corner
  lcd.setCursor(0, 1);
  String s = rtc.getDOWStr();
  lcd.print(s.substring(0,3));
  
  // Display date in the lower right corner
  lcd.setCursor(6, 1);
  lcd.print(rtc.getDateStr());

  // Wait one second before repeating :)
  delay (1000);
}
