// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  lcd.begin(16, 2);  
  lcd.print("Recebendo ...");
}

void loop()
{
  delay(10);
  
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(Wire.available())
  {
    byte c = Wire.read();
    Serial.print(c);
    Serial.print(' ');
    Serial.print(c, HEX);
    Serial.print(' ');
    Serial.print(c, BIN);
  }; 
  delay(10);
  Serial.println();  
}
