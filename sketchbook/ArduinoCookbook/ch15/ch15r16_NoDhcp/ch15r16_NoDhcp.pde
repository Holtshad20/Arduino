              
#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <TextFinder.h>

const unsigned long feedID  =  2955; // this is the ID of my float test feed
const int  streamCount      =     6; // Number of data streams to send
const long REFRESH_INTERVAL = 60000; // Update every  minute
// if connection fails/resets wait 10 seconds before trying again
// should not be less than 5
const long RETRY_INTERVAL   = 10000;  

//#define PACHUBE_API_KEY  "Your key here . . . " // fill in your API key
#define PACHUBE_API_KEY            "737d3e770e334d08eed9ad80b6ed951232b3a6c0d079f489dc7485341246cafc" // fill in your API key 

// make sure this is unique on your network
byte mac[] = { 0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; 
//byte remoteServer[] = { 209,40,205,190 };            // pachube.com
byte remoteServer[] = { 173,203,98,29 };               // new pachube ip address  

byte ip[] = { 192, 168, 1, 144  };                   // this version does not have DHCP so set IP address
byte gateway[] ={ 192, 168, 1, 1 };
byte subnet[] ={ 255, 255, 255, 0 };

//used to store csv output and response strings (date is 26 chars)
char buffer[32];            

Client      client(remoteServer, 80);
TextFinder  finder( client );

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);  
  delay(4000);
}

void loop()
{

int data[streamCount];
  buffer[0] = 0;
  char * bufPointer = buffer;
  for(int id = 0; id < streamCount; id++)
  {
     float temperature = getTemperature(id)   ;
     formatFloat( temperature, 1, &buffer[strlen(buffer)]);  // format as xx.y
     if( id < streamCount-1)
        strcat(buffer, ",");                // commas between all but last value
  }
  if( putFeed(feedID, streamCount) == true)
  {
     Serial.print("Feed updated: ");
     Serial.println(buffer);
     delay(REFRESH_INTERVAL);
  }
  else
  {
      Serial.println("Unable to update feed");
      delay(RETRY_INTERVAL);
  }
}

// returns true if able to connect and send data
boolean putFeed(int feedId, int streamCount )
{
boolean result = false;
  Serial.print("Connecting feed "); Serial.print(feedId); Serial.print(" ... ");
  if (client.connect()) {
    client.print("PUT /api/feeds/");
    client.print(feedId);
    client.print(".csv HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
    //client.print(".xml HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
    client.print(PACHUBE_API_KEY);
    client.print("\nUser-Agent: Arduino");
    client.print("\nContent-Type: text/csv\nContent-Length: ");
    client.print(strlen(buffer));
    client.print("\nConnection: close\n\n");
    client.print(buffer);

    client.println("\n");
  }
  else {
    Serial.println("Connection failed");
  }
  if (client.connected()) {
    Serial.println("Connected");
    if(  finder.find("HTTP/1.1") && finder.find("200 OK") ){
       finder.getString("Date: ", "\r",buffer, sizeof(buffer));  // store time
       result = true;
    }
    else
       Serial.println("Dropping connection - no 200 OK");
  }
  else {
    Serial.println("Disconnected");
  }
  client.stop();
  client.flush();
  return result;
}

float getTemperature(int pin)
{
   const int nbrSamples = 8; // take the average of 8 samples
   int samples = 0;
   for(int i=0; i < nbrSamples; i++)
       samples = samples + analogRead(pin);
   float t = samples / nbrSamples; // get the average value
   t =  (t /1024.0) * 500;  // each degree C is 10mv
   return t;
}

// returns the number of characters added to the buffer
int formatFloat(float number, byte digits, char *buf)
{
  if (!buf )
    return 0;
  char * bufStart = buf;  // store the start of the buffer
  // Handle negative numbers
  if (number < 0.0)
  {
     *buf++ = '-';
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  ultoa(int_part, buf, 10 );
  buf = &buf[strlen(buf)];

  // Print the decimal point, but only if there are digits
  if (digits > 0 )
    *buf++ = '.';

  // Extract digits from the remainder one at a time
  while (digits-- > 0 )
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    *buf++ = toPrint + '0';
    remainder -= toPrint;
  }
  *buf = 0;
  return buf-bufStart; // the number of characters added
}
