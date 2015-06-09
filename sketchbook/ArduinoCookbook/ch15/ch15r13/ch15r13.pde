*
 * UDPSendReceiveStrings
 * This sketch receives UDP message strings, prints them to the serial port
 * and sends an "acknowledge" string back to the sender
 *
*/

#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <Udp.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address to use
byte ip[] = {192, 168, 1, 177 };    // Arduino's IP address

unsigned int localPort = 8888;      // local port to listen on

// the next two variables are set when a packet is received
byte remoteIp[4];        // holds received packet's originating IP
unsigned int remotePort; // holds received packet's originating port

// buffers for receiving and sending data
byte packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char replyBuffer[] = "acknowledged";       // a string to send back


void setup() {
    // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  Serial.begin(9600);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.available(); // note that this includes the UDP header
  if(packetSize)
  {
    packetSize = packetSize - 8;      // subtract the 8 byte header
    Serial.print("Received packet of size ");
    Serial.println(packetSize);

    // read packet into packetBuffer and get sender's IP addr and port number
    Udp.readPacket(packetBuffer,UDP_TX_PACKET_MAX_SIZE, remoteIp, &remotePort);
    Serial.println("Contents:");
    Serial.println((char*)packetBuffer);

    // send a string back to the sender
    Udp.sendPacket((byte*)replyBuffer,strlen(replyBuffer),remoteIp,remotePort);

  }
  delay(10);
}
      
    
