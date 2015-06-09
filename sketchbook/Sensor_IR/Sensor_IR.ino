/* Programa para detectar distâcia de +-10cm e não sofre influência de outras 
   fontes de luz,apenas do sol. 
   visite: http://brasilrobotics.blogspot.com.br/2010/01/sensor-de-infra-vermelho-de-distacia.html
   by Gamesh_
   criado por Gamesh_ 
*/
int inPin = 7;  // Diodo infravermelho RECEPTOR TIL78 com filtro de luz(LED de cor negra)
int irPin = 11; // pino do LED TRANSMISSOR infravermelho(LED meio azulado)
int redPin = 12;  // LED normal de cor vermelha

int val = 0; // variavel lida no pino

void setup() {
  Serial.begin(9600);
  
  pinMode(irPin, OUTPUT); // declare LED as output
  pinMode(inPin, INPUT); // entrada do TIL78
  pinMode(redPin, OUTPUT);
}

void loop()
{
  val = digitalRead(inPin); // lendo valor  do TIL78
  Serial.println(val);
  digitalWrite(irPin, HIGH); // o pino infravermelho esta sempre ligado
  if (val == LOW) {// é verdade quando tem algo no campo do ir(detectado) -- com luz o TIL78 gera "0"
      digitalWrite(redPin, LOW); //  LED OFF
  } else {  //se nao for "0"(LOW) vai ser "1"(HIGH) entao deve fazer: Ligar o LED vermelho.
      digitalWrite(redPin, HIGH); //LED fica ligado ON
    }
}
