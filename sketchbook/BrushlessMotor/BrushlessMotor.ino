/*
  Arduino Brushless motor hd
*/

int phase1 = 3;
int phase2 = 5;
int phase3 = 6;
int led = 12;

unsigned long stepLength = 40000;
int minStepLength = 1400;
int steps = 5;

void setup() {
  pinMode(led, OUTPUT);  
  pinMode(phase1, OUTPUT);
  pinMode(phase2, OUTPUT);
  pinMode(phase3, OUTPUT);
  digitalWrite(led, LOW); 
}

void loop() {
  switchStep(1);
  switchStep(2); 
  switchStep(3);
  
  if(stepLength > minStepLength)
  {
    stepLength = stepLength - steps;
  } else {
    // set the minimum pulse length
    stepLength=minStepLength;
  }
  
  if (stepLength < 39950) {
    digitalWrite(led, HIGH);   // second gear
    steps = 300;
  }
   
  if (stepLength < 20000) {
    digitalWrite(led, LOW);      // third gear
    steps = 50;
  }

  if (stepLength < 3000) {
    digitalWrite(led, HIGH);      // fourth gear
    steps = 2;
  }
}


void switchStep(int stage)
{
  switch(stage)
  {
  case 1:
    digitalWrite(phase1, HIGH);
    digitalWrite(phase2, LOW);
    digitalWrite(phase3, LOW);
    myDelay(stepLength);
    break;
    
  case 2:
    digitalWrite(phase1, LOW);
    digitalWrite(phase2, HIGH);
    digitalWrite(phase3, LOW);
    myDelay(stepLength);
    break;
    
  default:
    digitalWrite(phase1, LOW);
    digitalWrite(phase2, LOW);
    digitalWrite(phase3, HIGH);
    myDelay(stepLength);
    break;
  }  


}

void myDelay(unsigned long p) {
if (p > 16380) {
  delay (p/1000);
  } else {
  delayMicroseconds(p);
  }
}
