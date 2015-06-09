/*

Program to intepret CNC commands and activate motors
Uses Adafruit v2 motor shields http://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/overview

Commands that will be received:
R; = Reset position counters to 0:0:0 (Set Home)
C; = Coordinates:  Report back the current stepper counter position over the serial link for use in calibrating and catching errors
Pn; = Z axis relative move n steps
Zn; = Move to absolute Z coord
Mx:y; = Move relative by x:y steps
Ax:y; = Move to absolute coordinate x:y
Tn; = Set tool speed to n.  May be positive or minus.  0 is off.
S; = Stop all motors
I; = Power all motors/ initialize

After each command the Arduino will send back characters:
B; - Arduino received the command and is busy executing it
R; - Ready for a new command
Cx:y:z - Arduino counters say this is the current coordinates

 */
 
#define INCH 4000 //steps per inch of the machine = threads per inch * steps per rotation 
#define RPM 30 //speed of the stepper motors

#include <Wire.h>
#include <Adafruit_MotorShield.h> 
//#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address for Z axis stepper and Tool
Adafruit_MotorShield AFMS_ZT = Adafruit_MotorShield(); 
// Or, create it with a different I2C address for stacking for X and Y axis steppers
Adafruit_MotorShield AFMS_XY = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *stepperZ = AFMS_ZT.getStepper(200, 2);
// And connect a DC motor to port M1
Adafruit_DCMotor *myTool = AFMS_ZT.getMotor(1);
// Connect two stepper motor with 200 steps per revolution (1.8 degree)
// to motor ports on bottom shield
Adafruit_StepperMotor *stepperX = AFMS_XY.getStepper(200, 1);
Adafruit_StepperMotor *stepperY = AFMS_XY.getStepper(200, 2);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
long previousMillis = 0;        //used for timeout counter
long timeout = 10000;           //timeout length

// Set up the basic machine position variables
signed long int Xpos = 0;
signed long int Ypos = 0;
signed long int Zpos = 0;

// Set up the destination machine position variables
signed long int newXpos = 0;
signed long int newYpos = 0;
signed long int newZpos = 0;

void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
  inputString.reserve(200);
  establishContact();  // send a serial byte to establish contact until receiver responds 
  
  //Start the PWM
  AFMS_XY.begin();  // create with the default frequency 1.6KHz
  AFMS_ZT.begin();  // create with the default frequency 1.6KHz
  
  //initialize motors
  initMotors();
  
  // Home position
  resetPositionCounter();
  
  Serial.println("Stepper Controller Online;");
  reportCoords();
  
}

void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
  
    // get the new byte:
    char inChar = (char)Serial.read(); 

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == ';') {
      stringComplete = true;
    } else {
      // add it to the inputString:
      inputString += inChar;
    }
    
    // emergency stop command, process immediatly no matter what I'm doing
    if (inChar == '*') emergencyStop();
  
    if (stringComplete) {
      //Indicate busy 
      Serial.print("B;"); 
      previousMillis = millis();         //set the timeout counter to start

      //Echo the command
      //Serial.print(inputString);
  
      //decode and do what the command said
      switch( inputString[0] ) {
        case '*' : emergencyStop(); break;
        case 'I' : initMotors();  break;
        case 'T' : setToolSpeed();  break;
        case 'C' : reportCoords();  break;
        case 'P' : relativeZMove(); break;
        case 'Z' : absoluteZMove(); break;
        case 'M' : relativeXYMove(); break;
        case 'A' : absoluteXYMove(); break;
        case 'R' : resetPositionCounter(); break;
        default  : break;
      }
       
      //clear the command
      inputString = "";
      stringComplete = false;
  
      // Indicate ready
      Serial.print("R;"); 
      
    }  

  } 
}

//    else if ((previousMillis+timeout) > millis()) {
//       //Waited too long for the end of the command
//        intputString = "";
//        stringComplete = false;
//       Serial.println("E;");
//    } 

void establishContact() {   //for initial identification
  while (Serial.available() <= 0) {
    Serial.print("~");   // send a charachter to identify myself
    delay(300);
  }
}

void emergencyStop() {   //release all motors, limit hit or user intervention
      myTool->run(RELEASE);
      stepperX->release();
      stepperY->release();
      stepperZ->release();
      //Serial.println("All motors released;");
}

void setToolSpeed() {  
    signed long int newSpeed = extractCoord(inputString, 0);
    
    if (newSpeed>0) myTool->run(FORWARD);
    else myTool->run(BACKWARD);
    
    if (newSpeed == 0) myTool->run(RELEASE);
    else myTool->setSpeed(abs(newSpeed));
}


