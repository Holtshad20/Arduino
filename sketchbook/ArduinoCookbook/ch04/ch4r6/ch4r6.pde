/*
 * SendBinary sketch
 * Sends a header followed by two random integer values as binary data.
*/

int intValue;    // an integer value (16 bits)

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print('H'); // send a header character

  // send a random integer
  intValue = random(599); // generate a random number between 0 and 599
  // send the two bytes that comprise an integer
  Serial.print(lowByte(intValue), BYTE);  // send the low byte
  Serial.print(highByte(intValue), BYTE); // send the high byte

  // send another random integer
  intValue = random(599); // generate a random number between 0 and 599
  // send the two bytes that comprise an integer
  Serial.print(lowByte(intValue), BYTE);  // send the low byte
  Serial.print(highByte(intValue), BYTE); // send the high byte

  delay(1000);
}
      
    
