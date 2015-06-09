#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0B, 0x00, 0x26 };
IPAddress ip(192,168,1,100);
IPAddress server( 64, 233, 187, 99 ); // Google

EthernetClient client;

void setup()
{
  Ethernet.begin(mac, ip); // start ethernet using the mac and IP address
  Serial.begin(9600);      // start the serial library:
  delay(1000);             // give the ethernet hardware a second to initialize

  Serial.println("connecting...");

  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.0"); // the HTTP request
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);  // echo all data received to the Serial Monitor
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
      
    
