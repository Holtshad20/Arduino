#include <SevenDisp.h>

SevenDisp Seven(5,6,7); // pin 5 = data, 6 = latch, 7 = clock

byte b = B00000001;

void setup() {
   Serial.begin(9600);
}

void loop() {
  for (byte x=0; x<7; x++) {
    b = b << 1;
    Serial.println(b, BIN);    
    delay(1000);    
  }
  delay(3000);
  Serial.println("=======");
  for (byte x=0; x<7; x++) {
    b = b >> 1;
    Serial.println(b, BIN);
    delay(1000);
    
  }
  
}




