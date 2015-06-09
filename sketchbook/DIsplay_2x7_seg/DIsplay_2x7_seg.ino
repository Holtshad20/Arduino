/*
   Módulo para display de 7 seguementos duplo [88] Kingbrigth modelo DC56-11EWA.
   Usa dois CIs 74HC595.      
*/
#define DATA  5  // pin 14 74hc595
#define LATCH 6  // pin 12 74hc595
#define CLOCK 7  // pin 11 74hc595

#define BASE 10 // base númerica para o display

// definição dos padrões para o display seguindo a ordem dos bits, 
// da esquerda para a direita abcdefg. 
byte LEDDSP [] = {           
		B00000011, // 0
		B10011111, // 1
		B00100101, // 2
		B00001101, // 3
                B10011001, // 4
		B01001001, // 5 
		B01000001, // 6
		B00011111, // 7
		B00000001, // 8
		B00001001, // 9
		B00010001, // a
		B11000001, // b
		B01100011, // c
		B10000101, // d
		B01100001, // e
		B01110001, // f
	};

void setup() {
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
}

void loop() {
    for (int x=0; x<100; x++) {
      seven(x);
      delay(300);
    }
}

void seven(int x) {
  // [ab]
  // digito menos significativo em b
  // digito mais significativo em a
  int a = (x / BASE);
  int b = x - (a * BASE);
  
  digitalWrite(LATCH, LOW);   
  shiftOut(DATA, CLOCK, LSBFIRST, LEDDSP[b]); // envia primeiro a unidade
  shiftOut(DATA, CLOCK, LSBFIRST, LEDDSP[a]); // envia a dezena 
  digitalWrite(LATCH, HIGH);  
}


