#include <LiquidCrystal.h>

// inicializar display
LiquidCrystal lcd(12,11,5,4,3,2);

void setup() {
  lcd.begin(16,2); // definir col x lin do display
  lcd.print("Start ...");
  delay(1000);
  lcd.clear();
  MostraCaracteres();
}

void MostraCaracteres() {
  for (int x=0; x<16; x++) {
    
  }
  for (int i=0; i<255; i++) {
    lcd.write(i); 
    delay(500);
    lcd.scrollDisplayLeft();
    }
}

void loop() {
}

