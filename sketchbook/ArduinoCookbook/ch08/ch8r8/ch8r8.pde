/*
 * SimpleBrushed sketch
 * commands from serial port control motor speed
 * digits '0' through '9' are valid where '0' is off, '9' is max speed
 */

const int motorPins = 3; // motor driver is connected to pin 3

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if ( Serial.available()) {
    char ch = Serial.read();

    if(ch >= '0' && ch <= '9')              // is ch a number?
    {
      int speed = map(ch, '0', '9', 0, 255);
      analogWrite(3, speed);
      Serial.println(speed);
    }
    else
    {
      Serial.print("Unexpected character ");
      Serial.println(ch);
    }
  }
}
      
    
