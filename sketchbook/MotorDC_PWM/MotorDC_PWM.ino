/*
  Variar a velocidade de um motor DC
 Entrada: A3  - controlada por um pot
 Saída:   9   - ligado o motor e um led
 
 */
const int pinIn = A3;
const int pinOut = 9;

void setup() {
  Serial.begin(9600);  
}



void loop() {
  int valIn = 0;
  
  // variar a velocidade do motor conforme movimento do potenciometro ;
  analogWrite(pinOut, valIn = map(analogRead(pinIn),0,1023,0,255));  
  Serial.println((valIn*100)/255);
}

