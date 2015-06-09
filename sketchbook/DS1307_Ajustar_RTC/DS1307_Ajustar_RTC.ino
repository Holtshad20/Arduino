/*

  Ajustar e Cobnferir o mdulo  RTC 

   Pinagem do Modulo RC + Temperatura 
   
   Placa vista de cima, com conectores na parte superior (bateria na parte de baixo)
   
   1 - A0 - LM35 - e tomado como base duas referencias para a leitura do LM35
   2 - A1 - LM35
   3 - GND
   4 - +5V
   5 - NC
   6 - D5 - DS1307 - SCL
   7 - D4 - DS1307 - SDA
   
*/
#include <DS1307.h>

// Init the DS1307
DS1307 rtc(A4, A5);


void Ajustar() {
  //Aciona o relogio
  rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  rtc.setDOW(THURSDAY);      //Define o dia da semana
  rtc.setTime(17, 44, 0);     //Define o horario
  rtc.setDate(25, 2, 2015);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
}


void setup()
{
  //Ajustar();
  
  Serial.begin(9600);

}

void loop()
{
  String dataString = "";

  dataString += rtc.getDateStr();
  dataString += " ";  
  
  String s = rtc.getDOWStr();
  dataString += s.substring(0,3);
  dataString += " - ";
  
  dataString += rtc.getTimeStr();
  dataString += " ";
  
  Serial.println(dataString);
  delay(1000); // faz a leitura aproximadamente a cada 60 segundos
}

