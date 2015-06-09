const int speakerPin =  9;  // Speaker connected to digital pin 9
const int sensorPin = 0;    // connect sensor to analog input 0

// the next two lines set the min and max delay between blinks
const int minDuration = 100;  // minimum wait between blinks
const int maxDuration = 1000;  // maximum wait between blinks


void setup()
{
  pinMode(speakerPin, OUTPUT);  // enable output on the pin
}

void loop()
{
  int rate = analogRead(sensorPin);    // read the analog input
  // the next line scales the pulse rate between the min and max values
  rate = map(rate, 200,800,minDuration, maxDuration); // convert to pulse rate
  digitalWrite(speakerPin, HIGH);   // turn pin on to move the speaker cone
  delay(rate);                      // wait duration dependent on light level
  digitalWrite(speakerPin, LOW);    // turn the pin off
  delay(rate);
}