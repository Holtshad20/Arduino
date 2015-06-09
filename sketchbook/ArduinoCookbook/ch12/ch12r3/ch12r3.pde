/*
  PulseIn sketch
  uses pulseIn to display how long a switch is pressed and released
 */


const int inputPin = 2;         // input pin for the switch

long val;

void setup()
{
  pinMode(inputPin, INPUT);
  digitalWrite(inputPin,HIGH);  // turn on internal pull-up on the inputPin
  Serial.begin(9600);
  Serial.println("Press and release switch");
}

void loop()
{

  val = pulseIn(inputPin, LOW);
  if( val != 0)  // timeout returns 0
  {
    Serial.print("switch pressed for ");
    Serial.print(val);
    Serial.println(" microseconds");
  }
}
      
    
