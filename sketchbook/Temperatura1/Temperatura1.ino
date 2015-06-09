/*
  Ler a temperatura do sensor LM35, na porta A0 e mostrar no display
  em °C e F
*/

#include <LiquidCrystal595.h>
#include <SevenDisp.h>

#define tempPin  0  // sensor de temperatura pin analog

/* 2 - datapin(14) 3 - latchpin(12) 4 - clockpin(11) */
LiquidCrystal595 lcd(5,6,7); 
SevenDisp Seven(2,3,4); // pin 5 = data, 6 = latch, 7 = clock

#define R_LOAD 5.5

//declare variables
int reading;
float V, C, F;

void setup()
{
  analogReference(INTERNAL);  
  lcd.begin(16,2);
}

void loop()
{
  int val = analogRead(tempPin);
  float voltagetemp = (val) * 5;
  voltagetemp /= 1024.0;
  C = (voltagetemp - 0.5) * 10 ;   
  F = ((C * 1.8) + 32); // convert °C para °F

  lcd.setCursor(0,0);
  lcd.print("Temperatura=====");
  lcd.setCursor(0,1);
  lcd.print(C);
  lcd.setCursor(5,1);
  lcd.print("C"); 

  lcd.setCursor(10,1);
  lcd.print(F);
  lcd.setCursor(15,1);
  lcd.print("F"); 
 
  Seven.Show(C);
  delay(5000);    
}

