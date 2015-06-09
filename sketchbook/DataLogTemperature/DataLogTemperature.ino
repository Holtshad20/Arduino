/*

  OBJETIVO:

  Obter a temperatura de um LM35D (sensor analogico de temperatura);
  Obter a Data e Hora de um DS1703 (Relogio em Tempo Real - RTC);
  Gravar log num arquivo texto em cartao SD usando um modulo SD Card
  
  O log sera gravado num arquivo texto (DATALOG.TXT), que devera conter o seguinte formato: 

   DD.MM.YYYY HH:MM:SS 99.99 - Data, Hora e Temperatura em graus Celsius 

  Uma linha sera incluida no arquivo de logo a cada minuto.  
 
  O CIRCUITO: 
 
  ARDUINO UNO R3
  Modulo SD CARD
  Modulo RTC (DS1307)+ Temperatura (LM35)
 
   
 
   
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

/*******************************************************
* Example how to use LM35 Temperature Sensor for Arduino 
* in a full-range schema
* You can see the schema or datasheet or at link:
* http://www.ik0jre.net
* Author IK0JRE - Giuseppe Aquilani
* ik0jre@ik0jre.net
* You can use and modify this software under GPL licence 
* and I hope you write my name too in your software.
* We can measure celsius degree in full range scale 
* from -500 mV to 1500 mV 
* for these schema we need read a virtual ground or
* subtract a fixed value of 0.9 mV from analog read.
* For tollerance of electronic components I preferr
* read virtual ground value
* NOTE: from National datasheet in the full range schema
* the limits are
* LM35/LM35A     -55 +150 C
* LM35C/LM35CA   -40 +110 C
* LM35D            0 +100 C
* I tested schema and software with LM35DZ and it works
* under -10 and up +120
*******************************************************/

/*

   Pinagem do Modulo RC + Temperatura 
   
   Placa vista de cima, com conectores na parte superior
   
   1 - A0 - LM35 - e tomado como base duas referencias para a leitura do LM35
   2 - A1 - LM35
   3 - GND
   4 - +5V
   5 - NC
   6 - D5 - DS1307 - SCL
   7 - D4 - DS1307 - SDA
   
   
*/


#include <SPI.h>
#include <SD.h>
#include <DS1307.h>

// Definir o relogio 
DS1307 rtc(4,5);

//ATMEGA328 analog resolution is Vcc/1024
//#define FFACT ((float) 5 / 1024) //analog resolution at +5V

#define MAX_AV_READ  500        //n. read for average
const float SUPPLY_VOLTAGE = 500; // 5V * 100 (LM35 costant)

//#define tA0  0      //Arduino analog input 0 connect to +out pin
//#define tA1  1      //Arduino analog input 1 connect to -out pin
float tC0;          //A0 read temp var
float tC1;          //A1 read temp var
float tCalc;        //var need for calc celsius degrees
#define FCORR 3.3   // only for test if we don't have precision read
                    //because supply voltage is not exactly +5v
static char s[6];   // Usado para converter floats em strings



void Ajustar() {
  //Aciona o relogio
  rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  rtc.setDOW(SATURDAY);      //Define o dia da semana
  rtc.setTime(14, 41, 0);     //Define o horario
  rtc.setDate(31, 1, 2015);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
}



void setup()
{
  
  //Ajustar();
  
  analogReference(DEFAULT);  // set voltage reference at default 5.0 V for Arduino 1   
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";

  int i;
  // because at328 analog read is unstable
  // we neeed an average from many read
  // I found in 500 read a good solution between precision
  // and speed
  // in debug mode you can apply a multimeter at 
  // pin +out -out in schema for verify mV read from Arduino 
 
  for(i=0;i<MAX_AV_READ;i++){
     tC0 += analogRead(A0) ;    //read +out sensor pin
     delay(5);                   //atmega need 10uS delay but we preferr 5mS 
     tC1 += analogRead(A1) ;    //for full range scale -55 to + 155 C
  }                           //must read -out pin (virtual Vss)
    
  tC0 /=MAX_AV_READ;   //average
  tC1 /=MAX_AV_READ;   //average

  tCalc = tC0 - tC1 ;  //subtract virtual ground value
  // now we can calc celsius degrees 
  //tCalc = ( tCalc * SUPPLY_VOLTAGE /1024) - FCORR; 
  tCalc = (((tCalc / 1024) * 5) * 100) - FCORR;
  //only for debug next three line show millVolt out from LM35
  //Serial.print(tC0);           //show mV reads at A0 input
  //Serial.print(" ");           //print space
  //Serial.print(tC1);           //show mV reads at A1 input

  //next show temperature in Celsius scale
  //Serial.print(" ");
  //Serial.print(tCalc);
  //Serial.println(" C ");  

  //dtostrf(tC0,3,2,s);  
  //dataString += s;
  //dataString += " - ";
  //dtostrf(tC1,3,2,s);  
  //dataString += s;
  //dataString += " - ";
  
  dataString += rtc.getDateStr();
  dataString += " ";  
  
 // dataString += rtc.getDOWStr();
 // dataString += " - ";
  
  dataString += rtc.getTimeStr();
  dataString += " ";
  
  dtostrf(tCalc,3,2,s);    
  dataString += s;
  //dataString += " C";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
  
  delay(60000); // faz a leitura aproximadamente a cada 60 segundos
}









