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

boolean MsgSent = false;
const int Sensor = 2;

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);

  pinMode(Sensor, INPUT);
  digitalWrite(Sensor, HIGH);  //turn on  pull-up resistors
  delay(1000);
}

void loop()
{
  if(digitalRead(Sensor) == LOW)
  { // here if mailbox is open
    if(MsgSent == false){ // check if message already sent
       MsgSent = sendMessage("Mail has been delivered");
    }
  }
  else{
      MsgSent = false;  // door closed so reset the state
  }
  delay(100);
}

boolean sendMessage( char *message)
{
boolean isSent = false;

  Serial.println("connecting ...");
  if (twitter.post(message)) {
    int status = twitter.wait();
    if (status == 200) {
      Serial.println("OK.");
      isSent = true;
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
  delay(100);
  return isSent;
}
      
    
