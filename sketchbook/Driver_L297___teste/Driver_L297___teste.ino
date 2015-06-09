#define DELAY 1 // delay passos
#define DELAY2 60 // delay para mudança de direçao
#define PASSOS 48 // numero de passos
#define VOLTAS 5
#define CW LOW // Sentido horario
#define CCW HIGH // Sentido anti-horario
#define ENA LOW // ENA = 1 Habilita o L297


void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  pinMode(8, OUTPUT);
  

  digitalWrite(8, LOW); 
}



void loop() {
  digitalWrite(3, CW);
  for (int i=0; i<PASSOS*VOLTAS; i++) {
    digitalWrite(2, HIGH);
    delay(DELAY);
    digitalWrite(2, LOW);
    delay(DELAY);    
  }
  
  delay(DELAY2);

  digitalWrite(5, CW);
  for (int i=0; i<PASSOS*VOLTAS; i++) {
    digitalWrite(4, HIGH);
    delay(DELAY);
    digitalWrite(4, LOW);
    delay(DELAY);    
  }
  
  delay(DELAY2);

  digitalWrite(3, CCW);
  for (int i=0; i<PASSOS*VOLTAS; i++) {
    digitalWrite(2, HIGH);
    delay(DELAY);
    digitalWrite(2, LOW);
    delay(DELAY);    
  }
  
  delay(DELAY2);

  digitalWrite(5, CCW);
  for (int i=0; i<PASSOS*VOLTAS; i++) {
    digitalWrite(4, HIGH);
    delay(DELAY);
    digitalWrite(4, LOW);
    delay(DELAY);    
  }
  
  delay(DELAY2);


}

