//
// Arduino-based servo-driven sweeping IR radar obstacle avoidance and control system
//
// Remixed by TechBitar (Hazim Bitar) Oct 2011 - Tested successfully Oct 22, 2011
// IR sensor calculation by luckylarry.co.uk
//
// Latest Version: Beta 0.8 Oct 22, 2011
//
// This code was written to be as self-documenting as possible. I used verbose and expanded syntax 
// instead of compact code to make it easier to understand and to port to other programming languages. 
// This code was not copied from any project but I benefited from the collective experience of others.
// Anyone can use and modify this code with attribution.
//
// I used the Arduino UNO R2. 
// Arduino prototyping shield (optional)
// Three TG9e micro servos,  two of them modified for continuous rotation. 
// Sharp IR sensor GP2Y0A2

#include <Servo.h> 

// ============ OPERATIONAL CONSTANTS =================

int safe_distance = 25; // safe distance to obstacle before robot takes action (eg stop, turn) - in inches

// This robot moves by differential steering. To make it turn, one wheel must move
// faster than the other in same or opposite direction for a a number of seconds. 
// Use trial and error to find this time value for your own robot
//  These values depend on many factors such as wheel size, surface qualities, traction, etc. 
// It's best test your robot on surfaces resembling your robot's playground (eg sumo disc, marble)

// Times are expressed in milliseconds. 1000 = 1 sec. 

int rturn_time = 1000;          // time needed for wheels to make a 90 Right turn
int lturn_time = 1000;          // time needed for wheels to make a 90 Left turn
int reverse_time = 2000;     // time needed for wheels while reversing
int backturn_time = 2000;   // time needed for wheels to spin to turn 180

// Create Servo objects
Servo IR_servo;           // Servo to rotate Sharp IR sensor acting as radar 
Servo l_servo;             // Servo to drive Left wheel
Servo r_servo;            // Servo to drive Right wheel

// Assign servos/sensor I/O to Arduino  pins
int IR_read_pin = 5;       // analog pin to read IR sensor
int IR_pin = 9;                // pin to control servo to spin IR sensor
int IR_value_debug =0;
int l_pin = 10;              // pin to direct Left wheel servo
int r_pin = 11;             // pin to direct Right wheel servo

// Trackers to control servo positions. 
int IR_pos = 90;
int l_pos = 0;
int r_pos = 0;

// Values used to stop wheel servos. In this program, I don't use them 
// because I just "detach" servo objects which stops the servos

int r_stop = 78;  // this value is specific to a given servo 
int l_stop = 90;  //   this value is specific to a given servo 

// I use these values to drive servos at maximum speed. Note how one speed is negative 
// while other is positive. 
// That's because when you position servos on both sides of the bot, they will 
// spin opposite each other causing the robot to spin around itself. 
// so to drive a robot with two servos in one direction, one wheel must 
// move clockwise and the other counterclockwise for robot to move ahead or back 

int r_go = +200;  // this value is specific to a given servo 
int l_go = -200;  // this value is specific to a given servo 

// Same as above but reversed. 
int r_reverse = -200;  // this value is specific to a given servo 
int l_reverse = +200;  // this value is specific to a given servo 

char go_direction = 'F' ;  // holds current robot direction
// char last_direction = 'F';
// char dir = 'F';

void setup() 
{   
  Serial.begin(9600);
  pinMode(IR_pin, INPUT);    // set IR Sonar pin for input 
  IR_servo.attach(IR_pin);   // activate IR scanning servo  

  // find which direction to start moving.   
  go_direction = look_around('A');            // scan in (A)ll directions to decide where to go first
  go_direction = move_to(go_direction);  // Go in direction with more space
} 

void loop()
{ 
  // F = Forward  R = Right  L = Left  B = Backturn 180  T = Reverse S = Stop D= Deadend O = Obstacle

  while (go_direction == 'F'){  // keep going forward while no Obstacle is reached and direction returned is 'F'orward
                                              // Serial.print("Loop/Global IR value: ");   Serial.println(IR_value_debug);
    go_direction = look_around(go_direction);
  }
  go_still(); // stop
  
 // Serial.print("NO LONGER MOVING FORWARD BECAUSE OF CONDITION: "); Serial.println(go_direction);

  if (go_direction == 'O'){            // if Forward movement reaches Obstacle
    go_direction = look_around('A');   // look around
    if (go_direction == 'D') {         // if Dead end
      move_to('B');                    // turn 180
      go_direction = look_around('A'); // look around
      if (go_direction == 'D'){        // if another Dead end
        move_to('S');                  // Stop and send help
        // Serial.println("DEAD END! I GIVE UP.");
        while (1==1) {
        }                
      } 
    }
  }
  go_direction = move_to(go_direction);
}

// ====================== RADAR FUNCTION ============================

