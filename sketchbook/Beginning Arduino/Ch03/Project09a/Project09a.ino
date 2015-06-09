// Project 9 - LED Fire Effect
int ledPin1 = 8;
int ledPin2 = 9;
int ledPin3 = 10;
int tempo = 1000;

void setup()
{
  randomSeed(analogRead(0));
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop()
{
  switch (random(2)) {
    case 0: shineled(ledPin1);
    case 1: shineled(ledPin2);
    case 2: shineled(ledPin3);
  }
}


void shineled(int ledPin) {
  	for (int x=0; x<180; x++) {
    	// convert degrees to radians then obtain sin value
    	analogWrite(ledPin, int((sin(x*(3.1412/180)))*255));
        delay(10);
  	}
}

