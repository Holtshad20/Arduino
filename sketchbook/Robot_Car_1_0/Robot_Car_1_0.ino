/*
  Robot Car by Luís Bulek
  Versão: 1.0 
  Revisão: 14.04.2012
 
 Controlar um Carro Robo por intermédio de um controle remoto infravermelho, 
 com dois motores de trações separadas, usando como etapa de potência duas Pontes H, 
 para controle independente dos motores. O carro contém dois LEDs com tripla função, 
 servem como faróis, seta indicando a mudança de direção e indicador de marcha a ré.
 
 Comandos básicos: avançar, retroceder, esquerda, direita, ligar/desligar os faróis 
 
 Ligação do ARDUINO a Etapa de Potência:
 +========================================================+
 |Etapa de Potência              |  ARDUINO UNO   |LABEL  |
 +================================================|=======+
 |1 - GND                        |      GND       |       |
 |2 - M1 Avançar                 |      5         | M1_A  |
 |3 - M1 Retroceder              |      6         | M1_R  |
 |4 - M2 Avançar                 |      9         | M2_A  |
 |5 - M2 Retroceder              |      10        | M2_R  |
 |6 - NC                         |      -         |       |
 |7 - LED Esquerdo (Farol/Pisca) |      2         | LED_E |
 |8 - LED Direito (Farol/Pisca)  |      4         | LED_D |
 |-   SPEAKER                    |      7         | SPEAKER |
 |-   IRPIN                      |      11        | IRPIN   |
 +========================================================+
 A escolha das saídas PWM (5,6,9,10), se dá em função de já pensar 
 em controlar a velocidade dos motores por PWM
 
 Obs.: Futuramente colocarei sensores de obstáculos (IR ou Ultrasom), para evitar que o carro
 colida com os mesmos, mesmo que o operador comande o carro para frente ou para trás.
 
 
 */
#include <NECIRrcv.h>

#define IRPIN 11 // Porta do detector de IR
// Controlar a direção do carro 
#define AVANCAR     0xE9166B86  // CH+
#define RETROCEDER  0xEB146B86  // CH-
#define ESQUERDA    0xE8176B86  // VOL-
#define DIREITA     0xE01F6B86  // VOL+
#define PARAR       0xE31C6B86  // ZOOM
#define FAROIS      0xFF006B86  // SOURCE

// não sei que função dar a estas teclas
#define POWER  0xE11E6B86  // POWER
#define ALARME 0xEC136B86  // LOOP

// Aumenta/Diminui a velocidade dos dois motores
#define VEL_MAIS  0xEF106B86  // +100
#define VEL_MENOS 0xE7186B86  // MUTE

// Saídas que estão conectados os motores
#define M1_A 6   // Motor 1 avançar
#define M1_R 5   // Motor 1 retroceder
#define M2_A 10  // Motor 2 avançar
#define M2_R 9   // Motor 2 retroceder

// Saídas que estão conectados os LEDs
#define LED_E 2  // Farol Esquerdo
#define LED_D 4  // Fatrol Direito
// Piezo alarme
#define SPEAKER 7

// alternar o estado dos leds (farois), conforme o apertar dos botões do controle
// Ligar / Desligar os leds
boolean LED_E_STATE = LOW;
boolean LED_D_STATE = LOW;
boolean RE = false;

unsigned long UltMov;
byte Vel = 120; // inicializa a velocidade dos motores em 50%

NECIRrcv ir(IRPIN) ; // classe utilizada pela library IR

void setup() {
  Serial.begin(9600) ; // usada para debug
  Serial.println("Robot Car - Carro guiado por Controle Remoto") ;
  ir.begin() ; // inicializar IR   

  // definir pinos dos motores como saída 
  pinMode(M1_A, OUTPUT);
  pinMode(M1_R, OUTPUT);
  pinMode(M2_A, OUTPUT);
  pinMode(M2_R, OUTPUT);
  // definir pinos dos leds como saida
  pinMode(LED_E, OUTPUT);
  pinMode(LED_D, OUTPUT);
  
  pinMode(SPEAKER, OUTPUT);
  //playTone(4000,500);
}

