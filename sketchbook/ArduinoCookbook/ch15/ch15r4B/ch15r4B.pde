/*
 * WebClient_Babelfish sketch
 * Uses Post to get data from a web server
 *
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

byte server[] = {66,196,80,202 }; //babelfish.yahoo.com

Client client(server, 80);

// the text to translate
char * transText = "trtext=Ciao+mondo+da+Arduino.&lp=it_en";

char buffer [31];  // room for 30 characters and the terminating null

TextFinder  finder(client);

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  delay(3000);
}

void loop()
{
  postPage( "/translate_txt", transText);
  delay(5000);
}

void postPage(char *webPage, char *parameter){
  if (client.connect()) {
    client.print("POST ");
    client.print(webPage);
    client.println("  HTTP/1.0");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Host: babelfish.yahoo.com");
    client.print("Content-Length: ");
    client.println(strlen(parameter));
    client.println();
    client.println(parameter);
  }
  else {
    Serial.println(" connection failed");
  }
  if (client.connected()) {
     finder.find("<div id=\"result\">");
     finder.getString( ">", "<" ,buffer, sizeof(buffer));
     Serial.println(buffer);
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
}
      
    
