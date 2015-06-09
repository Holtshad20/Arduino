/*
 * WebClientGoogleFinance sketch
 * get the stock value for google and write to analog pin.
 */
#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {  192, 168,1,177 };
byte gateway[] ={ 192, 168, 1, 254 };
byte subnet[] ={ 255, 255, 255, 0 };

byte server[] = {209,85,229,147 };  // google
Client client(server, 80);

TextFinder  finder( client );
float value;

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  delay(2000);
}

void loop()
{
  Serial.print("Connecting...");
  if (client.connect()) {
    client.println("GET //finance?q=google HTTP/1.0"); // todo
    client.println("User-Agent: AVR ethernet");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
  }
  if (client.connected()) {
     if(finder.find("<span class=\"pr\">"))
     {
       finder.find(">");  // seek past the next '>'
       value = finder.getFloat();
       Serial.println(value);
       analogWrite(0, value);
     }
    else
      Serial.print("Could not find field");
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
  delay(5000); // 5 seconds between each connect attempt
}
      
    
