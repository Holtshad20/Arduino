#include <TimerOne.h>   // inclui biblioteca Timer1
#include <MsTimer2.h>   // Inclui biblioteca MsTimer2

int sensorDistancia = A0;  // porta associada ao sensor distancia IR
int triacGatePin = 3 ;  // porta associada ao gate do TRIAC

int distancia = 0 ;  // variavel para armazenar distancia
int power= 0 ;  //inicializa variavel que controla potencia na lampada com 0 (lampada apagada)

void setup() {  
  pinMode(triacGatePin, OUTPUT);
  /* associa interrupcao INT0 (D2) com funcao "zeroCrossInterrupt" */
  attachInterrupt(0, zeroCrossInterrupt, CHANGE);  
  /* inicializa Timer2 com 200 milisegundos (tempo entre leituras do sensor de dintancia) */
  MsTimer2::set(200, leSensor);
  MsTimer2::start();  // start do timer2
}

void loop() {
}


void zeroCrossInterrupt(){  // trata interrupcao INT0 
  if(power > 0) {
    long dimtime = int(map(power,0,100,8000,150));  // calcula o tempo de delay para o disparo do TRIAC
    Timer1.initialize (dimtime);  // inicializa o TIMER1 com o delay calculado
    Timer1.attachInterrupt(gateTRIAC);  //associa a funcao gateTRIAC com  Interrupcao do TIMER1
    Timer1.start();  // inicia contagem TIMER1
    }  else {
       digitalWrite(triacGatePin, LOW);   // mantem gate do TRIAC desativado.  
    Timer1.stop();
}
}

void gateTRIAC () {  // trata interrupcao do TIMER1 gerando pulso no gate do TRIAC
  digitalWrite(triacGatePin, HIGH);  // dispara o Triac
  delayMicroseconds(15);  // aguarda 15 microsegundos para garantir disparo do TRIAC
  digitalWrite(triacGatePin, LOW);   // desabibilta gate do TRIAC 
  Timer1.stop();
}

void leSensor() {
  distancia = analogRead(sensorDistancia);  // le sensor de distancia
  //Serial.println(distancia);
  distancia = map(distancia,70,600,120,0);  // mapeia distancia lida para uma faixa de potencia entre 0 e 120
  if (distancia < 0) {
     distancia = 0;
  }

  if (distancia >=0 && distancia <= 100) {  // se distancia <= a 100 atualiza potencia
     power=distancia;
  }
}

