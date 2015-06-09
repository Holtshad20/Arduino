// Arduino G-code Interpreter
// v1.0 by Mike Ellery - initial software (mellery@gmail.com)
// v1.1 by Zach Hoeken - cleaned up and did lots of tweaks (hoeken@gmail.com)
// v1.2 by Chris Meighan - cleanup / G2&G3 support (cmeighan@gmail.com)
// v1.3 by Zach Hoeken - added thermocouple support and multi-sample temp readings. (hoeken@gmail.com)

// v1.3a mmcp - removed extruder functionality
//            - changed variable array called word (!) to array called aWord
//            - added checksum to serial comms

// v1.4  mmcp - extended M114 to report limit switches as well as co-ordinates
//============================================================================

#define FIRMWARE_VERSION "v1.4"

#include <HardwareSerial.h> 

// our point structure to make things nice
//========================================
struct LongPoint {
  long x;
  long y;
  long z;
};

struct FloatPoint {
  float x;
  float y;
  float z;
};

// our command string
//===================
#define COMMAND_SIZE 128
char aWord[COMMAND_SIZE];

long serial_count;
long no_data = 0;

// used to decode commands
//========================
char c;
long code;
long line;
long LastLine = 0;

// coordinate mode
//================
boolean abs_mode = true;

// measurement mode
//=================
boolean inches = true;

// steppers state
//===============
boolean steppers = false;

// debug level
// 0 = no debug
// 1 = short
// 2 = verbose
//=============
int DebugLevel = 0;

void setup()
{
  // open comms port
  //================
  Serial.begin(19200);
  Serial.println("start");

  // initialise subsystems
  //======================
  init_process_string();
  init_steppers();
}

void loop()
{
  // read in characters if we got them
  //==================================
  if (Serial.available() > 0) {
    // read next character
    //====================
    c = Serial.read();
    no_data = 0;

    // newline is end of command
    //==========================
    if (c != '\n') {
      aWord[serial_count] = c;
      serial_count++;
    }
  }
  else {
    // start counting no-data clicks to timeout steppers
    //==================================================
    no_data++;
    delayMicroseconds(25);
  }

  // if we got a command, do it
  //===========================
  if (serial_count && (c == '\n')) {
    //Serial.println("command");
  
    // extract line number if present
    //===============================
    if (has_command('N', aWord, serial_count)) {
      line = (long)search_string('N', aWord, serial_count);
    }
    else {
      line = -1;
    }
    
    // check the checksum
    //===================
    if (Checksum(aWord, serial_count) == true) {
      if (line != LastLine + 1 && line != 0 && line != -1) {
        // line number out of synch
        //=========================
        if (line < LastLine + 1) {
          // repeated line so ignore
          //========================
          Serial.print("rp ");
          Serial.println(line, DEC);
        }
        else {
          // bad line number
          //================
          Serial.print("ex ");        }
          Serial.println(LastLine+1, DEC);
      }
      else {
        // now process our command!
        //=========================
        process_string(aWord, serial_count-1);

        // update line number
        //===================
        LastLine = line;
      }
    }
    else {
      // checksum fail so request resend
      //================================
      Serial.print("rs ");
      Serial.println(LastLine+1, DEC);
    }

    // and clear the command buffer
    //=============================
    init_process_string();
    no_data = 0;
    c = ' ';
  }

  // if no data turn off steppers
  //=============================
  if (no_data > 10000)
    disable_steppers();
}

boolean Checksum(char instruction[], int size) {
  // compute checksum
  // return TRUE if ok
  // return FALSE if not
  //====================
  
  int i;
  int csum;
  int rsum;
  
  // start with 0
  //=============
  csum = 0;
  
  // xor each byte
  //==============
  for (i=0; i<size; i++) {
    if (instruction[i] == '*')
      break;
    csum = csum ^ instruction[i]; 
  }
  
  // now see if digits match checksum
  //=================================
  rsum = search_string('*', instruction, size);
  if (rsum == csum)
    return true;
  else {
    if (DebugLevel > 1) {
      Serial.print("line: >");
      Serial.print(instruction);
      Serial.print("< length: ");
      Serial.print(size, DEC);
      Serial.print(" received: ");
      Serial.print(rsum, DEC);
      Serial.print(" calculated: ");
      Serial.println(csum, DEC);
    }
    return false;
  }
}

