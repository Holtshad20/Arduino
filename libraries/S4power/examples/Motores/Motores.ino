/*
  Exemplo de utilização do Shied S4Power do LdG 
  
  Este código de exemplo faz com que o motor seja acionado de -20% até +20% 
  (invertendo a direção) do motor.
  Os valores de -20 a +20 são armazenados na variável ".speed" de cada motor e em 
  seguida é chamada a função "Update" para acionar o motor na potência desejada.
  O mesmo é feito para a saída de potência com mosfet.
  Entre cada acionamento é utilizado um delay de 100 milisegundos, 
  portanto o tempo entre -20 e +20 porcento leva 40 x 100 ms = 4 segundos.
  
  Link para o tutorial: http://labdegaragem.com/profiles/blogs/tutorial-como-utilizar-o-shield-4power-com-arduino
  Wiki do LdG: http://www.labdegaragem.com.br/wiki/index.php?title=Shield_4Power#Programa.C3.A7.C3.A3o_exemplo_para_Arduino
 
*/

#include <S4power.h>

S4Power s4power;

void setup() {
  s4power.Config();
}
void loop()
{
  for ( int count = -20; count < 20; count++ )
  {
   s4power.M1.speed = count;
   s4power.M2.speed = count;
   s4power.M3.speed = count;
   s4power.M4.speed = count;

   s4power.M1.Update();
   s4power.M2.Update();
   s4power.M3.Update();
   s4power.M4.Update(); 

   s4power.light.intensity = count;
   s4power.light.Update();

   delay (100);
  }
}

