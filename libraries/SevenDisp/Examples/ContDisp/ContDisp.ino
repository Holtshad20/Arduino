#include <SevenDisp.h>

SevenDisp Seven(5,6,7); // pin 5 = data, 6 = latch, 7 = clock


void setup() {
}

void loop() {
  for (int x=0; x<100; x++) {
    Seven.Show(x);
    delay(300);
  }
}




