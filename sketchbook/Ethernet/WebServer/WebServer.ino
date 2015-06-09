/*
  Web Server
 
 
*/
#include <SPI.h>
#include <Ethernet.h>
// MAC Adress e IP Adress da Rede pro Ethernet Shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0B, 0x00, 0x26 };
IPAddress ip(192,168,1,100);

EthernetServer server(80); // Inicializa Server na porta 80

#define tempPin  0  // sensor de temperatura pin analog
int temp;
float C, F;



void setup()
{
  Serial.begin(9600); 

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    String s;
    boolean led;    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        s.concat(c);
        if (s.endsWith("/ledon")) led = true; 
        if (s.endsWith("/ledoff")) led = false;
      
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");         
          client.println();
          client.println("ARDUINO WEB SERVER ON LINE"); 
          client.println("<br />");
          client.println("Temperatura ambiente em Curitiba: "); 
          client.println("<br />");
          
          if (led) { 
            digitalWrite(13, HIGH) ;
             client.println("Led Ligado");
          }
          if (~led) {
           digitalWrite(13, LOW); 
           client.println("Led Desligado");
          }
      
         
          client.println();
          // obter dados do sensor de temperatura 
          temp = analogRead(tempPin);
          // converter para Celsius e Faranheit 
          C = (5.0 * temp * 100.0)/1024.0;  //convert the analog data to temperature  
          F = ((C * 1.8) + 32); 

          client.print("C = ");
          client.println(C);
          client.println("<br />");          
          client.print("F = ");
          client.println(F);
          client.println("<br />");          
          client.println("======================================================");
          client.println("<br />");          
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
