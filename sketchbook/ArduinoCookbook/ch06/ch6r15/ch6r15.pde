/*
  gyro sketch
  displays the rotation rate on the Serial Monitor
 */


const int inputPin = 0;      // analog input 0
const int powerDownPin = 15; // analog input 1 is digital input 15

int rotationRate = 0;

void setup()
{
  Serial.begin(9600);           // sets the serial port to 9600
  pinMode(powerDownPin, OUTPUT);
  digitalWrite(powerDownPin, LOW);  // gyro not in power down mode
}

void loop()
{
  rotationRate = analogRead(inputPin);       // read the gyro output
  Serial.print("rotation rate is ");
  Serial.println(rotationRate);
  delay(100);              // wait 100ms for next reading
}
      
    