char look_around(char whereto){
  char dirx = '/'; 
  int last_distance = 0;
  int distance = 0;

  switch (whereto) { // scan where?

  case 'F':  // scan if Forward is clear, return same F if true
    // Serial.println("look_around>> Scanning forward");
    IR_servo.write(90);
    delay(100);
    last_distance = IR_scan();
    delay(10);
    if (last_distance > safe_distance) {
      // Serial.print("look_around>> FORWARD DISTANCE: ");  Serial.println(last_distance);
      dirx = 'F';
      return 'F';    
    } 
    else
    {
      dirx = 'O';   // Obstacle reached
      return 'O';   // Obstacle reached
    }
    break;

  case 'A':  // Scan L,F,R if clear. return direction of direction with more space. 
    dirx = 'D';    // Assume Dead end unless changed in following tests 

    // SCAN LEFT
    IR_servo.write(180); // turn IR servo Left (or Right depending on your setting)
    delay(1000);
    distance = IR_scan(); // get distance to obstacle NOTE: be mindful of out of range values for sensor

    // Serial.print("look_around>> Left scan distance: "); Serial.println(distance);
    if (distance > safe_distance ){
      dirx = 'L';
    }

    // SCAN FORWARD
    IR_servo.write(90); // turn IR servo Forward
    delay(1000);
    last_distance = IR_scan(); // get distance to obstacle NOTE: be mindful of out of range values for sensor
    delay(500);
    // Serial.print("look_around>> Forward scan distance: ");  Serial.println(last_distance);
    if (last_distance > distance && last_distance > safe_distance ){
      distance = last_distance; // forward space is greater than previous space
      dirx = 'F'; //go forward
    }

    // SCAN RIGHT
    IR_servo.write(0); // turn Right (or Left depending on servo orientation)
    delay(1000);
    last_distance = IR_scan(); // get distance to obstacle NOTE: be mindful of out of range values for sensor
    delay(500);
    // Serial.print("look_around>> Right scan distance: "); Serial.println(last_distance);    
    if (last_distance > distance && last_distance > safe_distance ){
      distance = last_distance; // forward space is greater than previous space
      dirx = 'R'; //go Left (or Right depending on your servo)
    }
    return dirx;  // retun resulting direction. 
    break;
  }
  // Serial.print("look_around>> Go direction: "); Serial.print(dirx); Serial.print("   @distance: "); Serial.println 
(last_distance);
}

// ==================MOVEMENT FUCTION =====================================
// F = forward  R = right  L = left  B = backturn 180  T = reverse S = stop 
char move_to(char dir){
  switch (dir) { // scan ahead
  case 'F':  
    // Serial.println("move_to>> Turn  forward");
    go_forward();
    return 'F';
    break;
  case 'L': 
    // Serial.println("move_to>> Turn  Left");
    go_left(lturn_time);
    go_forward();
    return 'F';
    break;
  case 'R': 
    // Serial.println("move_to>> Turn  Right");
    go_right(rturn_time);
    go_forward();    
    return 'F';
    break; 
  case 'B': 
    // Serial.println("move_to>> Backturn 180");
    go_back(backturn_time);
    go_still();
    return 'S';
    break;
  case 'T': 
    // Serial.println("move_to>> Reverse in straight line");
    go_reverse(reverse_time);
    go_still();
    return 'S';
    break;
  case 'S': 
    // Serial.println("move_to>> Stop");
    go_still();
    return 'S';
    break;
  }
}

// =========================== IR LOGIC ===========================
int IR_scan()  // scan for obstacles then return distance
{
  float IR_distance =0;
  float TOT_IR_distance =0;
  float volts = 0;
  int IR_readings = 10;
  //  Serial.println("==============START REAL TIME IR READING ===========");
  for (int i =0; i < IR_readings ; i++) { 
    volts += analogRead(IR_read_pin) *0.0048828125;  
    IR_distance += 65*pow(volts, -1.10);    // worked out from graph 65 = theoretical distance / (1/Volts)S - luckylarry.co.uk
    //   Serial.println(IR_distance);
    delay(50);   
    //Serial.print("IR_scan>> each IR distance: "); Serial.println(IR_distance);
    //TOT_IR_distance += IR_distance;
  }
  IR_distance  /= IR_readings;
  IR_value_debug = IR_distance;
  // Serial.print("IR_scan>> AVG TOTAL distance: "); Serial.println(IR_distance);
  return IR_distance;
}

// ================== PHYSICAL MOVEMENT ========================

void go_forward()
{
  attach_servos();
  l_servo.write(l_go);
  r_servo.write(r_go);
}

void go_still()
{
  delay(100);
  detach_servos();
  delay(100);
}

void go_reverse(int move_time){ // move straight  but opposite direction
  delay(100);
  attach_servos();
  l_servo.write(l_reverse);
  r_servo.write(r_reverse);
  delay(move_time); // keep moving for seconds before stopping
  detach_servos();  // stop
  delay(100);
}

void  go_back(int move_time){  // turn 180 in place
  delay(100);
  attach_servos();
  l_servo.write(l_reverse);
  r_servo.write(r_go);
  delay(move_time); // keep moving for seconds before stopping
  detach_servos();  // stop
  delay(100);
}

void  go_left(int move_time){
  delay(100);
  attach_servos();
  l_servo.write(l_go);
  r_servo.write(r_reverse);
  delay(move_time); // keep moving for seconds before stopping
  detach_servos();  // stop
  delay(100);
}

void  go_right(int move_time){
  delay(100);
  attach_servos();
  l_servo.write(l_reverse);
  r_servo.write(r_go);
  delay(move_time); // keep moving for seconds before stopping
  detach_servos(); // stop
  delay(100);
}

void detach_servos(){
  delay(100);
  l_servo.detach();  // sure way to stop servo
  r_servo.detach(); // sure way to stop servo
  delay(100);
}

void attach_servos(){
  delay(100);
  l_servo.attach(l_pin);
  r_servo.attach(r_pin);
  delay(100);
}

//================ END ====================

