/*======================By:tudodearduino.blogspot.com======================*/
#include <IRremote.h>
int RECV_PIN = A0; // receptor IR conectado no pino 11

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600); // Abre a porta serial.
  pinMode(7, OUTPUT); // Define o pino 7 como saida.
  irrecv.enableIRIn(); // Inicia o receptor IR.
 

}
void loop(){
  if (irrecv.decode(&results)) {
    // Envia o codigo do botão pressionado para a porta serial.    
    Serial.println(results.value, HEX); 
    irrecv.resume();   
  }
  // Se o receptor IR receber o codigo "70" ira Acionar o Relé.  
  if (results.value == 70){ digitalWrite(7,HIGH);} 
  // Se o receptor IR receber o codigo "60" ira Desligar o Relé. 
   if (results.value == 60){ digitalWrite(7,LOW);} 

}
