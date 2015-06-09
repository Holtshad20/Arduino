/******************************************************************
 * PID Simple Example (Augmented with Processing.org Communication)
 * Version 0.2
 * by Marcelo Moraes
 * License: Free
 * April 2013
 * Sorocaba - São Paulo - Brazil
 ******************************************************************/



// Library definition
#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output; // these are just variables for storing values
int inputPin = 0; // Photo resistor input
int outputPin = 3; // LED output
int pot = 1; // Potentiometer input

// Tuning parameters
float Kp=0; //Initial Proportional Gain 
float Ki=10; //Initial Integral Gain 
float Kd=0; //Initial Differential Gain 

// Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,Kp,Ki,Kd, DIRECT);

// Timestamp
unsigned long serialTime; //this will help us know when to talk with processing
const int sampleRate = 1; // Variable that determines how fast our PID loop runs
const long serialPing = 500; //This determines how often we ping our loop
unsigned long now = 0; //This variable is used to keep track of time
unsigned long lastMessage = 0; //This keeps track of when our loop last spoke to serial


void setup()
{
  Input = analogRead(inputPin); // Read in light level
  Setpoint = analogRead(pot); // Read the SetPoint value from the potentiometer
  
  Serial.begin(9600); //Start a serial session
  myPID.SetMode(AUTOMATIC); // Turn on the PID loop as automatic control
  myPID.SetSampleTime(sampleRate); // Sets the sample rate
  lastMessage = millis(); // timestamp
}


void loop(){
  Input = analogRead(inputPin); // Read in light level
  Setpoint = analogRead(pot); // Read our setpoint light Level from the potentiometer 
  myPID.Compute(); // Run the PID loop
  analogWrite(outputPin, Output); // Write out the output from the PID loop to our LED pin
  now = millis(); // Keep track of time
  
  //send-receive with processing if it's time
  if(millis()>serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime+=500;
  }  
}




/********************************************
 * Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array



// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1: 0=Direct, 1=Reverse, else = ? error ?
//  2-5: float setpoint
//  6-9: float input
//  10-13: float output  
//  14-17: float P_Param
//  18-21: float I_Param
//  22-245: float D_Param
void SerialReceive()
{

  // read the bytes sent from Processing
  int index=0;
  byte Auto_Man = -1;
  byte Direct_Reverse = -1;
  while(Serial.available()&&index<26)
  {
    if(index==0) Auto_Man = Serial.read();
    else if(index==1) Direct_Reverse = Serial.read();
    else foo.asBytes[index-2] = Serial.read();
    index++;
  } 
  
  // if the information we got was in the correct format, 
  // read it into the system
  if(index==26  && (Auto_Man==0 || Auto_Man==1)&& (Direct_Reverse==0 || Direct_Reverse==1))
  {
    Setpoint=double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the 
                                          //   value of "Input"  in most cases (as 
                                          //   in this one) this is not needed.
    if(Auto_Man==0)                       // * only change the output if we are in 
    {                                     //   manual mode.  otherwise we'll get an
      Output=double(foo.asFloat[2]);      //   output blip, then the controller will 
    }                                     //   overwrite.
    
    double p, i, d;                       // * read in and set the controller tunings
    p = double(foo.asFloat[3]);           //
    i = double(foo.asFloat[4]);           //
    d = double(foo.asFloat[5]);           //
    myPID.SetTunings(p, i, d);            //
    
    if(Auto_Man==0) myPID.SetMode(MANUAL);// * set the controller mode
    else myPID.SetMode(AUTOMATIC);             //
    
    if(Direct_Reverse==0) myPID.SetControllerDirection(DIRECT);// * set the controller Direction
    else myPID.SetControllerDirection(REVERSE);          //
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}

// unlike our tiny microprocessor, the processing ap
// has no problem converting strings into floats, so
// we can just send strings.  much easier than getting
// floats from processing to here no?
void SerialSend()
{
  Serial.print("PID ");
  Serial.print(Setpoint);   
  Serial.print(" ");
  Serial.print(Input);   
  Serial.print(" ");
  Serial.print(Output);   
  Serial.print(" ");
  Serial.print(myPID.GetKp());   
  Serial.print(" ");
  Serial.print(myPID.GetKi());   
  Serial.print(" ");
  Serial.print(myPID.GetKd());   
  Serial.print(" ");
  if(myPID.GetMode()==AUTOMATIC) Serial.print("Automatic");
  else Serial.print("Manual");  
  Serial.print(" ");
  if(myPID.GetDirection()==DIRECT) Serial.println("Direct");
  else Serial.println("Reverse");
}

