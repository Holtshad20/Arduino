import processing.core.*; 
import processing.xml.*; 

import processing.serial.*; 
import controlP5.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class Hydra_GUI_v1_4 extends PApplet {


//
// Hydra Software for controlling cartesian positioing robot
// comunicates via Gcodes and Mcodes to Arduino microcontroller via serial interface
//
// Author: Clayton Webster
// Date: May 2010
//

//// state definitions, cant use #defines because java has no preprocessor
//#define STARTUP 1
//#define SELECTCOM 2
//#define WAITFORHANDSHAKE 3
//#define ERRORCONNECTING 4
//#define SELECTFILE 5
//#define SENDFILE 6
//#define SENDCMD 7
//#define CONFIRMATION 8
//#define ENDOFFILE 9
//#define TERMINATE 10
//#define STOP 11
//#define FILEPREVIEW 12
//#define PLAYBACK 13
//#define RXCMD 14





// environment variables
ControlP5 controlP5;
ControlWindow controlWindow;
ControlWindowCanvas cc;
controlP5.Textlabel x_label, y_label, mouse_label, progress_label;
int pw_width = 600; // width of file preview window
int pw_height = 600; // height of file preview window
int pw_lower_threshold = 135;
int x_mouse_pos, y_mouse_pos;

// FSM variables
int state = 0;
int previous_state, next_state, next_state_storage;

// Listbox variables
ListBox consolebox;
int counter = 0;
float distance = 1.0f;

// Serial and communication variables
Serial myPort;  // Create object from Serial class
String portName;
int portnum = -1;
int wait_time = 0;
int inByte;
int conf = 1;

// File and command variables
String[] lines; // file array
String filename = null;
int index;
String cmdstring;
float[][] coordinates;
int preview_index;
int preview_index_drawn;
int preview_index_total;
float build_scaler = 12.0f; // inches to pixels
float current_x, current_y, current_z;
float x_preview_offset = 15.0f;
float y_preview_offset = 15.0f;
float x_preview_offset_original, y_preview_offset_original, build_scaler_original;
float build_width = 20.0f; // inches
float build_depth = 22.0f; // inches
float x_max_ex = 0.0f;
float x_max = 0.0f;
float y_max_ex = 0.0f;
float y_max = 0.0f;
float z_max_ex = 0.0f;
float z_max = 0.0f;
float x_min_ex = 0.0f;
float x_min = 0.0f;
float y_min_ex = 0.0f;
float y_min = 0.0f;
float z_min_ex = 0.0f;
float z_min = 0.0f;
float hue_calc;
float previous_millis = 0.0f;
int speed_delay = 500;

// command receiving variables
int max_cmd_size = 100;
char [] buffer = new char[max_cmd_size]; // buffer for serial commands
char serial_char; // value for each byte read in from serial comms
int serial_count = 0; // current length of command
float current_temp;
int current_temp_int;
String current_temp_buff = "";

// boolean variables
boolean enable_preview = true; // decides if realtime preview window will open when sending a file, leave this out for large files
boolean software_endstops = false;
boolean handshake = true; // allow it to send 1st command
boolean firstcontact = false;
boolean programstop = false;
boolean sendingfile = false;
boolean english_units = true; // default to inches, other option is mm
boolean extruding = false;
boolean preview = false;
boolean Lmouse_pressed = false;
boolean Rmouse_pressed = false;
boolean need_to_update = false;


public void setup() 
{
  size(600,335);
  frameRate(30);
  frame.setTitle("Hydra GUI");
  controlP5 = new ControlP5(this);
  
  // Add listbox
  consolebox = controlP5.addListBox("myList",25,80,550,210);
  consolebox.setBarHeight(20);
  consolebox.setItemHeight(15);
  
  consolebox.captionLabel().set("                                         HYDRA Transmission Console");
  consolebox.captionLabel().style().marginTop = 5;
  consolebox.valueLabel().style().marginTop = 5; // the +/- sign
  
  consolebox.setColorBackground(color(255,30));
  consolebox.setColorActive(color(0,0,255,128));
  
  
  // Buttons and text boxes
  controlP5.addButton("button0",0,25,10,51,25);
  controlP5.controller("button0").setCaptionLabel(" Connect");
  
  controlP5.addButton("button1",0,85,10,40,25);
  controlP5.controller("button1").setCaptionLabel(" Pause");
  controlP5.controller("button1").hide();
  
  controlP5.addButton("button2",0,134,10,58,25);
  controlP5.controller("button2").setCaptionLabel(" Send File");
  controlP5.controller("button2").hide();
  
  controlP5.addButton("filepreview",0,201,10,72,25);
  controlP5.controller("filepreview").setCaptionLabel(" File Preview");
  
  controlP5.addButton("openpreview",0,25,302,31,17);
  controlP5.controller("openpreview").setCaptionLabel("Show");
  controlP5.controller("openpreview").hide();
  
  controlP5.addTextfield("temperature",292,10,60,20);
  controlP5.controller("temperature").setCaptionLabel("Set Temp (C)");
  controlP5.controller("temperature").hide();
  
  controlP5.addTextfield("command",370,10,205,20);
  controlP5.controller("command").setCaptionLabel("Enter Individual Command");
  controlP5.controller("command").hide();
  
  progress_label = controlP5.addTextlabel("progress_label","Progress",72,306);
  controlP5.controller("progress_label").hide();
  
}

public void keyPressed() { // anytime a key is pressed
  if( key >= '0' && key <= '9') { // if it is a number
    if (portnum < 0) { // we havent entered a port yet
      portnum = PApplet.parseInt(key)-48;
    }
  }
}


public void command(String theText) { // Send individual command
  if (firstcontact == true) { // we have authenticated machine
    if (sendingfile == false || programstop == true) {
      cmdstring = theText; // receiving text from controller texting
      state = 7; // go to send command state (7)
    }
    else // file is being sent and program is not stopped
    {
      consolebox.addItem("You cannot send commands while a file is being sent.  Please pause program first",counter++);
      consolebox.scroll(distance);
    }
  }
  else // we have not authenticated machine
  {
    consolebox.addItem("Must connect to HYDRA before using these buttons",counter++);
    consolebox.scroll(distance);
    state = 0; // null state
  }
}


public void temperature(String theText) { // Send individual command
  if (firstcontact == true) { // we have authenticated machine
    if (sendingfile == false || programstop == true) {
      consolebox.addItem("Setting temperature to: "+theText+" degrees C", counter++);
      consolebox.scroll(distance);
      myPort.write("M104 S"+theText+" :");
      next_state = 0; // go back to screen after getting confirmation
      state = 8; // go get confirmation
    }
    else // file is being sent and program is not stopped
    {
      consolebox.addItem("You cannot send commands while a file is being sent.  Please pause program first",counter++);
      consolebox.scroll(distance);
    }
  }
  else // we have not authenticated machine
  {
    consolebox.addItem("Must connect to HYDRA before using these buttons",counter++);
    consolebox.scroll(distance);
    state = 0; // null state
  }
}

public void button0(int theValue) { // Connect
  if (firstcontact == false) { // we have not authenticated a machine
    state = 1; // go to startup state (1)
  }
  else // already connected
  {
    consolebox.addItem("Already connected!",counter++);
    consolebox.scroll(distance);
  }
}

public void button1(int theValue) { // Stop
  if (firstcontact == true) { // machine is authenticated
    if (sendingfile == true) {
      if (programstop == false) { // program is running
        previous_state = state; // know where we want to go back to
        next_state_storage = next_state; // know where we were planning to go
        state = 11; // go to stopped state (11)
      }
      else
      {
        controlP5.controller("button1").setCaptionLabel(" Pause");
        programstop = false;
        if (previous_state == 8) { // if we were waiting for confirmation
          handshake = true; // we most likely stole the handshake
          myPort.clear();
          next_state = next_state_storage;
        }
        state = previous_state;
      }
    }
    else
    {
      consolebox.addItem("Nothing to pause!  Use the send file button to start sending a file",counter++);
      consolebox.scroll(distance);
    }
  }
  else
  {
    consolebox.addItem("Must connect to HYDRA before using these buttons",counter++);
    consolebox.scroll(distance);
    state = 0;
  }
  
}

public void button2(int theValue) { // Send file
  if (firstcontact == true) {
    if (sendingfile == false) {
      state = 5; // go to select file case (5)
    }
    else {
      consolebox.addItem("Please wait until the current file is done sending before sending another",counter++);
      consolebox.scroll(distance);
    }
  }
  else
  {
    consolebox.addItem("Must connect to HYDRA before using these buttons",counter++);
    consolebox.scroll(distance);
    state = 0;
  }
}


public void openpreview(int theValue) { // Open gcode preview window in case it was closed
  //controlP5.window("Gcode Viewer").show();
  open_gcode_preview_window();
}


public void filepreview(int theValue) { // Open gcode preview window in case it was closed
  if (sendingfile == true) {
    consolebox.addItem("Please wait until the current file is done sending before previewing another file",counter++);
    consolebox.scroll(distance);
  }
  else {
    state = 12; // go to preview case
  }
}


public void preview_playback(int theLine) {
  if (state != 13) { // we aren't playing
    controlP5.controller("preview_playback").setCaptionLabel(" Pause");
    state = 13;
  }
  else {
    controlP5.controller("preview_playback").setCaptionLabel(" Play");
    state = 0;
  }
}


public void speed_slider(int theSpeed) {
  speed_delay = (100-theSpeed) * 5;
}


public void preview_slider(int theLine) {
  preview_index = theLine;
  if (preview_index < preview_index_drawn) { // if we have to go backwards, clear screen
    println("preview backwards, clear");
    preview_index_drawn = 0;
  }
}


public void scale_slider(float theScaler) {
  build_scaler = theScaler;
  preview_index_drawn = 0;
}


public void x_offset(float theScaler) {
  x_preview_offset = theScaler;
  preview_index_drawn = 0;
}


public void y_offset(float theScaler) {
  y_preview_offset = theScaler;
  preview_index_drawn = 0;
}


public void update_progress_bar(int current, int total) // progress bar for how much of file is sent
{
  int pb_height = 10;
  int pb_x = 125;
  int pb_y = 305;
  int pb_total_width = 450;
  
  float progress_bar_width = (index*1.0f/lines.length)*pb_total_width;
  fill(255);
  rect(pb_x, pb_y, progress_bar_width, pb_height);
  
  int padding = 2;
  stroke(150);
  line(pb_x-padding, pb_y-padding, pb_x-padding, pb_y+pb_height+padding); // down left
  stroke(150);
  line(pb_x-padding, pb_y-padding, pb_x+pb_total_width+padding, pb_y-padding); // across top
  stroke(150);
  line(pb_x-padding, pb_y+pb_height+padding, pb_x+pb_total_width+padding, pb_y+pb_height+padding); // across bottom
  stroke(150);
  line(pb_x+pb_total_width+padding, pb_y-padding, pb_x+pb_total_width+padding, pb_y+pb_height+padding); // down right
}


public int find_char(char[] command, char search_char, int j_start)
{
  int j = j_start;
  while (j < command.length) {
    if (command[j] == search_char) {
      return j;
    }
    j++;
  }
  return -1;
}



public float get_coordinate(String cmdstring, char search_char)
{
  char [] command = cmdstring.toCharArray(); // convert to char array so we can search through it
  if (find_char(command, search_char, 0) > 0) { // found the search character
    if (find_char(command, ' ', find_char(command, search_char, 0)) > 0) { // found where the space is (end of number)
      String number_string = cmdstring.substring(find_char(command, search_char, 0)+1,find_char(command, ' ', find_char(command, search_char, 0))); // make substring of just number
      try {
        float number_float = Float.valueOf(number_string.trim()).floatValue(); // convert substring to float
        if (software_endstops == true) {
          if (number_float < 0) number_float = 0; // software endstops
        }
        return number_float;
      }
      catch (Exception e) {
        consolebox.addItem("A syntax error was detected at line "+(index+1), counter++);
        consolebox.scroll(distance);
        return -99999.0f; // same as if we didn't find a coordinate
      }
    }
  }
  return -99997.0f; // just use some random value to the program will know we didn't find a value
}



public void gcode_interpreter(String cmdstring)
{
  // check for specific G and M codes that actually mean something to the GUI
  if (cmdstring.indexOf("G20") != -1) { // found G20 for inch units
    english_units = true;
  }
  else if (cmdstring.indexOf("G21") != -1) { // found G21 for mm units
    english_units = false;
  }
  else if (cmdstring.indexOf("M101") != -1) { // found M101
    extruding = true;
  }
  else if (cmdstring.indexOf("M102") != -1 || cmdstring.indexOf("M103") != -1) { // found M102 or M103
    extruding = false;
  }
  else if (cmdstring.indexOf("M105") != -1) { // found M105
    if (preview == false) {
      println("M105 found");
      
      current_temp_int = -1;
      while(current_temp_int < 0) {
        if (myPort.available() > 0) // each time we see something
        {
          serial_char = PApplet.parseChar(myPort.read());
          
          if (serial_char == ':') { // end of line
            current_temp_int = Integer.parseInt(current_temp_buff);
            println(current_temp_int);
            current_temp_buff = "";
          }
          else {
            current_temp_buff = current_temp_buff + serial_char;
          }
          
        }
      }
      current_temp = current_temp_int/1023.0f*5.0f/0.01f;
      consolebox.addItem("  Current Temperature (C): "+current_temp, counter++);
      consolebox.scroll(distance);
    }
  }
  
  if (state != 7) { // not just an individual command
    // If we get a movement command, parse the coordinates and pass to the gcode preview window
    if (cmdstring.indexOf("G0") != -1 || cmdstring.indexOf("G1") != -1){
      if (get_coordinate(cmdstring, 'X') != -99997.0f) { // found an X value
        coordinates[1][preview_index] = get_coordinate(cmdstring, 'X');
        if (english_units == false) coordinates[1][preview_index] = coordinates[1][preview_index] / 25.4f;
        if (extruding == true && coordinates[1][preview_index] > x_max_ex) x_max_ex = coordinates[1][preview_index];
        if (coordinates[1][preview_index] > x_max) x_max = coordinates[1][preview_index];
        if (extruding == true && coordinates[1][preview_index] < x_min_ex) x_min_ex = coordinates[1][preview_index];
        if (coordinates[1][preview_index] < x_min) x_min = coordinates[1][preview_index];
      }
      else if (preview_index > 1) {
        coordinates[1][preview_index] = coordinates[1][preview_index - 1]; // if we didn't get a coordinate, keep previous
      }
      if (get_coordinate(cmdstring, 'Y') != -99997.0f) { // found a Y value
        coordinates[2][preview_index] = get_coordinate(cmdstring, 'Y');
        if (english_units == false) coordinates[2][preview_index] = coordinates[2][preview_index] / 25.4f;
        if (extruding == true && coordinates[2][preview_index] > y_max_ex) y_max_ex = coordinates[2][preview_index];
        if (coordinates[2][preview_index] > y_max) y_max = coordinates[2][preview_index];
        if (extruding == true && coordinates[2][preview_index] < y_min_ex) y_min_ex = coordinates[2][preview_index];
        if (coordinates[2][preview_index] < y_min) y_min = coordinates[2][preview_index];
      }
      else if (preview_index > 1) {
        coordinates[2][preview_index] = coordinates[2][preview_index - 1]; // if we didn't get a coordinate, keep previous
      }
      if (get_coordinate(cmdstring, 'Z') != -99997.0f) { // found a Z value
        coordinates[3][preview_index] = get_coordinate(cmdstring, 'Z');
        if (english_units == false) coordinates[3][preview_index] = coordinates[3][preview_index] / 25.4f;
        if (extruding == true && coordinates[3][preview_index] > z_max_ex) z_max_ex = coordinates[3][preview_index];
        if (coordinates[3][preview_index] > z_max) z_max = coordinates[3][preview_index];
        if (extruding == true && coordinates[3][preview_index] < z_min_ex) z_min_ex = coordinates[3][preview_index];
        if (coordinates[3][preview_index] < z_min) z_min = coordinates[3][preview_index];
      }
      else if (preview_index > 1) {
        coordinates[3][preview_index] = coordinates[3][preview_index - 1]; // if we didn't get a coordinate, keep previous
      }
      
      if (extruding == true) { // are we extruding while making this move?
        coordinates[4][preview_index] = 1.0f;
      }
      else {
        coordinates[4][preview_index] = 0.0f;
      }
      
      preview_index++; // only update this if we found a movement command with new coordinates (G0 and G1)
    }
  }
}



public void open_gcode_preview_window()
{
  controlWindow = controlP5.addControlWindow("Gcode Viewer",0,0,pw_width,pw_height,10);
  controlWindow.setUpdateMode(ControlWindow.NORMAL);
  controlWindow.setDrawBackground(false);
  controlWindow.hideCoordinates();
  cc = new MyCanvas();
  cc.pre();
  controlWindow.addCanvas(cc);
  
  x_label = controlP5.addTextlabel("x_label","X",47,2);
  x_label.setWindow(controlWindow);
  y_label = controlP5.addTextlabel("y_label","Y",2,48);
  y_label.setWindow(controlWindow);
  mouse_label = controlP5.addTextlabel("mouse_label","Rmouse up/down - Zoom in/out,  Lmouse - change table position",pw_width/2-156,2);
  mouse_label.setWindow(controlWindow);
  
  Controller scaleSlider = controlP5.addSlider("scale_slider",1,1000,build_scaler,25,pw_height-70,pw_width-180,10);
  controlP5.controller("scale_slider").setCaptionLabel("Preview Scale (in to pixels)");
  scaleSlider.setWindow(controlWindow);
  
  Controller xSlider = controlP5.addSlider("x_offset",-pw_width*3,pw_width*3,x_preview_offset,25,pw_height-50,pw_width-128,10);
  controlP5.controller("x_offset").setCaptionLabel("X Offset (pixels)");
  xSlider.setWindow(controlWindow);
  
  Controller ySlider = controlP5.addSlider("y_offset",-pw_height*3,pw_height*3,y_preview_offset,25,pw_height-30,pw_width-128,10);
  controlP5.controller("y_offset").setCaptionLabel("Y Offset (pixels)");
  ySlider.setWindow(controlWindow);
  
  if (preview == true) {
    
    Controller playbackButton = controlP5.addButton("preview_playback",0,25,pw_height-115,38,15);
    controlP5.controller("preview_playback").setCaptionLabel(" Play");
    playbackButton.setWindow(controlWindow);
    
    Controller speedSlider = controlP5.addSlider("speed_slider",0,100,30,87,pw_height-113,pw_width-140,10);
    controlP5.controller("speed_slider").setCaptionLabel("Speed");
    speedSlider.setWindow(controlWindow);
    
    Controller previewSlider = controlP5.addSlider("preview_slider",0,preview_index_total,preview_index,25,pw_height-90,pw_width-110,10);
    controlP5.controller("preview_slider").setCaptionLabel("Program Line");
    previewSlider.setWindow(controlWindow);
  }
  
  //preview = false;
}


// controlWindowCanvas class for gcode preview window
class MyCanvas extends ControlWindowCanvas {
  public void draw(PApplet theApplet) {
    
    if (state == 13) { // in playback
      controlP5.controller("preview_slider").setValue(preview_index); // update speed controller
    }
    
    
    // need to check that the moues is above pw threshold too to make sure you aren't just changing the sliders
    if ((theApplet.mousePressed == true) && (theApplet.mouseButton == LEFT) && (Lmouse_pressed == false) && ((pw_height-theApplet.mouseY) > pw_lower_threshold)) { // first press
      Lmouse_pressed = true;
      x_mouse_pos = theApplet.mouseX;
      y_mouse_pos = theApplet.mouseY;
      x_preview_offset_original = x_preview_offset;
      y_preview_offset_original = y_preview_offset;
    }
    else if ((theApplet.mousePressed == true) && (theApplet.mouseButton == LEFT) && (Lmouse_pressed == true) && ((pw_height-theApplet.mouseY) > pw_lower_threshold)) { // still holding
      x_preview_offset = x_preview_offset_original + (theApplet.mouseX-x_mouse_pos);
      y_preview_offset = y_preview_offset_original + (theApplet.mouseY-y_mouse_pos);
      controlP5.controller("x_offset").setValue(x_preview_offset);
      controlP5.controller("y_offset").setValue(y_preview_offset);
      preview_index_drawn = 0;
    }
    else if ((theApplet.mousePressed == false) && (Lmouse_pressed == true) && ((pw_height-theApplet.mouseY) > pw_lower_threshold)) { // just let go
      x_preview_offset = x_preview_offset_original + (theApplet.mouseX-x_mouse_pos);
      y_preview_offset = y_preview_offset_original + (theApplet.mouseY-y_mouse_pos);
      controlP5.controller("x_offset").setValue(x_preview_offset);
      controlP5.controller("y_offset").setValue(y_preview_offset);
      preview_index_drawn = 0;
      Lmouse_pressed = false;
    }
    
    if ((theApplet.mousePressed == true) && (theApplet.mouseButton == RIGHT) && (Rmouse_pressed == false) && ((pw_height-theApplet.mouseY) > pw_lower_threshold)) { // first press
      Rmouse_pressed = true;
      y_mouse_pos = theApplet.mouseY;
      build_scaler_original = build_scaler;
    }
    else if ((theApplet.mousePressed == true) && (theApplet.mouseButton == RIGHT) && (Rmouse_pressed == true) && ((pw_height-theApplet.mouseY) > pw_lower_threshold)) { // still holding
      build_scaler = build_scaler_original + (y_mouse_pos-theApplet.mouseY)/3.0f;
      controlP5.controller("scale_slider").setValue(build_scaler);
      preview_index_drawn = 0;
    }
    else if ((theApplet.mousePressed == false) && (Rmouse_pressed == true) && ((pw_height-theApplet.mouseY) > pw_lower_threshold)) { // just let go
      build_scaler = build_scaler_original + (y_mouse_pos-theApplet.mouseY)/3.0f;
      controlP5.controller("scale_slider").setValue(build_scaler);
      Rmouse_pressed = false;
      preview_index_drawn = 0;
    }
    
    
    // draw background if we need a complete redraw
    if (preview_index_drawn == 0) {
      theApplet.background(20);
      current_x = 0.0f;
      current_y = 0.0f;
      current_z = 0.0f;
    }
    
    
    // update controller values since we changed them
    //controlP5.controller("x_offset").setValue(x_preview_offset);
    //controlP5.controller("y_offset").setValue(y_preview_offset);
    //controlP5.controller("scale_slider").setValue(build_scaler);
    
    // draw the build platform
    theApplet.strokeWeight(2);
    theApplet.stroke(100);
    theApplet.line(0*build_scaler+x_preview_offset, 0*build_scaler+y_preview_offset, build_width*build_scaler+x_preview_offset, 0*build_scaler+y_preview_offset);
    theApplet.strokeWeight(2);
    theApplet.stroke(100);
    theApplet.line(0*build_scaler+x_preview_offset, 0*build_scaler+y_preview_offset, 0*build_scaler+x_preview_offset, build_depth*build_scaler+y_preview_offset);
    theApplet.strokeWeight(2);
    theApplet.stroke(100);
    theApplet.line(build_width*build_scaler+x_preview_offset, 0*build_scaler+y_preview_offset, build_width*build_scaler+x_preview_offset, build_depth*build_scaler+y_preview_offset);
    theApplet.strokeWeight(2);
    theApplet.stroke(100);
    theApplet.line(0*build_scaler+x_preview_offset, build_depth*build_scaler+y_preview_offset, build_width*build_scaler+x_preview_offset, build_depth*build_scaler+y_preview_offset);
    
    theApplet.strokeWeight(1);
    
    
    int i = preview_index_drawn;
    //println(preview_index_drawn);
    while (i < preview_index) {
      theApplet.colorMode(HSB);
      if (coordinates[4][i] == 1.0f) {
        theApplet.strokeWeight(4);
      }
      else {
        theApplet.strokeWeight(1);
      }
      
      hue_calc = current_z/z_max_ex*240+15;
      if (hue_calc > 255) hue_calc = 255;
      else if (hue_calc < 0) hue_calc = 0;
      
      theApplet.stroke(hue_calc, 255, 100);
      theApplet.line(current_x*build_scaler+x_preview_offset, current_y*build_scaler+y_preview_offset, coordinates[1][i]*build_scaler+x_preview_offset, coordinates[2][i]*build_scaler+y_preview_offset);
      
      theApplet.strokeWeight(1); // reset stroke weight for controllers
      theApplet.colorMode(RGB); // reset color mode
      
//      // draw circle for where tool head currently is
//      if ((preview_index - i == 1) && (state != 13)) { // preview_index is always one ahead, so this is the last line drawn.  Also make sure we aren't in playback (too slow to redraw everything each frame)
//        theApplet.ellipseMode(CENTER);
//        theApplet.noStroke();
//        theApplet.fill(220);
//        theApplet.ellipse(coordinates[1][i]*build_scaler+x_preview_offset, coordinates[2][i]*build_scaler+y_preview_offset, 6, 6);
//      }
      
      current_x = coordinates[1][i];
      current_y = coordinates[2][i];
      current_z = coordinates[3][i];
      
      i++;
    }
    preview_index_drawn = preview_index;
    
    theApplet.strokeWeight(2);
    theApplet.stroke(200);
    theApplet.line(5, 5, 40, 5);
    theApplet.triangle(42, 5, 40, 3, 40, 7);
    theApplet.line(5, 5, 5, 40);
    theApplet.triangle(5, 42, 3, 40, 7, 40);
    
    theApplet.strokeWeight(1);
    
  }
}


public void draw() {
  background(20);
  
  switch(state) {
    case 1: // STARTUP
      //println("case 1");
      if (Serial.list().length < 1) { // no ports available
        consolebox.addItem("There does not appear to be any serial ports associated with the computer.", counter++);
        consolebox.addItem("Please ensure that Hydra is connected via a USB serial cable and then try connection again",counter++);
        consolebox.scroll(distance);
        state = 0;
        break;
      }
      
      consolebox.addItem("Available serial ports are listed below",counter++);
      
      int i = 1;
      while (i <= Serial.list().length) {
        try
        {
          consolebox.addItem("  " + Serial.list()[i-1], counter++); // array is zero indexed so we must subtract 1
          i++;
        }
        catch(ArrayIndexOutOfBoundsException e)
        {
          break;
        }
      }
      consolebox.scroll(distance);
      portnum = 0; // start with first available
      state = 2; // go to serial port selection case (2)
      break;
    
    case 2: // SELECTCOM
      //println("case 2");
      if (portnum < Serial.list().length) { // port number is below available ports, should be < not <=
        try
        {
          portName = Serial.list()[portnum]; // change this to correct com port ([0] = COM1, etc)
        }
        catch(ArrayIndexOutOfBoundsException e)
        {
          consolebox.addItem(" ", counter++);
          consolebox.addItem("Invalid port number tested!  Please try to connect again",counter++);
          consolebox.scroll(distance);
          state = 0;
          break;
        }
        catch(Exception e) {
          consolebox.addItem(" ", counter++);
          consolebox.addItem("Something went wrong with selected port!  Please try to connect again",counter++);
          consolebox.scroll(distance);
          state = 0;
          break;
        }
        consolebox.addItem(" ", counter++);
        consolebox.addItem("Attempting connection on " + Serial.list()[portnum] + ".  Waiting for handshake...",counter++);
        consolebox.scroll(distance);
        firstcontact = false;
        try {
          myPort = new Serial(this, portName, 19200);
        }
        catch(Exception e) {
          consolebox.addItem(" ", counter++);
          consolebox.addItem("Port creation failed!  Port may be in use.  Close any other programs that may be using it and try again",counter++);
          consolebox.scroll(distance);
          state = 0;
          break;
        }
        state = 3;
        break;
      }
      else { // port num is too large and we still don't have connection
        consolebox.addItem(" ", counter++);
        consolebox.addItem("All available serial ports were tested and none responded with the appropriate authentication sequence", counter++);
        consolebox.addItem("Please ensure that Hydra is connected via a USB serial cable and try again",counter++);
        consolebox.scroll(distance);
        state = 0;
        break;
      }
    
    case 3: // WAITFORHANDSHAKE
      //println("case 3");
      wait_time = 0;
      while (firstcontact == false){
        if ( myPort.available() > 0) { // if data is available
          inByte = myPort.read();
          if (inByte == 'A') { 
            firstcontact = true; // you've had first contact from the microcontroller
            myPort.write("G999A :"); // send Arduino bogus command so that it stops broadcasting authentication code
            consolebox.addItem("  Hydra Authenticated!", counter++);
            consolebox.scroll(distance);
            
            // show all the buttons we can now use once we are connected
            controlP5.controller("button1").show();
            controlP5.controller("button2").show();
            controlP5.controller("temperature").show();
            controlP5.controller("command").show();
            
            state = 0;
            break;
          }
        }
        delay(5);
        wait_time++;
        
        if (wait_time > 600) { // if we have waited 3 seconds
          state = 4;
          break;
        }
      }
      break;
    
    case 4: // ERRORCONNECTING
      //println("case 4");
      consolebox.addItem("  It appears there is nothing present on this COM port", counter++);
      consolebox.scroll(distance);
      portnum++;
      state = 2;
      break;
    
    case 5: // SELECTFILE
      //println("case 5");
      consolebox.addItem(" ", counter++);
      consolebox.addItem("Please select gcode program file...", counter++);
      
      if (filename == null) { // we have not selected a file yet
        try {
          filename = selectInput();
          if (filename == null){
            consolebox.addItem("Invalid file selected!  Use the send file button to try again", counter++);
            consolebox.scroll(distance);
            state = 0;
            break;
          }
          consolebox.addItem(filename, counter++);
          lines = loadStrings(filename);
        }
        catch (Exception e)
        {
          consolebox.addItem(" ", counter++);
          consolebox.addItem("Something went wrong with file selection.  Please try again",counter++);
          consolebox.scroll(distance);
          state = 0;
          break;
        }
        consolebox.addItem(" ", counter++);
        consolebox.addItem("program is " + lines.length + " lines long", counter++);
        consolebox.scroll(distance);
        
        index = 0; // ensure that we start at the beginning of the file
        sendingfile = true;
        
        if (enable_preview == true) {
          controlP5.controller("openpreview").show();
          
          // get the preview window open and started
          open_gcode_preview_window();
          coordinates = new float[5][lines.length];
          preview_index = 0;
        }
        
        
        myPort.clear(); // clear serial port buffer right before we start sending commands
        state = 6; // now that we have the file selected, go send it
      }
      break;
    
    case 6: // SENDFILE
      //println("case 6");
      
      // send file commands
      while (index < lines.length-1) {
        myPort.write(lines[index] + " :"); // add colon because Arduino uses that as end of line character
        consolebox.addItem("Line " + (index+1) + ": " + lines[index], counter++);
        consolebox.scroll(distance);
        index = index + 1;
        
        
        if (enable_preview == true) {
          // addition for gcode viewer
          preview_index_drawn = 0; // redraw preview window for each new command
          cmdstring = lines[index];
          gcode_interpreter(cmdstring); // interpret what was just sent to the Arduino
        }
        
        
        controlP5.controller("progress_label").show();
        update_progress_bar(index, lines.length);
        
        handshake = false;
        
        next_state = 6; // come back after getting confirmation
        state = 8; // wait for confirmation
        break; // this will only break out of while loop, need if statement below
      }
      
      if (index >= lines.length-1) { // if we are at end
        state = 9; // file is done
      }
      break;
    
    case 7: // SENDCMD
      println("case 7");
      
      myPort.clear();
      myPort.write(cmdstring + " :");
      consolebox.addItem("Individual Command: " + cmdstring, counter++);
      consolebox.scroll(distance);
      handshake = false;
      gcode_interpreter(cmdstring);
      
      next_state = 0; // go back to screen after getting confirmation
      state = 8; // go get confirmation
      
      break;
    
    case 8: // CONFIRMATION
      //println("case 8");
      if (handshake == false){ // wait for confirmation
        if ( myPort.available() > 0) {  // if data is available,
          conf = myPort.read();         // read it and store it
          if (conf == 'A'){
            handshake = true;
            state = next_state; // go to desired state
          }
        }
      }
      else // if we already got confirmation, why are we still here?
      {
        state = next_state; // go to desired state
        break;
      }
      
      if (next_state == 6) { // we are sending a long file
        update_progress_bar(index, lines.length); // update progress bar
      }
      
      break;
    
    case 9: // ENDOFFILE
      println("case 9");
      consolebox.addItem("-- end of program --", counter++);
      consolebox.scroll(distance);
      
      sendingfile = false;
      filename = null;
      controlP5.controller("openpreview").hide();
      controlP5.controller("progress_label").hide();
      state = 0;
      break;
    
    case 10: // TERMINATE
      println("case 10");
      delay(3000);
      exit();
      break;
    
    case 11: // STOP
      println("case 11");
      if (programstop == false) { // program has not been stopped yet
        myPort.write("M0 :");
        consolebox.addItem("Program flow paused. Click resume to continue sending commands", counter++);
        consolebox.scroll(distance);
        programstop = true;
        
        handshake = false;
        
        while (handshake == false){ // wait for confirmation
          if ( myPort.available() > 0) {  // if data is available,
            conf = myPort.read();         // read it and store it
            if (conf == 'A'){
              handshake = true;
              controlP5.controller("button1").setCaptionLabel("Resume");
              state = 0;
              // dont change state because we want to just keep coming back here until they resume
            }
          }
        }
      }
      // jump to here once we have stopped and just keep waiting for resume command
      break;
      
    case 12: // FILEPREVIEW
      consolebox.addItem(" ", counter++);
      consolebox.addItem("Please select gcode program file...", counter++);
      
      if (filename == null) { // we have not selected a file yet
        
        preview = true;
        
        try {
          filename = selectInput();
          if (filename == null){
            consolebox.addItem("Invalid file selected!  Use the file preview button to try again", counter++);
            consolebox.scroll(distance);
            state = 0;
            break;
          }
          consolebox.addItem(filename, counter++);
          lines = loadStrings(filename);
        }
        catch (Exception e)
        {
          consolebox.addItem(" ", counter++);
          consolebox.addItem("Something went wrong with file selection.  Please try again",counter++);
          consolebox.scroll(distance);
          state = 0;
          break;
        }
        consolebox.addItem(" ", counter++);
        consolebox.addItem("program is " + lines.length + " lines long", counter++);
        consolebox.scroll(distance);
        
        index = 0; // ensure that we start at the beginning of the file
        coordinates = new float[5][lines.length];
        preview_index = 0;
        
        // store file in coordinates array
        while (index < lines.length) {
          cmdstring = lines[index];
          gcode_interpreter(cmdstring); // interpret each line
          index = index + 1;
        }
        
        preview_index_total = preview_index;
        preview_index = 0; // initialize view to first line
        preview_index_drawn = 0;
        open_gcode_preview_window();
        filename = null;
        state = 0;
        
        consolebox.addItem("Movement dimensions: X: "+x_min+" to "+x_max+",  Y: "+y_min+" to "+y_max+",  Z: "+z_min+" to "+z_max, counter++);
        consolebox.addItem("Extrusion dimensions: X: "+x_min_ex+" to "+x_max_ex+",  Y: "+y_min_ex+" to "+y_max_ex+",  Z: "+z_min_ex+" to "+z_max_ex, counter++);
        consolebox.scroll(distance);
        
      }
      break;
      
    case 13: // PLAYBACK
      if (millis()-previous_millis > speed_delay) {
        previous_millis = millis();
        preview_index++;
      }
      break;
    
  }
  
}


public void stop()
{
  println("exiting...");
  if (firstcontact == true) { // machine is authenticated
    // tell firmware we are about to close
    myPort.write("M900 :");
    delay(3000); // give the firmware time to store the positioning values before we close the serial connection
    // wrap up serial connections
    myPort.clear();
    myPort.stop();
  }
  
  super.stop();
}


  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#F0F0F0", "Hydra_GUI_v1_4" });
  }
}
