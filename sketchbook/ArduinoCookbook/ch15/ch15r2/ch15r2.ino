#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include "Dhcp.h"         // add this for the DHCP library

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
                                   // IP addressing lines removed
byte server[] = { 209,85,229,104  }; // Google

Client client(server, 80);

void setup()
{
  Serial.begin(9600);
  if(Dhcp.beginWithDHCP(mac) == 1)   // begin method returns 1 if successful
  {
    Serial.println("got IP address, connecting...");
    delay(5000);
  }
  else
  {
    Serial.println("unable to acquire ip address!");
    while(true)
       ;  // do nothing
  }

  if (client.connect()) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
      
    
