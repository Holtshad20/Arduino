//SerialArdAsSlave2- Arduino as slave to host connected over serial line
//Complements Delphi program DD83

//version: 03 Aug 2010, tweaked after moved to html

/*MOSTLY working, some rough edges.

Worst(?) rough edge: If the program in the connected serial device
is running, e.g. DD83 is running in a Windows PC, then some
gibberish seems to be sent to the connected device if the Arduino
is reprogrammed while that other program runs.

This is harder to see in DD83 than it was in DD82.

This is the fourth step along a road to having an external
serial device talking with an Arduino. Get the material in
earlier programs working first, then proceed with this
program.

We have Arduino waiting for command from Master, then responding.

I worked with one PC running the Arduino Development
Environment, providing me with the means of programming
the Arduino, and of watching for the output of lines
like Serial.print"a", which passed to the Arduino through
the "programming" cable, the one on the Arduino lines
0 and 1, input to the Arduino, output from it, respectively.

I had the second PC connected to the Arduino via the
second PC's serial port, through a Wulfden PA4B TTL/RS-232
adapter, and what is labeled "Rx" on the PA4B was connected
to D3 of the Arduino, which was being used for OUTPUT. (The
"Rx" on the PA4B is relative to the big PC (or other serial
device "upstream" from the PA4B. The Arduino TRANSMITS (Tx)
while the other device RECEIVES "Rx"

Ever since the first step in developing what we have, the "Tx"
from the PA4B has been connected to D2 in the Arduino. The
Arduino RECEIVEs data via D2, from the PA4B's "Tx" line, fed
from the other serial device.

N.B.: The serial monitor shows BOTH what the Arduino is
SENDING and what the Arduino is RECEIVING. Messy, but works!
To use this, de-rem the Serial.print lines, and change
the speed at which the main loop in the master program is
executing.

You may have to "de-rem" some Serial.print lines in the Arduino code
to see the material promised for the Serial Monitor data stream.

At least SOME buffering is happening somewhere. Remember: I
believe I set the big PCs port up with no handshaking... in
any case, I don't know of any wires in my circuit to implement
it. Even so... if I increase the delay (below) to 1000, and
press XYZ very quickly on the big PC's keyboard, over the next
three seconds, the serial monitor displays aXaYaZ. Hurrah!
I think that NewSoftSerial buffered those data as they arrived,
holding them for when they were asked for.

You could make the bit inside...
if (mySerialPort.available()>0){}
... more clever, with a loop to process all that is incoming
before returning to the rest of "loop()", if you wanted to.
I didn't!

*/

#include <SoftwareSerial.h>
//Yes, NEWSoftSerial... I don't think SoftwareSerial has the
//critical "available()" function, does it?

#define LEDpin 13//no; here. Pin connected to 5v via
//    LED AND current limiting resistor.
#define SwMom 12//no; here. Pin connected to ground
//    via a momentary switch to ground.

#define SerInToArdu 2//no ; here expand rem
#define SerOutFrmArdu 3//no ; expand rem

SoftwareSerial mySerialPort(SerInToArdu,SerOutFrmArdu);//expand rem
byte bToSend=0;
byte bCmndRecd=255;//255 rogue for nothing received

void setup(){

pinMode(LEDpin,OUTPUT);

pinMode(SwMom,INPUT);
digitalWrite(SwMom,HIGH);//"Switch on" internal pull up

pinMode(SerInToArdu,INPUT);//Not actually needed... put in
   //to reassure users as to data direction over
   //serial lines
pinMode(SerOutFrmArdu,OUTPUT);

mySerialPort.begin(9600);

Serial.begin(9600);
};//end "setup()"

void loop(){
if (digitalRead(SwMom)==HIGH){
    //Serial.print("h");
    mySerialPort.print("h");}
  else{
    //Serial.print("w");
    mySerialPort.print("w");};

delay(10);

if (mySerialPort.available()>0){
   bCmndRecd=mySerialPort.read();
   delay(10);
   //Serial.print(bCmndRecd,BYTE);
   delay(10);
   if (bCmndRecd=='n')LEDOn();
   if (bCmndRecd=='f')LEDOff();

   ;
bCmndRecd=255;//Not "essential"... yet. Here.

};
};//end of "loop()"

void LEDOn(){
digitalWrite(LEDpin,LOW);//yes... LOW for ON
};

void LEDOff(){
digitalWrite(LEDpin,HIGH);//yes... HIGH for OFF
};
