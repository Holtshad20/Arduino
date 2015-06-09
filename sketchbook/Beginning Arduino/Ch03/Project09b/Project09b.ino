// Project 9b - Variar todo o expectro RGB. 
int ledPin1 = 8;
int ledPin2 = 9;
int ledPin3 = 10;

void setup()
{
  randomSeed(analogRead(0));
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop()
{
  int x = random(255);
  int y = random(255);
  int z = random(255);

  switch (random(2)) {
    case 0: shineled(ledPin1, x); 
    case 1: shineled(ledPin2, y);         
    case 2: shineled(ledPin3, z);         
  }
  delay(300);
}


void shineled(int ledPin, byte t) {
  	for (int x=0; x<180; x++) {
    	// convert degrees to radians then obtain sin value
    	analogWrite(ledPin, int((sin(x*(3.1412/180)))*t));
        delay(30);
  	}
}

