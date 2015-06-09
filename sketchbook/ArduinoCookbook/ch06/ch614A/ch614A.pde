// Another simple sketch to detect the Southern Hemisphere
// Assumes: LED on pin 13, GPS connected to pins 2/3
// (Optional) Serial debug console attached to hardware serial port 0/1

#include "TinyGPS.h"
#include "NewSoftSerial.h"

#define HEMISPHERE_PIN 13
#define GPS_RX_PIN 2
#define GPS_TX_PIN 3

TinyGPS gps; // create a TinyGPS object
NewSoftSerial nss(GPS_RX_PIN, GPS_TX_PIN); // create soft serial object

void setup()
{
  Serial.begin(9600); // for debugging
  nss.begin(4800); // Use NewSoftSerial object to talk to GPS
  pinMode(HEMISPHERE_PIN, OUTPUT);
  digitalWrite(HEMISPHERE_PIN, LOW); // turn off LED to start
}
void loop()
{
  while (nss.available())
  {
    int c = nss.read();
    Serial.print(c, BYTE); // display NMEA data for debug
    // Send each byte to encode()
    // Check for new position if encode() returns "True"
    if (gps.encode(c))
    {
      long lat, lon;
      gps.get_position(&lat, &lon);
      // Display new lat/lon on debug console
      Serial.print("Lat: "); 
      Serial.print(lat);
      Serial.print(" Lon: "); 
      Serial.println(lon);
      if (lat < 0) // Southern Hemisphere?
        digitalWrite(HEMISPHERE_PIN, HIGH);
      else
        digitalWrite(HEMISPHERE_PIN, LOW);
    }
  }
}

