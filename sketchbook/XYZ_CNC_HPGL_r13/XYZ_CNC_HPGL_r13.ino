 // Stepper Motor Control Program
// For an XYZ stepper motor controller
// Receives serial commands from PC, controls 3 motor CNC machine
// Interprets basic HPGL style instructions  
// Tom Wilson 2011

#define INCH 100 //steps per inch of the machine = threads per inch * steps per rotation 
#define RPM 30 //speed of the motors
#define SER_HEADER 'G' // Header tag for serial message

// Adafruit Motor shield library
// modified by tomw to drive 2 shields at once
// This requires a modified library 
//with the second having MOTORLATCH on pin 13, renamed MOTORLATCHA
//and all functions renamed to A versions
#include <AFMotor.h>

//Set up the steps per revolution and location of motors 
AF_Stepper motorX(48, 1);
AF_Stepper motorY(48, 2);

// Set up the basic machine position variable
signed long int Xpos = 0;
signed long int Ypos = 0;
signed long int Zpos = 0;

// Set up the destination machine position variable
signed long int newXpos = 0;
signed long int newYpos = 0;
signed long int newZpos = 0;

// Set up global place to keep serial values
signed long int newSerialData;
bool XorY = false;

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps

  //set the default speed of the motors
  motorX.setSpeed(RPM);
  motorY.setSpeed(RPM);

  delay(1000); // wait for the supply to come up

  //step motors one step and back to power the coils
  motorX.step(1, FORWARD, SINGLE); 
  motorY.step(1, FORWARD, SINGLE);
  motorX.step(1, BACKWARD, SINGLE); 
  motorY.step(1, BACKWARD, SINGLE);

  // Home position
  Xpos = 0;
  Ypos = 0;
  Zpos = 0;
  newXpos = 0;
  newYpos = 0;
  newZpos = 0;

  Serial.println("Stepper Controller Online");
  Serial.print(Xpos);
  Serial.print(":");
  Serial.print(Ypos); 
  Serial.print(":");
  Serial.print(Zpos);
  Serial.println(";");  // ; is for serial flow control
}

void loop() { 
  while(Serial.available() ) {
    // Loop simply accepts commands from serial and executes them
    readHPGLcommand();               //read the command 

  }
}


