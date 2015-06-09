/*
  AnalogReadSerial
 Reads an analog input on pin 0, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */
void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  int thisPitch = map(sensorValue, 0, 1023, 300, 1000);
  tone(2, thisPitch, 10);
}
