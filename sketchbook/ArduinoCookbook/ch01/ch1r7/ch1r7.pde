const int ledPin =  13;     // LED connected to digital pin 13
const int sensorPin = 0;    // connect sensor to analog input 0

const int minDuration = 100;  // minimum wait between blinks
const int maxDuration = 1000;  // maximum wait between blinks

void setup()
{
  pinMode(ledPin, OUTPUT);  // enable output on the led pin
}


void loop()
{
  int sensorReading = analogRead(sensorPin);    // read the analog input
  int rate = map(sensorReading, 200,800,minDuration, maxDuration);
  rate = rate / 100;            // add this line for audio frequency
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(rate);                  // wait duration dependent on light level
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(rate);
}
