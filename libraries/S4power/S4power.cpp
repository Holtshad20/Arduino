#include "Arduino.h"
#include "S4power.h"

/*********************************************************
* CONTROLE DOS MOTORES
*********************************************************/
   
//Configurar o controlador 
void Motor::SetConf ( char pinDir1, char pinDir2, char pinPwm ) 
{
  //Configurar os IOS 
  pinMode(pinDir1, OUTPUT);
  pinMode(pinDir2, OUTPUT);
  pinMode(pinPwm, OUTPUT);

  //Guardar os valores dos IOs
  conf.pinDir1 = pinDir1;
  conf.pinDir2 = pinDir2;
  conf.pinPwm = pinPwm;
}
  
//Atualizar o valor de velocidade para o controlador 
void Motor::Update ( void ) 
{
  //Verificar qual a direção que o motor deve girar 
  if ( speed >= 0 )
  {
      digitalWrite ( conf.pinDir1, LOW );
      digitalWrite ( conf.pinDir2, HIGH );
  }
  else
  {
      digitalWrite ( conf.pinDir1, HIGH );
      digitalWrite ( conf.pinDir2, LOW );
  }

  //Enviar o valor de potência (convertido) 
  analogWrite ( conf.pinPwm, ConvertToPwm(speed));
}
 
//Converter o valor de velocidade para PWM ( velocidade 100 = 255 no PWM )
char Motor::ConvertToPwm ( char speed ) 
{
  float temporary = speed;

  if ( temporary <= 0 )
      temporary *= -1;

  temporary = (temporary == 0 ? 0 : temporary + 18);  

  if ( temporary > 100 )
      temporary = 100;

  temporary = ( temporary / 100 ) * 255;

  return (char) temporary;
}
  
  //Define a estrutura de uma iluminação
void Light::SetConf ( char pinPwm, char pinSensor ) 
{
  //Configura os IOs
  pinMode(pinPwm, OUTPUT);
  pinMode(pinSensor, INPUT);

  //Guarda a configuração de IO
  conf.pinPwm = pinPwm;  
  conf.pinSensor = pinSensor;
}

//Atualiza o valor de intensidade para o controlador 
void Light::Update ( void ) 
{
  //Envia o valor da intensidade (convertido) 
  analogWrite ( conf.pinPwm, ConvertToPwm(intensity) );

  //Faz a leitura analógica e converte para miliamperes
  power = ConvertToPower ( analogRead ( conf.pinSensor ) );
}

//Converte valor de leitura analógica para miliamperes
int Light::ConvertToPower ( int analogSensor )
{
  //ToDo: fazer o cálculo de conversão de leitura analógica para corrente * 100
  return  analogSensor;
}

//Converte o valor de intensidade para PWM ( intensidade 100 = 255 no PWM )
char Light::ConvertToPwm ( char intensity ) 
{
  float temporary = intensity;

  temporary = ( temporary / 100 ) * 255;

  return (char) temporary;
}

/*********************************************************
* Configuração das portas 
*********************************************************/
  void S4Power::Config ( ) 
  {
      //ToDo: Configurar as portas do arduino corretamente 
      this->M1.SetConf ( 3, 4, 9 );
      this->M2.SetConf ( 7, 8, 10 );
      this->M3.SetConf ( 12, 13, 11 );
      this->M4.SetConf ( 15, 16, 5 );
  
      this->light.SetConf ( 6, 0 );
  }
