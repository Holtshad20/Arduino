/*
#####################################################################################
#  	Arquivo:            Chave_optica_1                                            
#       Micro-processador:  Arduino UNO ou Teensy++ 2.0      
#  	Linguagem:	    Wiring / C /Processing /Fritzing / Arduino IDE          
#						
#	Objetivo:           Sensor Optical Switch (chave óptica)
#										  
#	Funcionamento:	    Contagem de Objetos, sistema de segurança
#                           com cartão de identificação perfurado, medida
#                           de velocidade de cremalheiras ou objetos,
#                           etc...
#####################################################################################
 
  Este projeto contém código de domínio público.
  A modificação é premitida sem aviso prévio.
 */


// deinições de variáveis
#define opticalPin 2
#define myDelay 1000

// executado na inicialização do Arduino
void setup(){
  Serial.begin(9600); // inicializa a comunicação serial
  pinMode(opticalPin, INPUT); // definição do pino do sensor
}

// loop principal do programa
void loop(){
  // definição de varial para temporização
  // pode armazenar temporização de até 50 dias
  // myDelay é o tempo que queremos comparar
  unsigned long endTime = millis() + myDelay;
  // faz a leitura do sensor e guarda como estado prévio
  byte previousState = digitalRead(opticalPin); 
  int counter = 0; // inicializa contador
  // enquanto for menor que "endTime" executa
  while(endTime > millis()){
    // coloca estado do pino como estado corrente
    byte currentState = digitalRead(opticalPin);
    // se o estado conrrente não for igual ao estado prévio
    if(currentState != previousState){
      counter++; // acrescenta 1 ao contador
      // faz estado prévio igual ao estado corrente
      previousState = currentState; 
    }// continua executando
  }
  // imprime contador no monitor serial
  // facilmente modificado se precisar imprimir em LCD
  // ou display 7 segmentos
  Serial.println(counter);
}
//FIM DA COMPILAÇÃO

