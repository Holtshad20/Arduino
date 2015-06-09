/*
 * WebClientDNS sketch
 */

#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include "Ethernet_dns.h"
#include "Dns.h"   // uses DNS library from Matt Robertson

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192, 168, 1, 177 };    // change to a valid address for your network


byte gateway[] ={192, 168, 1, 254 };  // add this if you use a router or gateway
// see text for more on IP addressing
byte subnet[] ={255, 255, 255, 0 };   // this defines the subnet address


byte ipBuffer[6];  // this will get the server IP address from DNS

Client client(ipBuffer, 80);
DnsClass Dns;

//Client client(ipBuffer, server, 80);

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);

  //// Do DNS Lookup
  Serial.println("getting server address");
  Dns.init("google.com", ipBuffer);  //Buffer has IP address of the DNS server
  Dns.resolve();

  int results;
  while(!(results=Dns.finished())) ;  //wait for DNS to resolve the name

  if(results != 1){
    Serial.print("DNS Error code: ");
    Serial.print(results,DEC);
    while(true)
      ;  // do nothing
  }

  delay(5000);


  if (client.connect()) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  }
  else {
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
      
    
