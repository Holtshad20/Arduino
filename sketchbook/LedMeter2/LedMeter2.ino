#define LED 3
#define POT 0
#define DELAY 20
int val = 0; 

void setup ()
{
  //Serial.begin(9600);
  pinMode (LED, OUTPUT); 
  delay(3000);
}

void loop ()
{
  //val = map(analogRead(POT), 0, 800, 0, 6);
  //Serial.println(val);
  //analogWrite (LED, val); 
  for (int x=0; x<7; x++) {
    analogWrite(LED, x);
    delay(DELAY);   
  }
  delay(500);
  
  for (int x=7; x>-1; x--) {
    analogWrite(LED, x);
    delay(DELAY);   
  }
  delay(500);
}
