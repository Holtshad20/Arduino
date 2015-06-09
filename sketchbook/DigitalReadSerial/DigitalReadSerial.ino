/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */
 
int ledpin = 9;        // Led na saida 9
int pinSensor1 = 2;    // chave na entrada 2

void setup() {
  Serial.begin(9600);
  // sensor1
  pinMode(pinSensor1, INPUT);
  // declare pin ledpin to be an output:
  pinMode(ledpin, OUTPUT);
  
}

void loop() {
  int sensorValue = digitalRead(pinSensor1);
  Serial.println(sensorValue);
  if (sensorValue == 1) fadeLed();
}

void fadeLed()  { 

  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by

  // set the brightness of pin ledpin:
  analogWrite(ledpin, brightness);   
  
  for (brightness = 0; brightness < 255; brightness = brightness + fadeAmount) {
      analogWrite(ledpin, brightness);
      delay(30);
  }

  delay(100);
  
  for (brightness = 255; brightness > 0; brightness = brightness - fadeAmount) {
      analogWrite(ledpin, brightness);
      delay(30);
  }    

  analogWrite(ledpin, brightness);     
}


