#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <Twitter.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };
byte gateway[] = { 192, 168, 1, 254 };
byte subnet[] = { 255, 255, 255, 0 };

Twitter twitter("YourID:Password");

char frontOpen[] = "The front door was opened";
char backOpen[] = "The back door was opened";

const int frontSensor = 2;
const int backSensor  = 3;

boolean frontMsgSent = false;
boolean backMsgSent = false;

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);

  pinMode(frontSensor, INPUT);
  pinMode(backSensor, INPUT);
  digitalWrite(frontSensor, HIGH);  // pull-ups
  digitalWrite(backSensor, HIGH);

  delay(1000);
}

void loop()
{
  if(digitalRead(frontSensor) == LOW)
  { // here if door is open
    if(frontMsgSent == false){ // check if message already  sent
       frontMsgSent = sendMessage(frontOpen);
    }
  }
  else{
      frontMsgSent = false;  // door closed so reset the state
  }
  if(digitalRead(backSensor) == LOW)
  {
    if(frontMsgSent == false) {
       backMsgSent = sendMessage(backOpen);
    }
  }
  else {
      backMsgSent = false;
  }
  delay(100);
}
// add in the sendMessage function from the sketch above
      
    
