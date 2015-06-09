/*
  Versão: 1.00
  Mostrar Data, Hora, Mês e Dia da Semana no display LCD(16,2)
  Formato: 0123456789111111
                     012345
           ----------------
           dd/mm/yy     mmm 
           HH:MM:SS     ddd

  onde: dd = dia 
        mm = mês numérico
        yyyy = ano
        mmm = (Jan, Fev, Mar, Abr, Mai, Jun, Jul, Ago, Set, Out, Nov, Dez)
        ddd = (Seg, Ter, Qua, Qui, Sex, Sab, Dom)
         
*/

#include <LiquidCrystal595.h>
#include <DS1307RTC.h>
#include <Wire.h>
#include <Time.h>

#define TIME_DELAY 1000 // tempo para refresh do display lcd
#define TIME_PRESS 500 // tempo 
#define SET 8 // botão de Ajuste D8

// inicializar display
LiquidCrystal595 lcd(2,3,4);
tmElements_t tm;

unsigned long pressTime;
int lastTime = 0;
byte dia = 0;
boolean stateSet;
boolean stateCursor =  false;
int blinkcursor = 0;

String meses[12] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
String dias_semana[7] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

void setup() {
  Serial.begin(9600);
  
  pinMode(SET, INPUT);
  lcd.begin(16,2); // definir col x lin do display
  lcd.noCursor();
  
  lastTime = millis();
  blinkcursor = millis();    
}

void loop() {
  
  if (RTC.read(tm)) {
    
    // refresh no display quando mudar de dia
    if (tm.Day != dia) {
       dia = tm.Day; // guarda ultimo dia
       showDate();
       showMonth();
       showTime();
       showDiaSemana();
    };
    
  } else {
    if (RTC.chipPresent()) {
      Serial.println("DS1307 nao esta conectado!");
    } else {
      Serial.println("DS1307 erro de leitura!");
    }
    delay(10000);
  }
  
  // mostrar a hora a cada segundo
  if ((millis() - lastTime) > TIME_DELAY) {
     showTime();
     lastTime = millis();
  };
  
  
  // acionado botão de Ajuste ?
  stateSet = digitalRead(SET);
  
  if (stateSet == HIGH && (millis() - pressTime) > TIME_PRESS) {          
     lcd.setCursor(0,0);  
     lcd.cursor();

     stateSet = LOW;
     pressTime = millis();
  };
  
}



void showDate(){
  /* Mostrar o calendario no LCD */
  lcd.setCursor(0,0);
  lcd.print(fmtdigits(tm.Day) + '/' + fmtdigits(tm.Month) + '/' + fmtdigits(tmYearToCalendar(tm.Year)));  
}

void showTime() {
  /* mostrar Horário no display LCD */
  lcd.setCursor(0,1);
  lcd.print(fmtdigits(tm.Hour) + ":" + fmtdigits(tm.Minute) + ":" + fmtdigits(tm.Second));
}

void showMonth() {
  /* mostrar Mes [AAA] no display  */
  lcd.setCursor(13,0);
  lcd.print(meses[tm.Month-1]);
}

void showDiaSemana() {
  lcd.setCursor(13,1);
  lcd.print(dias_semana[tm.Wday-1]);

}


String fmtdigits(int number) {
  /* inserir zero a esquerda de numeros < 10 */
  String s = "";  
  
  if (number >= 0 && number < 10) { 
     s = s + '0';
  }
  s = s + String(number);
  
  return(s);
}

