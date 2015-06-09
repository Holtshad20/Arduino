#if ARDUINO > 18
#include <SPI.h>         // needed for Arduino versions later than 0018
#endif

#include <Ethernet.h>
#include <TextFinder.h>

const unsigned long feedID  =    504; // this is the ID of the
                                      // remote Pachube feed that
                                      // you want to connect to
const int  streamCount      =      4; // Number of data streams to get
const long REFRESH_INTERVAL = 600000; // Update every 10 minutes
const long RETRY_INTERVAL   = 10000;  // if connection fails/resets
                                      // wait 10 seconds before trying
                                      // again - should not be less than 5

#define PACHUBE_API_KEY  "your key here . . ." // fill in your API key

// mac address, make sure this is unique on your network
byte mac[] = { 0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; 
byte remoteServer[] = { 173,203,98,29 };            // pachube.com

byte ip[] = { 192, 168, 1, 144  };                   // no DHCP so set IP address
byte gateway[] ={ 192, 168, 1, 1 };
byte subnet[] ={ 255, 255, 255, 0 };

float streamData[streamCount];    // change float to long if needed for your data
char findBuffer     [11];  // holds one numeric field - room for 10 characters
                           // and the terminating null

Client client(remoteServer, 80);
TextFinder  finder( client );

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
}

void loop()
{
   if( getFeed(feedID, streamCount) == true)
   {
      for(int id = 0; id < streamCount; id++){
        Serial.println( streamData[id]);
      }
      Serial.println("--");
      delay(REFRESH_INTERVAL);
   }
   else
   {
      Serial.println("Unable to get feed");
      delay(RETRY_INTERVAL);
   }
}

// returns true if able to connect and get data for all requested streams
// in this feed
boolean getFeed(int feedId, int streamCount )
{
boolean result = false;
  Serial.print("Connecting feed "); Serial.print(feedId); Serial.print(" ... ");
  if (client.connect()) {
    client.print("GET /api/feeds/");
    client.print(feedId);
   // client.print(".csv HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
    client.print(".xml HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
    client.print(PACHUBE_API_KEY);
    client.print("\nUser-Agent: Arduino");
    client.println("\n");
  }
  else {
    Serial.println("Connection failed");
  }
  if (client.connected()) {
    Serial.println("Connected");
    if(  finder.find("HTTP/1.1") && finder.find("200 OK") )
       result = processFeed(streamCount);
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

int processFeed(int streamCount)
{
  finder.find("<environment updated=");
  finder.getString("T", "\"",findBuffer, sizeof(findBuffer));    // get the time
  Serial.print("Values updated at ");
  Serial.println(findBuffer);

  int processed = 0;
  for(int id = 0; id < streamCount; id++)
  {
    if( finder.find( "<data id=" ) )  //find next data field
    {
       if(finder.find("<value ")  )
       {
         finder.find(">"); // seek to the angle brackets
         streamData[id] = finder.getValue();
         processed++;
       }
    }
    else {
      Serial.print("unable to find Id field ");
      Serial.println(id);
    }
  }
  return(processed == streamCount );  // return true iff got all data
}
      
    
