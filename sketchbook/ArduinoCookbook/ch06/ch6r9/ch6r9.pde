/*
 RFID sketch
 Displays the value read from an RFID tag
 */

const int startByte   = 10;  // ASCII line feed precedes each tag
const int endByte    = 13;   // ASCII carriage return terminates each tag
const int tagLength   = 10;  // the number of digits in tag
const int totalLength = tagLength + 2; //tag length + start and end bytes

char tag[tagLength + 1];  // holds the tag and a terminating null

int bytesread = 0;

void setup()
{
  Serial.begin(2400);    // set this to the baud rate of your RFID reader
  pinMode(2,OUTPUT);     // connected to the RFID ENABLE pin
  digitalWrite(2, LOW);  // enable the RFID reader
}

void loop()
{
  if(Serial.available() >= totalLength)  // check if there's enough data
  {
    if(Serial.read() == startByte)
    {
      bytesread = 0;                // start of tag so reset count to 0
      while(bytesread < tagLength)  // read 10 digit code
      {
        int val = Serial.read();
        if((val == startByte)||(val == endByte))  // check for end of code
          break;
        tag[bytesread] = val;
         bytesread = bytesread + 1; // ready to read next digit
      }
      if( Serial.read() == endByte)  // check for the correct end character
      {
        tag[bytesread] = 0; // terminate the string
        Serial.print("RFID tag is: ");
        Serial.println(tag);
      }
    }
  }
}
      
    
