/*
 * UDPSendReceive sketch:
*/
#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <Udp.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address to use
byte ip[] = {192, 168, 1, 177 };    // Arduino's IP address
byte gw[] = {192, 168, 1, 254 };    // Gateway IP address
unsigned int localPort = 8888;      // local port to listen on

byte packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
int packetSize; // holds received packet size
byte remoteIp[4]; // holds received packet's originating IP
unsigned int remotePort; // holds received packet's originating port

const int analogOutPins[] = { 3,5,6,9};  // pins 10 and 11 used by ethernet shield

void setup() {
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  // if there's data available, read a packet
  packetSize = Udp.available();
  if(packetSize > 0 )
  {
    packetSize = packetSize - 8; // 8 byte UDP header so subtract 8 bytes
    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.println(" with contents:");
    // read packet into packetBuffer and get sender's IP addr and port number
    packetSize = min(packetSize,UDP_TX_PACKET_MAX_SIZE);
    Udp.readPacket(packetBuffer,UDP_TX_PACKET_MAX_SIZE, remoteIp, &remotePort);

    for( int i=0; i < packetSize; i++)
    {
        byte value = packetBuffer[i];
        if( i < 4)
        {
           // only write to the first four analog out pins
           analogWrite(analogOutPins[i], value);
        }
        Serial.println(value, DEC);
    }
    Serial.println();
    // tell the sender the values of our analog ports
    sendAnalogValues(remoteIp, remotePort);
  }
  //wait a bit
  delay(10);
}

void sendAnalogValues( byte targetIp[], int targetPort )
{
  int index = 0;
  for(int i=0; i < 6; i++)
  {
     int value = analogRead(i);

     packetBuffer[index++] = lowByte(value);  // the low byte);
     packetBuffer[index++] = highByte(value); // the high byte);   }
  }
  //send a packet to specified peer
  Udp.sendPacket(packetBuffer, index, targetIp, targetPort);  
}
      
    
