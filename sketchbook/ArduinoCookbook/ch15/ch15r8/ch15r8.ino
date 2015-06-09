/*
 * WebServerMultiPage
 *
 * Respond to requests in the URL to view digital and analog output ports
 *
 * http://192.168.1.177/analog/   displays analog pin data
 * http://192.168.1.177/digital/  displays digital pin data
*/
#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] =  { 192,168,1,177 };

char buffer[8]; // make this buffer big enough to hold requested page names

Server server(80);

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println("Ready");
}

void loop()
{
  Client client = server.available();
  if (client) {
    TextFinder  finder(client );
    while (client.connected()) {
      if (client.available()) {
        if( finder.find("GET ") ) {
          // look for the page name
          if(finder.getString( "/", "/", buffer, sizeof(buffer) )) 
          {
            if(strcmp(buffer, "analog") == 0)
              showAnalog(client);
            else if(strcmp(buffer, "digital") == 0)
              showDigital(client);
            else
              unknownPage(client, buffer);
          }
        }
        Serial.println();
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}

void showAnalog(Client client)
{
  Serial.println("analog");
  sendHeader(client);
  client.println("<h1>Analog Pins</h1>");
  // output the value of each analog input pin

  for (int i = 0; i < 6; i++) {
    client.print("analog pin ");
    client.print(i);
    client.print(" = ");
    client.print(analogRead(i));
    client.println("<br />");
  }
}

void showDigital(Client client)
{
  Serial.println("digital");
  sendHeader(client);
  client.println("<h1>Digital Pins</h1>");
  // show the value of digital pins
  for (int i = 2; i < 8; i++) {
    pinMode(i, INPUT);
    client.print("digital pin ");
    client.print(i);
    client.print(" is ");
    if(digitalRead(i) == LOW)
      client.print("LOW");
    else
      client.print("HIGH");
    client.println("<br />");
  }
  client.println("</body></html>");
}

void unknownPage(Client client, char *page)
{
  Serial.print("Unknown : ");
  Serial.println("page");

  sendHeader(client);
  client.println("<h1>Unknown Page</h1>");
  client.println(page);
  client.println("</body></html>");
}

void sendHeader(Client client)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<html><head><title>Web server multi-page Example</title>");
  client.println("<body>");
}
      
    
