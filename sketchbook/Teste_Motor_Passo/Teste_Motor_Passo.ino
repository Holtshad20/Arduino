#define VOLTAS 11
boolean Dir  = LOW;


void setup() {                
  pinMode(2, OUTPUT);     
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, Dir);
  delay(300);
}


void loop() {
  digitalWrite(3, Dir);  
  for (int i=0; i < 48*VOLTAS; i++) {
       digitalWrite(2, HIGH);
       delay(2);          
       digitalWrite(2, LOW); 
       delay(2); 
     }     
  delay(300);    
  Dir = ~Dir;
}
