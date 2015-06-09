#include <SPI.h>
#include <Ethernet.h>

#define BUFFER 16

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0B, 0x00, 0x26 };
IPAddress ip(192,168,1,100);
EthernetServer server(80);

char ctl[BUFFER];


void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
     for (byte i = 0; i < BUFFER; i++) {
       ctl[i] = client.read();
     }
     Serial.println(ctl);
     Serial.println();   
     switch(ctl[0]) {
       case 'R':        
       LerPortas();
       client.write(ctl);
       break;
     } 
  }
}

void LerPortas(){
  for (byte i = 2; i < 10; i++) {
    if (digitalRead(i) == HIGH) ctl[i]='H'; else ctl[i]='L';
  }; 
}