signed long int extractCoord(String numberString, int pos) {  //pulls the signed coord from the serial input string
  // pos indicates first or second coord desired
  
  int splitPos = 0;   //location of the : character
  signed long int coord = 0;   //temp storage for coords 
  signed int sign = 1;         //temp storage for coord sign 
  char c;  
  
  //First find the position of the :
    for (int i=1; i<numberString.length(); i++) {
      if( numberString[i] == ':' ) splitPos = i ; 
  }
    
  if (splitPos ==0)  {  // there is only one coord  
    for (int i=1; i<numberString.length(); i++) {
      c = numberString[i];
      if( c == '-' ) sign = -1; // capture the sign 
      if( c >= '0' && c <= '9') coord = (10 * coord) + (c - '0') ; // convert digits to a number     
    }
  } else {
  
    switch (pos) {
      case 0 :
      for (int i=1; i<splitPos; i++) {
          c = numberString[i];
          if( c == '-' ) sign = -1; // capture the sign 
          if( c >= '0' && c <= '9') coord = (10 * coord) + (c - '0') ; // convert digits to a number     
      }
      break;
      case 1: 
        for (int i=splitPos; i<numberString.length(); i++) {
          c = numberString[i];
          if( c == '-' ) sign = -1; // capture the sign 
          if( c >= '0' && c <= '9') coord = (10 * coord) + (c - '0') ; // convert digits to a number     
        }  
      break;
      default: //something is wrong, return 0;
      break;
    }
      
  }  
  return(coord * sign);
}
  
void resetPositionCounter() {
  Xpos = 0;
  Ypos = 0;
  Zpos = 0; 
  newXpos = 0;
  newYpos = 0;
  newZpos = 0;
}  

void reportCoords() {
    Serial.print(Xpos);Serial.print(":");Serial.print(Ypos); Serial.print(":");Serial.print(Zpos);Serial.println(";");  
}

void initMotors() {
  //initialize motors
  myTool->setSpeed(RPM);   
  myTool->run(RELEASE);
  stepperX->setSpeed(RPM);  
  stepperY->setSpeed(RPM);    
  stepperZ->setSpeed(RPM);    
  
  //step motors one step and back to power the coils and hold
  stepperX->step(1, FORWARD, SINGLE); 
  stepperY->step(1, FORWARD, SINGLE);
  stepperZ->step(1, FORWARD, SINGLE); 
  stepperX->step(1, BACKWARD, SINGLE); 
  stepperY->step(1, BACKWARD, SINGLE);
  stepperZ->step(1, BACKWARD, SINGLE); 
}

void relativeZMove(){
    zMove(Zpos + extractCoord(inputString, 0));
}

void absoluteZMove(){
    zMove(extractCoord(inputString, 0));
}

void relativeXYMove(){
    linearInterpolationMove(Xpos+extractCoord(inputString, 0), Ypos+extractCoord(inputString, 1));
}

void absoluteXYMove(){
    linearInterpolationMove(extractCoord(inputString, 0), extractCoord(inputString, 1));
}

void linearInterpolationMove ( signed long int newX, signed long int newY) {
  float distance = 0;
  int stepnum = 0;
  signed long int nextX;
  signed long int nextY ;
  signed long int oldX = Xpos;
  signed long int oldY = Ypos;

  //find the hypotenuse, the total distance to be traveled
  distance = sqrt((newX - oldX)*(newX - oldX) + (newY - oldY)*(newY - oldY) ) + 0.5;

  //round to integer number of steps that distance. Step by two to minimize 0 size steps.
  for (stepnum=0; stepnum <= distance; stepnum++) {
  
    //calculate the nearest integer points along the way 
    nextX = oldX + stepnum/distance*(newX-oldX);
    nextY = oldY + stepnum/distance*(newY-oldY);

    //move machine to that new coordinate, if 0 delta, don't move
    if ((distance < 4*INCH) && (distance > -4*INCH)) { //trap crazy values
      if ((nextX-Xpos) >=  1)  stepperX->step((nextX - Xpos), FORWARD, SINGLE); 
      if ((nextX-Xpos) <= -1)  stepperX->step((Xpos - nextX), BACKWARD, SINGLE);
      if ((nextY-Ypos) >=  1)  stepperY->step((nextY - Ypos), FORWARD, SINGLE); 
      if ((nextY-Ypos) <= -1)  stepperY->step((Ypos - nextY), BACKWARD, SINGLE);
    
    //update the machine current position
    Xpos = nextX;
    Ypos = nextY;
    } else {
      Serial.println("ERROR! Distance too big");
      break;
    }
  }
  
  // nudge to the exact final desired coord to correct for rounding down errors
  if ((abs(newX-Xpos)<6) && (abs(newY-Ypos)<6)) { //trap crazy values
      if ((newX-Xpos) >  0)  stepperX->step((newX - Xpos), FORWARD, SINGLE); 
      if ((newX-Xpos) < 0)  stepperX->step((Xpos - newX), BACKWARD, SINGLE);
      if ((newY-Ypos) >  0)  stepperY->step((newY - Ypos), FORWARD, SINGLE); 
      if ((newY-Ypos) < 0)  stepperY->step((Ypos - newY), BACKWARD, SINGLE);
  }
    //update the machine current position
    Xpos = newX;
    Ypos = newY;
}

void zMove (signed long int newZ) {
  //calculates steps to move, and moves, to new Z position requested
  signed long int distance = 0;
  signed long int oldZ = Zpos;

  distance = newZ - oldZ;
  
  if ((distance < 8000) && (distance > -8000)) {
    if (distance >=  1)  stepperZ->step(distance, FORWARD, SINGLE); 
    if (distance <= -1)  stepperZ->step(-1*distance, BACKWARD, SINGLE);
  } else {
    //movement requested is large and likely an error
    Serial.println("ERROR - Z axis range");
  }

  Zpos = newZ;  //update machine current position
}


