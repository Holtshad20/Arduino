#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <LiquidCrystal595.h>

LiquidCrystal595 lcd(2,3,4);

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  lcd.begin(16,2);
  delay(200);
  lcd.setCursor(0,0);
  lcd.print("DS1307RTC");
  lcd.setCursor(0,1);
  lcd.print("---------");
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    lcd.clear();
    lcd.setCursor(4,0);    
    lcd.print(fmtdigits(tm.Hour) + ":" + fmtdigits(tm.Minute) + ":" + fmtdigits(tm.Second));

    lcd.setCursor(3,1);
    lcd.print(fmtdigits(tm.Day) + '/' + fmtdigits(tm.Month) + '/' + fmtdigits(tmYearToCalendar(tm.Year)));
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

String fmtdigits(int number) {
  
  String s = "";  
  
  if (number >= 0 && number < 10) { 
     s = s + '0';
  }
  s = s + String(number);
  
  return(s);
}
