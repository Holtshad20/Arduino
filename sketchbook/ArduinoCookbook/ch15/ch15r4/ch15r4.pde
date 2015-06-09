/*
 * SimpleClientwFinder sketch
 *
 */

#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,1,177 };
//byte gateway[] ={ 192, 168, 1, 254 };
//byte subnet[] ={ 255, 255, 255, 0 };

byte server[] = { 173,194,33,104 }; // Google

Client client(server, 80);
TextFinder finder( client);

int result; // the result of the google calculation

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  delay(2000);
  Serial.println("connecting...");

}

void loop()
{
  if (client.connect()) {
    Serial.print("connected... ");
    client.println("GET /search?hl=en&q=what+is+50+km+in+mi HTTP/1.1");
    client.println();
  } else {
    Serial.println("connection failed");
  }
  if (client.connected()) {
    if(finder.find("<b>50 kilometers")){
      if(finder.find("=")){
         result = finder.getValue();
         Serial.println(result);
      }
    }
    else
       Serial.println("result not found");
    client.stop();
    delay(10000); // check again in 10 seconds
  }
  else {
    Serial.println();
    Serial.println("not connected");
    client.stop();
    delay(1000);
  }
}
      
    
