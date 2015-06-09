/*

   Controle PID de Potência em Corrente Alternada - Arduino e TRIAC
   
   Fonte: 
   http://automatobr.blogspot.com.br/2013/05/controle-de-potencia-em-corrente.html

   exemplo 1

*/
#include <PID_v1.h>
 
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
 
long t;
 
void setup()
{
  Serial.begin(9600);
  pinMode(OutputPin, OUTPUT);
// Essa instrução ativa o PID, ou seja, a cada Compute(), o sistema
// irá calcular o valor de MV.
  pid.SetMode(AUTOMATIC); 
  SetPoint=60;
  t=millis();
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
  PresentValue=temp;
// Calcula o valor manipulado   
  pid.Compute();
// Atua no processo
  analogWrite(OutputPin,ManipulatedValue);
// Mostra os valores pela serial  
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
