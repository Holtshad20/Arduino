#define irSense A0

void setup() {
  pinMode(irSense, INPUT);
  Serial.begin(9600);
}

void loop() {
  int Value = analogRead(irSense);
  delay(20);
  if (Value > 1005) 
     Serial.println(Value);
}

