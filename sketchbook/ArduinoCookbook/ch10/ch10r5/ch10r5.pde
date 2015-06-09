/*
 OptoRemote sketch
 A switch connected to pin 2 turns a device on and off using optocouplers
 The outputs are pulsed for half a second when the switch is pressed or released
*/
const int inputPin     = 2;         // input pin for the switch
const int remoteOnPin  = 3;         // output pin to turn the remote on
const int remoteOffPin = 4;         // output pin to turn the remote off
const int PUSHED       = LOW;       // value when button is pressed
boolean isOn;                       // variable stores the last command

void setup() {
 Serial.begin(9600);
 pinMode(remoteOnPin, OUTPUT);
 pinMode(remoteOffPin, OUTPUT);
 pinMode(inputPin, INPUT);
 digitalWrite(inputPin,HIGH);  // turn on internal pull-up on the inputPin
}
void loop(){
 int val = digitalRead(inputPin);  // read input value
 // if the switch is pushed then switch on if not already on
 if( val == PUSHED)
 {
    if(isOn != true)       // if it's not already on, turn the remote button on
    {
     pulseRemote(remoteOnPin);
     isOn = true;                // remember that the remote is now on
    }
 }
 // if the switch is not pushed then switch off if not already off
 else
 {
    // here if the button is not pushed
    if(isOn == true)               // if it's on, turn the remote button off
    {
     pulseRemote(remoteOffPin);
     isOn = false;                // remember that the remote is now off
    }
 }
}
// turn the optocoupler on for half a second to blip the remote control button
void pulseRemote(int pin )
{
 digitalWrite(pin, HIGH);     // turn the optocoupler on
 delay(500);                  // wait half a second
 digitalWrite(pin, LOW);      // turn the optocoupler off
}
