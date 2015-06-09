/*
 RespondingToChanges sketch
 flash an LED to indicate low voltage levels
*/

long warningThreshold  = 1200;  // Warning level in millivolts - LED flashes
long criticalThreshold = 1000;  // Critical voltage level - LED stays on

const int batteryPin = 0;
const int ledPin = 13;

void setup()
{
   pinMode(ledPin, OUTPUT);
}

void loop()
{
  int val = analogRead(batteryPin);    // read the value from the sensor
  if( val < (warningThreshold  * 1023L)/5000) {
    // in the line above, L following a number makes it a 32 bit value
    flash(val) ;
  }
}

// function to flash an led with on/off time determined by value
// passed as percent
void flash(int percent)
{
  digitalWrite(ledPin, HIGH);
  delay(percent + 1);
  digitalWrite(ledPin, LOW);
  delay(100 - percent );  // check delay == 0?
}
      
    
