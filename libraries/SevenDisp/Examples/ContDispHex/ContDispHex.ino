#include <SevenDisp.h>

SevenDisp Seven(5,6,7); // pin 5 = data, 6 = latch, 7 = clock


void setup() {
  Seven.Base(16); // define a base númerica como hexadecinal para o display, de 00 a FF
}

void loop() {
  for (int x=0; x<256; x++) {
    Seven.Show(x);
    delay(300);
  }
}




