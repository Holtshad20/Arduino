/*

   Controle PID de Potência em Corrente Alternada - Arduino e TRIAC
   
   Fonte: 
   http://automatobr.blogspot.com.br/2013/05/controle-de-potencia-em-corrente.html

   Versao final para controlar um triac

*/

#include "PID_v1.h"
 
// Carga (pino onde será aplicado o output)
#define OutputPin 3    
 
// Constantes do PID
#define kp 50
#define ki 10
#define kd  0
 
// Variáveis do PID: a nomenclatura é a mesma descrita no post III
double SetPoint, PresentValue, ManipulatedValue;
 
// O último parâmetro indica se o controle é inversa ou 
// diretamente proporcional, ou seja, se a uma alteração do PV
// deve corresponder uma alteração de mesmo sinal (DIRECT) ou 
// de sinal trocado (REVERSE).
PID pid(&PresentValue, &ManipulatedValue, &SetPoint, kp, ki, kd, DIRECT);
 
volatile int power = 100;  
 
long t;
 
void setup()
{
  Serial.begin(9600);
  pinMode(OutputPin, OUTPUT);
  attachInterrupt(0, zero_crosss_int, RISING); 
// Os valores 10 e 240 foram definidos empiricamente usando-se
// o osciloscópio para ver onde o controle da onda é estável.
  pid.SetOutputLimits(10, 240);
  pid.SetMode(AUTOMATIC); 
  SetPoint=60;
  t=millis();
}
 
void zero_crosss_int()  
{
// Cálculo do ângulo de disparo: 60Hz-> 8.33ms (1/2 ciclo)
// (8333us - 8.33us) / 256 = 32 (aprox).
// Ou seja, cada 1 unidade em power corresponde a 32 microssegundos
// na onda de meio ciclo.
  int powertime = (32*(256-power));      
// Mantém o circuito desligado por powertime microssegundos 
  delayMicroseconds(powertime);   
// Envia sinal ao TRIAC para que ele passe a conduzir 
  digitalWrite(OutputPin, HIGH);  
// Espera alguns microssegundos para que o TRIAC perceba o pulso
  delayMicroseconds(8.33);      
// Desliga o pulso
  digitalWrite(OutputPin, LOW);   
}
 
 
// Função que lê o PV. É uma função que lê a saída  
// de um LM35 cinco vezes e acumula, depois tirando a média das
// 5 leituras e calculando a temperatura em °C.
int GetTemp(int sensor)
{
  float temp = 0;
  for(int i=0; i< 5;i++)
  {
    temp += analogRead(sensor);
    delay(20);
  }
  temp = (temp * 0.48875855)/5;
  return (int)temp;
}
 
void loop()
{
  float temp = GetTemp(A0);
  PresentValue=temp;// Calcula o valor manipulado   
  pid.Compute();
// Valor de power, que vai ser usado em
// zero_crosss_int() para atuar no processo.
  power = ManipulatedValue;
// Exibe os valores usados de 1 em 1s.
  if ((millis()-t) > 1000)
  {
    Serial.print(millis()/1000);
    Serial.print(",");
    Serial.print(PresentValue);
    Serial.print(",");
    Serial.println(ManipulatedValue);
    t=millis();
  }
  delay(300);
}
