/*
 * i2cDebug.cpp
 */
 #include <i2cDebug.h>
 #include <Wire.h>    // the Arduino I2C library
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
I2CDebugClass i2cDebug;   // Create an I2C debug object