void loop()
{
  unsigned long ircode ;

  while (ir.available()) {
    ircode = ir.read() ;
    Serial.print("got code: 0x") ;
    Serial.println(ircode,HEX) ;
  };

  // ligar / desligar farois (os dois leds)
  if (ircode == FAROIS) { 
    LED_E_STATE = !LED_E_STATE;          
    digitalWrite(LED_E, LED_E_STATE); 
    LED_D_STATE = !LED_D_STATE;                
    digitalWrite(LED_D, LED_D_STATE); 
  };

  // Aumentar/Diminuir a velocidade dos motores
  if (ircode == VEL_MAIS) {
    Vel+=10;
    Serial.println(Vel);        
    if (UltMov == AVANCAR) Motor(HIGH, LOW, HIGH, LOW);
    if (UltMov == RETROCEDER) Motor(LOW, HIGH, LOW, HIGH);
  }
  
  if (ircode == VEL_MENOS) {
    Vel-=10;
    Serial.println(Vel);        
    if (UltMov == AVANCAR) Motor(HIGH, LOW, HIGH, LOW);
    if (UltMov == RETROCEDER) Motor(LOW, HIGH, LOW, HIGH);    
  }
  
  // Deixar a velocidade dentros dos limites do PWM
  if (Vel < 10)  Vel = 10; 
  if (Vel > 240) Vel = 240;

  // avançar carro
  if (ircode == AVANCAR) Avancar();

  // parar carro
  if (ircode == PARAR) Parar();

  // retroceder carro
  if (ircode == RETROCEDER) Retroceder();

  // avançar à direita
  if (ircode == DIREITA) Avancar_direita();

  // avançar à esquerda
  if (ircode == ESQUERDA) Avancar_esquerda();

  // piscar os 2 leds quando for marcha a ré
  if (RE) Piscar_doisLeds();
}

void Motor(boolean M1A, boolean M1R, boolean M2A, boolean M2R) {
  // Envia para as portas de saída, os comandos para os motores

  // Se for colocar o motor em movimento, usar o PWM
  if (M1A == HIGH) 
     analogWrite(M1_A, Vel);
  else
     digitalWrite(M1_A, LOW); // parar o motor
 
  if (M1R == HIGH) 
     analogWrite(M1_R, Vel);
  else 
     digitalWrite(M1_R, LOW);
     
  if (M2A == HIGH) 
     analogWrite(M2_A, Vel);
  else 
     digitalWrite(M2_A, LOW);
    
  if (M2R == HIGH) 
     analogWrite(M2_R, Vel);
  else 
     digitalWrite(M2_R, LOW);
}

void Piscar_doisLeds() {
  // piscar os dois leds quando for marcha a ré
  digitalWrite(LED_E, LOW);
  digitalWrite(LED_D, LOW);  
  //playTone(1000, 300);    
  delay(300);
  digitalWrite(LED_E, HIGH);
  digitalWrite(LED_D, HIGH);  
  //playTone(300, 500);    
  delay(300); 
  digitalWrite(LED_E, LOW);  
  digitalWrite(LED_D, LOW);    

  digitalWrite(LED_E, LED_E_STATE); 
  digitalWrite(LED_D, LED_D_STATE); 
}

void Parar() {
  // Parar o carro
  UltMov = PARAR;
  Motor(LOW, LOW, LOW, LOW);
  RE = false;
}

void Avancar() {
  // mover o carro para frente
  UltMov = AVANCAR;
  Motor(HIGH, LOW, HIGH, LOW);
  RE = false;  
}

void Retroceder() {
  // mover o carro para trás
  UltMov = RETROCEDER;
  Motor(LOW, HIGH, LOW, HIGH);
  // indicar que foi dada marcha a ré
  RE = true;
}

void Avancar_direita() {
  // Avancar à direita
  Motor(HIGH, LOW, LOW, LOW);
  
  for (int x = 0; x < 3; x++) {
    Piscar_led(LED_D);
  };
  Avancar();
}

void Avancar_esquerda() {
  // Avançar à esquerda
  Motor(LOW, LOW, HIGH, LOW);

  for (int x = 0; x < 3; x++) {
    Piscar_led(LED_E);
  };
  Avancar();
}

void Piscar_led(byte LED) {
  digitalWrite(LED, LOW);
  delay(300);
  digitalWrite(LED, HIGH);
  delay(300); 
  digitalWrite(LED, LOW);  
  digitalWrite(LED_E, LED_E_STATE); 
  digitalWrite(LED_D, LED_D_STATE); 
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(SPEAKER, HIGH);
    delayMicroseconds(tone);
    digitalWrite(SPEAKER, LOW);
    delayMicroseconds(tone);
  }
}

