/* 
  Sensor de proximidade de objetos com Emissor IR e Repector IR PHSC38
  Funciona legal
  Colocar Emissor e Receptor dentro de tubos pretos, para evitar a luz que irá 
  interferir na leitura do sensor. Tubos pretos de 1 cm de diâmetro,com 2 a 4cm 
  de comprimento. 
  Realizei testes com os tubos colocados e não houve interfêrencia nem de lâmpadas
  e nem da luz solar. :-)))
  
  Link do original: http://lusorobotica.com/index.php/topic,527.0.html
  Library MsTimer2: http://playground.arduino.cc/Main/MsTimer2
  
*/

#include <MsTimer2.h>
#define irx 2   // Receptor IR no pin digital 2
int value ;  // Salva o valor lido para processamento
static boolean output = HIGH;  /// Definir o estado do Emissor IR


void setup(){
  pinMode(irx, INPUT);     // Pin Emissor IR
  Serial.begin(9600);      
  MsTimer2::set(20, flash); // Definir interrupções de 20 ms para acender/apagar LED IR
                            // Pode-se diminuir este tempo para que a detecção capture objetos 
                            // que passem rapidamente em frente ao sendor
  MsTimer2::start();        // Iniciar interrupções
} 

void loop(){
  value = digitalRead(irx);
 
  if (value == 0) {  // value = 0 - detectou um objeto próximo
    Serial.println("Objeto detectado");
    delay(300);
  }
  
}

/* Função chamada durante a interrupção */
void flash() {               
  digitalWrite(12, output);  // ligar o LED
  output = !output;          // Alternar ligado/desligado
}

