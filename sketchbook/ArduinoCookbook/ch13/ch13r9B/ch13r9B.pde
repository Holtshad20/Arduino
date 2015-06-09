              
/*
 * I2C_Master
 * Sends serial data to an I2C slave
 */


#include <Wire.h>

const int address = 4;    //the address to be used by the communicating devices
const int sensorPin = 0;  // select the analog input pin for the sensor
int val;                  // variable to store the sensor value

class I2CDebugClass : public Print
{
  private:
    int I2CAddress;
    byte count;
    void write(byte c);
  public:
    I2CDebugClass();
    boolean begin(int id);
};


 I2CDebugClass::I2CDebugClass()
 {
 }
 boolean I2CDebugClass::begin(int id)
 {
    I2CAddress = id;  // save the slave's address
    Wire.begin(); // join I2C bus (address optional for master)
    return true;
 }

  void I2CDebugClass::write(byte c)
  {
     if( count == 0)
     {
      // here if the first char in the transmission
       Wire.beginTransmission(I2CAddress); // transmit to device
     }
     Wire.send(c);
     //  if the I2C buffer is full or an end of line is reached, send the data
     // BUFFER_LENGTH is defined in the Wire library
     if(++count >= BUFFER_LENGTH || c == '\n') 
     {
         // send data if buffer full or newline character
         Wire.endTransmission();
         count = 0;
     }
  }

I2CDebugClass I2CDebug;   // the I2C print object

void setup()
{
   Wire.begin();
   Serial.begin(9600);
}


void loop()
{
   val = analogRead(sensorPin);       // read the voltage on the pot 
                                      // (val ranges from 0 to 1023)
   Serial.println(val);
   I2CDebug.println(val);
}
