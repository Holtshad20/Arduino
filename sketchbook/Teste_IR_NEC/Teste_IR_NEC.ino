// look for IR codes and print them as they are received

#include <NECIRrcv.h>
#define IRPIN 11    // pin that IR detector is connected to

NECIRrcv ir(IRPIN) ;

void setup()
{
  Serial.begin(38400) ;
  ir.begin() ;
  digitalWrite(7, LOW);  
}

void loop()
{
  unsigned long ircode ;
  
  while (ir.available() > 0) {
    ircode = ir.read() ;
    Serial.println(ircode) ;
  };
}
