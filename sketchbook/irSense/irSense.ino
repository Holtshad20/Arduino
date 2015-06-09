int IRSense = A0;
int IRTrans = 2;
int valueAmbient = 0;

void setup() 
{
  pinMode(IRSense, INPUT);
  pinMode(IRTrans, OUTPUT);
  Serial.begin(9600);
  digitalWrite(IRTrans,LOW);
}

void ambient()
{
  digitalWrite(IRTrans,HIGH);
  delay(20); 
  valueAmbient = analogRead(IRSense);
  delay(20);
}

void loop() 
{
  ambient();
  digitalWrite(IRTrans,LOW);
  delay(20);
  int value = analogRead(IRSense);
  int value2 = round(value - valueAmbient);
  int dist = (((value2+15.0)/(value2*(value2+100.0)))*2000.0);
  Serial.print(dist);
  Serial.print("cm.");
  Serial.println("");
  delay(20);
}
