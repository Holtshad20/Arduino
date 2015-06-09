/*
 * WebServerParsing
 
 Responde as requisições da URL para alterar as portas Analógicas e Digitais 
 e mostrar os valores lido nas portas Analógicas
 Por exemplo, enviando http://192.168.1.177/?pinD2=1 pelo browser coloca o pin 2 em ON
              enviando http://192.168.1.177/?pinD2=0 coloca o pin 2 em OFF
  
 http://arduserver.sytes.net/?pinD8=1&pinD7=0&pinA0=45 ; Digital 7 e 8 HIGH, e A0 com 45.
              
*/
#include <SPI.h> 
#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0B, 0x00, 0x26 }; // endereço MAC
IPAddress ip(192,168,1,100); // endereço IP do EthernetShield na rede

EthernetServer server(80); // declarar o Server
EthernetClient client; // declarar Client

void setup()
{
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip); // inicializar Ethernet
  server.begin(); // inicializar Server
  
  Serial.println("ready");  
}

void loop()
{
  client = server.available();
  if (client) {
    TextFinder  finder(client );
    while (client.connected()) {
      if (client.available()) {
        // counters to show the number of pin change requests
        int digitalRequests = 0;  
        int analogRequests = 0;
        if( finder.find("GET /") ) {
          // procurar tokens iniciando por "pin" e terminando com NL/LF
          while(finder.findUntil("pin", "\n\r")){
            char type = client.read(); // D or A
            int pin = finder.getValue();
            int val = finder.getValue();
            if( type == 'D') {
              Serial.print("Digital pin ");
              pinMode(pin, OUTPUT);
              digitalWrite(pin, val);
              digitalRequests++;
            }
            else if( type == 'A'){
              Serial.print("Analog pin ");
              analogWrite(pin, val);
              analogRequests++;
            }
            else {
              Serial.print("Unexpected type ");
              Serial.print(type);
            }
            Serial.print(pin);
            Serial.print("=");
            Serial.println(val);
          }
        }
        Serial.println();
        // chegou ao final dos comandos no browser, mostrará a situação 
        // das portas analógicas e digitais
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        // output the number of pins handled by the request
        client.print(digitalRequests);
        client.print(" digital pin(s) written");
        client.println("<br />");
        client.print(analogRequests);
        client.print("analog pin(s) written");
        client.println("<br />");
        client.println("<br />");
        // mostrar valor dos pinos analógicos
        client.println("<h1>Analog Pins</h1>");        
        for (int i = 0; i < 6; i++) {
          client.print("Analog input ");
          client.print(i);
          client.print(" is ");
          client.print(analogRead(i));
          client.println("<br />");
        }
        // mostrar a situação dos pinos digitais
        client.println("<h1>Digital Pins</h1>");
        // mostrar situação dos pinos digitais
        for (int i = 2; i < 10; i++) {
          //pinMode(i, INPUT);
          client.print("digital pin ");
          client.print(i);
          client.print(" is ");
          if(digitalRead(i) == LOW)
            client.print("LOW");
          else
            client.print("HIGH");
          client.println("<br />");
        }
       break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}
      
    
