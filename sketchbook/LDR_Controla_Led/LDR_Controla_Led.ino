/* Sketch simples, para testes com LDR */

// definição de variáveis
int photocellPin = 0;     // o LDR e seu resistor de pulldown são conectados ao pino a0
int photocellReading;     // leitura analógica do divisor do sensor
int LEDpin = 11;          // pino do LED
int LEDbrightness;        // variável para conter o brilho do LED
void setup(void) {
  // envia informação para o monitor serial
  Serial.begin(9600);   // inicializa a comunicação serial
}

void loop(void) {
  photocellReading = analogRead(photocellPin); // faz a leitura do LDR  

  Serial.print("Leitura analógica = "); // escreve mensagem
  Serial.println(photocellReading);     // escreve a quantidade lida

  // LED brilhará mais no escuro 
  // isso significa que temos que inverter a leitura de 0-1023 para 1023-0
  photocellReading = 1023 - photocellReading;
  
  // mapeamento dos valores de 0-1023 para 0-255 que é o que o a função analogWrite usa
  LEDbrightness = map(photocellReading, 100, 800, 0, 255);
  analogWrite(LEDpin, LEDbrightness); // acende o LED com o brilho determinado

  delay(100); // pausa
}


// Fim da compilação

