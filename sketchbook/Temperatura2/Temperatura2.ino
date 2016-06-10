#define MAX_AV_READ 100               // média de leituras do sensor
#define FCORR 3.3                     // fator de correção da leitura
#define SUPPLY_VOLTAGE 500.00         // 5V * 100 (LM35 costant)
#define MIN_TEMP 38                   // definir temperaturas mínima e máxima 
#define MAX_TEMP 42                   // para ligar e desligar o aquecimento 


#define tA0 0  // sensor de temperatura -out fio branco 
#define tA1 1  //                       +out fio verde

#define LED 13 // indicar o acionamento do aquecedor

 
float tC0, tC1, tCalc;               // armazenar os valores lidos do sensor;



void setup() {
  // put your setup code here, to run once:
  analogReference(DEFAULT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i<MAX_AV_READ; i++) {
    tC0 += analogRead(tA0);
    delay(5);
    tC1 += analogRead(tA1);
  }

  tC0 /= MAX_AV_READ;  // média
  tC1 /= MAX_AV_READ;

  tCalc = ( (tC0 - tC1) * SUPPLY_VOLTAGE /1024) - FCORR;

  // mostrar resultados na serial 
  Serial.print(tC0);           //show mV reads at A0 input
  Serial.print(" : ");           //print space
  Serial.print(tC1);           //show mV reads at A1 input

  //next show temperature in Celsius scale
  Serial.print(" = C ");
  Serial.println(tCalc);

  // desligar aquecimento
  if (tCalc > MAX_TEMP) {
    digitalWrite(LED, LOW); 
    Serial.println("Desligar Aquecimento");
  }
  // Ligar aquecimento
  if (tCalc < MIN_TEMP) { 
    digitalWrite(LED, HIGH);
    Serial.println("Ligar Aquecimento");
  }
  
  delay(1000);
  
}
