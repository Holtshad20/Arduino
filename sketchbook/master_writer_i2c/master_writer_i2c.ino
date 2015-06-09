#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus
  pinMode(13, OUTPUT); 
}

void loop()
{
  for (byte b=0; b<256; b++) {
    Wire.beginTransmission(4);
    Wire.write(b);
    Wire.endTransmission();
    delay(500);
    digitalWrite(13, HIGH);  
    delay(500);
    digitalWrite(13, LOW);  
  }
}