void readHPGLcommand() {
  // reads serial port for two letter HPGL command and optional coords that follow
  char c;
  bool coord_avail;

  //make sure there is enough data there for form a command, at least "IN;"
  if(Serial.available()>=3){
    
    c = Serial.read();
   
    // Interpret HPGL command, ingnore unsupported commands and whitespace
    //http://paulbourke.net/dataformats/hpgl/    has definitions
    switch( c ) {
      case 'P' : 
          switch( Serial.read() ) {
            case 'U' :  // PU Pen Up
              zMove(-500);
              // expect either nothing or pairs of coords to follow
              XorY = true;
              coord_avail = true;
              while( coord_avail ) {    
                coord_avail = readSerialNumber();
                // hack to alternate reading values as X or Y
                if ( XorY) { 
                  newXpos = newSerialData;
                  XorY = false;
                } else {
                  newYpos = newSerialData;
                  XorY = true; 
                  linearInterpolationMove( newXpos, newYpos);
                }  
              }
              break;
            case 'D' :  // PD Pen Down
              zMove(0);
              coord_avail = false;   
              // expect either nothing or pairs of coords to follow              
              XorY = true;
              coord_avail = true;  //need to make sure the loop goes at least once
              while( coord_avail ) {    
                coord_avail = readSerialNumber();
                // hack to alternate reading values as X or Y
                if ( XorY) { 
                  newXpos = newSerialData;
                  XorY = false;
                } else {
                  newYpos = newSerialData;
                  XorY = true; 
                  // only valid coord if two values were found
                  linearInterpolationMove( newXpos, newYpos);
                }  
              }
    
              break;
            case 'G' :  // PG Page Feed
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              Serial.println("PG;");
              // Command doesn't do anything yet
              break;
            case 'T' :  // PG Pen Thickness
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              // expects one number to be returned, indicating pen
              // currently not doing anything with this command
              Serial.print("PT;");
              Serial.println(newSerialData);
              break;
            case 'R' :  // Pen Relative Move
              zMove(0);
              coord_avail = false;   
              // expect either nothing or pairs of coords to follow              
              XorY = true;
              coord_avail = true;  //need to make sure the loop goes at least once
              while( coord_avail ) {    
                coord_avail = readSerialNumber();
                // hack to alternate reading values as X or Y
                if ( XorY) { 
                  newXpos = newSerialData;
                  XorY = false;
                } else {
                  newYpos = newSerialData;
                  XorY = true; 
                  // only valid coord if two values were found
                  linearInterpolationMove( newXpos+Xpos, newYpos+Ypos);
                }  
              }
    
              break; 
            default:
              Serial.println("WARNING: Px ignored;");
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              break;
          }
          break;
      case 'I' : 
          switch( Serial.read() ) {
            case 'N' :  // IN Initialize
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              // Home position
              Xpos = 0;
              Ypos = 0;
              Zpos = 0;
              Serial.print(Xpos);Serial.print(":");Serial.print(Ypos); Serial.print(":");Serial.print(Zpos);Serial.println(";");  // ; is for serial flow control
              break;
            default :
              Serial.println("WARNING: Ix ignored;");
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              break;
          }
          break;
      case 'S' : 
          switch( Serial.read() ) {
            case 'P' :  // SP Select Pen
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              // expects one number to be returned, indicating pen
              Serial.print("Pen: "); 
              Serial.println(newSerialData);
              //Serial.print(Xpos);Serial.print(":");Serial.print(Ypos); Serial.print(":");Serial.print(Zpos);Serial.println(";");  // ; is for serial flow control
              // currently not doing anything with this command
              break;
            default :
              Serial.println("WARNING: Sx ignored;");
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              break;
          }
          break;    
        case 'Z' : 
          switch( Serial.read() ) {
            case 'A' :  // ZR move relative Z position
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              // expects one number to be returned, indicating distance
              zMove(newSerialData);
              break;
            case 'R' :  // ZR move relative Z position
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              // expects one number to be returned, indicating distance
              zMove(newSerialData+Zpos);
              break;
            default :
              Serial.println("WARNING: Zx ignored;");
              while( readSerialNumber() ) {}    //keep reading until coords or ';' is found
              break;
          }
          break;            
      default : 
          //Whitespace or unrecognized letter, ignore it 
          break;
    } //cmd switch
  } //if serial 
}


bool readSerialNumber() {
//read coords from serial input 
//commands end with ';' and lists of coords are separated by ',' 
//
// Three things can happen and need to differentiate
// 1) no coords at all, just a ; - need to return right away and break the calling while loop
// 2) a coordinate followed by , - need to output the coord and continue
// 3) a coordinate followed by ; - need to output the coord and break the calling while loop

  signed long int coord = 0;   //temp storage for coords from serial
  signed int sign = 1;         //temp storage for coord sign from serial
  char c;  
         
       while ( c != ',' && c != ';'){ 
         c = Serial.read(); 
         if( c == '-' ) sign = -1; // capture the sign 
         if( c >= '0' && c <= '9'){ 
           coord = (10 * coord) + (c - '0') ; // convert digits to a number 
         }
       }
      
      //put the data in the top level variable
      newSerialData = coord * sign;
           
      //detect the end of command ';' after the last coord
      if( c == ';') return false;
      return true;
}


void zMove (signed long int newZ) {
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
      if ((nextX-Xpos) >=  1)  motorX.step((nextX - Xpos), FORWARD, SINGLE); 
      if ((nextX-Xpos) <= -1)  motorX.step((Xpos - nextX), BACKWARD, SINGLE);
      if ((nextY-Ypos) >=  1)  motorY.step((nextY - Ypos), FORWARD, SINGLE); 
      if ((nextY-Ypos) <= -1)  motorY.step((Ypos - nextY), BACKWARD, SINGLE);
    
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
      if ((newX-Xpos) >  0)  motorX.step((newX - Xpos), FORWARD, SINGLE); 
      if ((newX-Xpos) < 0)  motorX.step((Xpos - newX), BACKWARD, SINGLE);
      if ((newY-Ypos) >  0)  motorY.step((newY - Ypos), FORWARD, SINGLE); 
      if ((newY-Ypos) < 0)  motorY.step((Ypos - newY), BACKWARD, SINGLE);
  }
    //update the machine current position
    Xpos = newX;
    Ypos = newY;
    
    Serial.print(Xpos);Serial.print(":");Serial.print(Ypos); Serial.print(":");Serial.print(Zpos);Serial.println(";");  // ; is for serial flow control

 
}


