#include <Wire.h>

/*
  Trasmissor I2C Master
*/

#define DEV 4

void setup()
{
  Wire.begin(); // join i2c bus
  pinMode(13, OUTPUT); 
}

void loop()
{

    Wire.beginTransmission(DEV);
    Wire.write("012");
    Wire.endTransmission();
    delay(500);
    digitalWrite(13, HIGH);  
    delay(500);
    digitalWrite(13, LOW);  

}
