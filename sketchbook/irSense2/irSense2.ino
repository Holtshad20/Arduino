#define IRSENSE1 A0
#define IRTRANS1 2

#define IRSENSE2 A1
#define IRTRANS2 3

void setup() {
  Serial.begin(9600);
  
  pinMode(IRSENSE1, INPUT);
  pinMode(IRTRANS1, OUTPUT);
  digitalWrite(IRTRANS1, LOW);
  
  pinMode(IRSENSE2, INPUT);
  pinMode(IRTRANS2, OUTPUT);
  digitalWrite(IRTRANS2, LOW);
}

void loop(){
  Serial.print("Sensor1:");
  int valueSensor1 = getDist(IRTRANS1, IRSENSE1);
  Serial.print(valueSensor1);
  Serial.print(" cm");
  Serial.print("   ");
  
  Serial.print("Sensor2:");
  Serial.print(getDist(IRTRANS2, IRSENSE2));
  Serial.print(" cm.");
  
  Serial.println();
}


int getDist(byte irTrans, byte irSense) {
  digitalWrite(irTrans, HIGH);
  delay(20);
  int valueAmbient = analogRead(irSense);
  delay(20);
  
  digitalWrite(irTrans, LOW);
  delay(20);
  int value = analogRead(irSense);
  int value2 = round(value - valueAmbient);
  int dist = (((value2+15.0)/(value2*(value2+100.0)))*2000.0);
  delay(20);  
  
  return dist;
}

