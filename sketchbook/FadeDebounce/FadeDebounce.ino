/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 
*/
const int ledpin = 9;    // Led no pino 9
const int buttonPin = 2;     // botão no pino 2

int brightness = 0;      // how bright the LED is
int fadeAmount = 5;      // how many points to fade the LED by

int ledState = HIGH;     // estado do pino 9
int buttonState;            // leitura do pino 2
int lastButtonState = LOW;  // leitura anterior do pino 2

long lastDebounceTime = 0;  // a última vez que o botão(pin 2) fou pressionado
long debounceDelay = 50;    

void setup()  { 
  // declare pin ledpin to be an output:
  pinMode(ledpin, OUTPUT);
  pinMode(buttonPin, INPUT);
} 

void loop()  { 
  // ler o estado do botão
  int reading = digitalRead(buttonPin);
  
  // checar o estado do botão, ou seja, se mudou o estado do botão
  if (reading != lastButtonState) {
     lastDebounceTime = millis(); // inicializa o debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
     buttonState = reading; 
  }
  
  
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


