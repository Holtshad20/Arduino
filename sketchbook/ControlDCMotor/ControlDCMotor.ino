/*
  Variar a velocidade de um motor DC
  Entrada: A0  - controlada por um pot
  Saída:   3   - ligado o motor e um led

*/
#define pinIn A0
#define pinOut 3
#define encoderA 2

volatile long count = 0;

void setup() {
    Serial.begin(115600);
    pinMode(pinOut, OUTPUT);
   
    attachInterrupt(0, ReadEncoderA, CHANGE);
    
}

void loop() {
  // variar a velocidade do motor ;
  analogWrite(pinOut, map(analogRead(pinIn),0,1023,0,255));
  
  Serial.println(count, DEC);
  delay(20);
 
}

void ReadEncoderA()  {                   
  count++;
}
