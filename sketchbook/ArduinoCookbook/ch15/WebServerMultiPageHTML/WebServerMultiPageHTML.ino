/*
 * WebServerMultiPageHTML
 *
 * Display analog and digital pin values using HTML formatting
*/
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0B, 0x00, 0x26 }; // endereço MAC
IPAddress ip(192,168,1,100); // endereço IP do EthernetShield na rede

EthernetServer server(80); // declarar o Server
EthernetClient client; // declarar Client
char buffer[8]; // make this buffer big enough to hold requested page names

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(13,OUTPUT);
  for(int i=0; i < 3; i++)
  {
     digitalWrite(13,HIGH);
     delay(500);
     digitalWrite(13,LOW);
     delay(500);
  }
}

void loop()
{
  EthernetClient client = server.available();
  if (client) {
    TextFinder  finder(client );
    while (client.connected()) {
      if (client.available()) {
        if( finder.find("GET ") ) {
          // look for the page name
          if(finder.getString( "/", "/", buffer, sizeof(buffer) )) 
          {
            if(strcasecmp(buffer, "analog") == 0)
              showAnalog(client);
            else if(strcasecmp(buffer, "digital") == 0)
              showDigital(client);
            else if(strcmp(buffer, "private")== 0)
              showPrivate(client, finder);
            else
              unknownPage(client, buffer);
          }
        }
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}

void showAnalog(EthernetClient client)
{
  sendHeader(client,"Multi-page: Analog");
  client.println("<h2>Analog Pins</h2>");
  client.println("<table border='1' >");
  for (int i = 0; i < 6; i++) {
    // output the value of each analog input pin
    client.print("<tr><td>analog pin ");
    client.print(i);
    client.print(" </td><td>");
    client.print(analogRead(i));
    client.println("</td></tr>");
  }
  client.println("</table>");
  client.println("</body></html>");
}

void showDigital(EthernetClient client)
{
  sendHeader(client,"Multi-page: Digital");
  client.println("<h2>Digital Pins</h2>");
  client.println("<table border='1'>");
  for (int i = 2; i < 8; i++) {
    // show the value of digital pins
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // turn on pull-ups
    client.print("<tr><td>digital pin ");
    client.print(i);
    client.print(" </td><td>");
    if(digitalRead(i) == LOW)
      client.print("Low");
    else
      client.print("High");
    client.println("</td></tr>");
  }
  client.println("</table>");
  client.println("</body></html>");
}

void showPrivate(EthernetClient client, TextFinder finder)
{
  sendHeader(client,"Multi-page: Private");
  // find tokens starting with "pin" and stop on the first blank line
  while(finder.findUntil("pin", "\n\r")){
    char type = client.read(); // D or A
    int pin = finder.getValue();
    int val = finder.getValue();
    if( type == 'D') {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, val);

    }
    else if( type == 'A'){
      analogWrite(pin, val);
    }
    else {
     client.print("Unexpected type ");
     client.println(type);
    }
  }
}

void unknownPage(EthernetClient client, char *page)
{
  sendHeader(client, "Unknown Page");
  client.println("<h1>Unknown Page</h1>");
  client.println("Recognized pages are:<br />");
  client.println("/analog/<br />");
  client.println("/digital/<br />");
  client.println("/private/<br />");  
  client.println("</body></html>");
}

void sendHeader(EthernetClient client, char *title)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.print("<html><head><title>");
  client.println(title);
  client.println("</title><body>");
}
      
    
