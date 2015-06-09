/*
 * i2cDebug.h
 */
 #include <WProgram.h>
 #include <Print.h>   // the Arduino print class
 
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

extern I2CDebugClass i2cDebug;   // the i2c debug object