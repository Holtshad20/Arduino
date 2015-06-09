/* 
   Leitura de sinal infravermelho sem biblioteca usando apenas pulseIn

   Autor: Leonardo Barichello


*/   
#define sensor 11 //pino com o sensor

void setup() {
  //pino do sensor IR
  pinMode(sensor, INPUT);
  //monitor serial
  Serial.begin(9600);
}

void loop() {
  unsigned int duracao, menor;
  unsigned int sinal[32]; //tamanho arbitrário que parece ser suficiente
  int sinalbits[256]; //tamanho arbitrário que parece ser suficiente
  int j,i,pos,v,t1,t2;

  v=1; //variável para alternar entre leitura de sinal HIGH e LOW
  duracao = pulseIn(sensor, v, 100000);

  if(duracao!=0) {

    //zerando vetor de duracoes
    for(i=0;i<32;i=i+1) {
      sinal[i]=0;
    }

    //zerando vetor de bits
    for(i=0;i<256;i=i+1) {
      sinalbits[i]=0;
    }

    i=0;
    sinal[i]=duracao; //guardando a primeira duração já lida

    //lendo sinal
    while (duracao!=0) {
      v=(v+1)%2;
      duracao = pulseIn(sensor, v, 100000);
      i=i+1;
      sinal[i] = duracao;
    }

    t1=i; //comprimento real do vetor de durações

    //mostrando o vetor de duracoes
    Serial.print("Vetor de duracoes: ");
    for(i=0;i<t1;i=i+1) {
      Serial.print(sinal[i]);
      Serial.print(", ");
    }
    Serial.println(".");

    //achando o menor pulso
    Serial.print("Duracao do menor pulso: ");
    menor=100000;
    for(i=0;i<t1;i=i+1) {
      if ( (sinal[i]<menor) && (sinal[i]!=0) ) {
        menor = sinal[i];
      }
    }
    Serial.println(menor);

    //transformando o vetor de duracao em microseg para numero de intervalos
    for(i=0;i<t1;i=i+1) {
      sinal[i] = round(sinal[i]/menor);
    }

    //mostrando o vetor de duracoes convertido
    Serial.print("Vetor de duracoes em pulsos: ");
    for(i=0;i<t1;i=i+1) {
      Serial.print(sinal[i]);
      Serial.print(", ");
    }
    Serial.println(".");

    //gerando vetor de bits
    v = 1;
    pos = 0;
    for(i=0;i<32;i=i+1) {
      for(j=0;j<sinal[i];j=j+1) {
        sinalbits[pos]=v;
        pos=pos+1;
      }
      v=(v+1)%2;
    }

    t2=pos; //tamanho real do vetor de bits

      //mostrando o vetor de bits
    Serial.print("Vetor de bits: ");
    for(j=0;j<t2;j=j+1) {
      Serial.print(sinalbits[j]);
    }
    Serial.println(".");

  }

}


