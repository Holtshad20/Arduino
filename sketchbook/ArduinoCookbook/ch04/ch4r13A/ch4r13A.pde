/*
 * NewSoftSerialOutput sketch
 * Output data to a software serial port
 */

#include <NewSoftSerial.h>

const int rxpin = 2;           // pin used to receive from LCD
const int txpin = 3;           // pin used to send to LCD
NewSoftSerial serial_lcd(rxpin, txpin); // new serial port on pins 2 and 3

void setup()
{
  Serial.begin(9600); // 9600 baud for the built-in serial port
  serial_lcd.begin(9600); //initialize the software serial port also for 9600
}

int number = 0;

void loop()
{
  serial_lcd.print("The number is ");  // send text to the LCD
  serial_lcd.println(number);    // print the number on the LCD
  Serial.print("The number is ");
  Serial.println(number);        // print the number on the PC console

  delay(500); // delay half second between numbers
  number++;   // to the next number
}
      
    
