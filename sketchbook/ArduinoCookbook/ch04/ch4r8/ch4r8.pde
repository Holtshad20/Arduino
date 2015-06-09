// Processing Sketch
              
/* SendingBinaryToArduino
 * Language: Processing
 */
import processing.serial.*;

Serial myPort;  // Create object from Serial class
public static final char HEADER = '|';
public static final char MOUSE  = 'M';

void setup()
{
  size(200, 400);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
}

void draw(){
}

void serialEvent(Serial p) {
  // handle incoming serial data
  String inString = myPort.readStringUntil('\n');
  if(inString != null) {
      println( inString );   // echo text string from Arduino
  }
}
      
    
