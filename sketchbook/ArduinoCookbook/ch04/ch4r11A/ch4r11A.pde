// Processing Sketch
              
/**
 * GoogleEarthFS_P
 *
 * Read Arduino Serial packets
 * and send mouse position to Google Earth
 *
 */


import processing.serial.*;

Serial myPort;       // Create object from Serial class
int portIndex = 1;   // select the com port, 0 is the first port
int HEADER = 255;
int val;             // Data received from the serial port

GoogleFS myGoogle;

void setup()
{
  size(256, 256);
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this,Serial.list()[portIndex], 9600);
  myGoogle = new GoogleFS();
  smooth();
  fill(255);
  background(255);
  println("Start Google FS in the center of your screen");
  println("center the mouse pointer in google earth and press Z button to start");
  do{
    getData();
  }
  while(buttons != 2 ); // wait for data and Z button to start

  println("started");
  myGoogle.mousePress(InputEvent.BUTTON1_MASK); // starts the FS
}

int accx,accy,buttons;
int xOffset, yOffset = 0;

boolean getData(){
  if ( myPort.available() >= 4) {  // If a data packet is available,
    if(myPort.read() == HEADER){  // check for header
      // erase the old markers
      stroke(255);
      ellipse(accx, accy,4,4);

      accx = myPort.read();
      accy = myPort.read();
      buttons = myPort.read();
      if(xOffset == 0){
        // here if first time
        xOffset = accx;
        yOffset = accy;
      }
      if( buttons == 1) {
        println("tbutton: c");
        xOffset = accx;
        yOffset = accy;
      }
      if(buttons == 3 ){
        exit();
      }
      return true;  // data available
    }
  }
  return false;
}

void draw()
{
  if(getData()){
    if(buttons != 2 ){
      // only activate the mouse when the Z button is not pressed
      myGoogle.move(accx- xOffset, accy - yOffset);
     }

    print("accx: ");     print(accx);
    print("\taccy: ");   print(accy);
    println();

    stroke(255,0,0);     ellipse(accx, accy,4,4);
  }
}

class GoogleFS {
  Robot myRobot;     // create object from Robot class;
  int centerX,centerY;
  GoogleFS(){
    try {
      myRobot = new Robot();
    }
    catch (AWTException e) {
      e.printStackTrace();
    }
    Dimension screen = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
    centerY =  (int)screen.getHeight() / 2 ;
    centerX =  (int)screen.getWidth() / 2;
  }
  // moves mouse from center of screen by given offset
  void move(int offsetX, int offsetY){
    myRobot.mouseMove(centerX + 4 * offsetX,centerY + -4 * offsetY);
  }
  void mousePress( int button){
    myRobot.mousePress(button) ;
  }
}
      
    
