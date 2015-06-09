// Processing Sketch
              
//CommaDelimitedInput.pde (Processing Sketch)

import processing.serial.*;

Serial myPort;        // Create object from Serial class
char HEADER = 'H';    // character to identify the start of a message
short LF = 10;        // ASCII linefeed
short portIndex = 0;  // select the com port, 0 is the first port

void setup() {
  size(200, 200);

  // WARNING!
  // If necessary, change the definition of portIndex at the top of this
  // sketch to the desired serial port.
  //
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this,Serial.list()[portIndex], 9600);
}

void draw() {

}

void serialEvent(Serial p)
{
  String message = myPort.readStringUntil(LF); // read serial data

  if(message != null)
  {
    print(message);
    String [] data  = message.split(","); // Split the comma-separated message
    if(data[0].charAt(0) == HEADER)       // check for header character in the
                                          // first field
    {
      for( int i = 1; i < data.length-1; i++) // skip the header 
                                              // and terminating cr and lf
      {
        int value = Integer.parseInt(data[i]);
        println("Value" +  i + " = " + value);  // Print the value for
                                                // each field
      }
      println();
    }
  }
}
      
    
