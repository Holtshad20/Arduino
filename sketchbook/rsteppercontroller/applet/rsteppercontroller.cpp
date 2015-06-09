#include <inttypes.h>
#include "_init.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "WProgram.h"
void setup();
void init_process_string();
void loop();
void motor_init(void);
void motor_on(void);
void motor_off(void);
void myStepper_init(void);
void limitConfig(uint8_t pin);
bool can_move(axis a);
void setStep(uint8_t s);
void myStepper_off(void);
void myStepper_on(void);
void myStepper_reset(void);
void addObj(uint8_t *str);
void purge_commands();
void parse_commands(uint8_t *str);
double getValue(const char x);
bool command_exists(const char x);
void setXYZ(FloatPoint *fp);
void process_string(uint8_t  *instruction);
void bzero(uint8_t *ptr, uint8_t len);
void init_steppers();
axis nextEvent(void);
void r_move(float feedrate);
void set_target(FloatPoint *fp);
void set_position(FloatPoint *fp);
long to_steps(float steps_per_unit, float units);
void calculate_deltas();
uint16_t getMaxFeedrate();
uint16_t _getMaxFeedrate();
bool    abs_mode = true;   //0 = incremental; 1 = absolute
uint8_t stepping = DEFAULT_STEP;
uint8_t command_word[COMMAND_SIZE];
uint8_t serial_count=0;
uint16_t no_data = 0;
uint16_t oldKeys;
axis axis_array[3];
float _feedrate;

struct axis_t xaxis_data;
struct axis_t yaxis_data;
struct axis_t zaxis_data;

axis xaxis;
axis yaxis;
axis zaxis;

void setup() {
  //serial
  Serial.begin(9600);
 
  //my init code
//  keypad_init();
  myStepper_init();
  motor_init();
  _feedrate = getMaxFeedrate();
  
  //reprap init code
  init_steppers();
  init_process_string();
  //increase clock resolution
  TCCR0B &= ~_BV(CS00); //for ATmega168!! XXX: this will mess up millis,micros,delay,delayMicroseconds
 
  Serial.println("start");
}

void init_process_string() {
  for (byte i=0; i<COMMAND_SIZE; i++) command_word[i] = 0;
  serial_count = 0;
}


void loop() {
  uint8_t c;
  uint16_t newKeys;

  //read in characters if we got them.
  if (Serial.available() > 0)	{
    c = (uint8_t)Serial.read();
    no_data = 0;
    command_word[serial_count++] = c;
  } 
  else {
    no_data++;
    delayMicroseconds(100);
  }

  //if theres a pause or we got a real command, do it
  if (serial_count && (c == '\n' || no_data > 100)) {
    command_word[serial_count] = 0; //change \n to null terminator
    process_string(command_word); //do
    init_process_string(); //clear
  }

/*
  //keypad actions
  newKeys = keypad_scan();
  if (newKeys != oldKeys) {
    Serial.println(newKeys,HEX);
    oldKeys = newKeys;
    switch(newKeys) {
      case _BV(1):
      case _BV(2):
      case _BV(3):
      case _BV(4):
      case _BV(5):
      case _BV(6):
      case _BV(7):
      case _BV(8):
      case _BV(9):
      case _BV(10):
      case _BV(11):
      case _BV(12):
      case _BV(13):
      case _BV(14):
      case _BV(15):
      default:
        break;
    }
  }
*/

  //no data?  turn off steppers
  if (no_data > 1000)  {
    disable_steppers();
  }
}
//foo


/*

uint8_t COLUMN[3] = {16,14,17};
uint8_t ROW[4]    = {15,19,18,4};

void keypad_init() {
  int x;
  
  //keypad
  for (x=0;x<3;x++) {
    pinMode(COLUMN[x], OUTPUT);
    digitalWrite(COLUMN[x], HIGH);
  }
  for (x=0;x<4;x++) {
    pinMode(ROW[x], INPUT);
    digitalWrite(ROW[x], HIGH);
  }
}

uint16_t keypad_scan() {
  int x,y;
  uint16_t out = 0;
  for (x=0;x<3;x++) {
    digitalWrite(COLUMN[x], LOW);
    //scan 1st three rows
    for (y=0;y<4;y++) {
      if (digitalRead(ROW[y])==LOW) out |= _BV(y*3+x+1);
    }
    digitalWrite(COLUMN[x], HIGH);
  }
  return out;
}

*/
void motor_init(void) {
  pinMode(MOTOR_PIN, OUTPUT);
}

void motor_on(void) {
  digitalWrite(MOTOR_PIN, HIGH); 
}

void motor_off(void) {
  digitalWrite(MOTOR_PIN, LOW);
}

void myStepper_init(void) {
  //Control Pins
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(SLP, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(STEP_X, OUTPUT); 
  pinMode(STEP_Y, OUTPUT); 
  pinMode(STEP_Z, OUTPUT);
  pinMode(DIR_X, OUTPUT); 
  pinMode(DIR_Y, OUTPUT); 
  pinMode(DIR_Z, OUTPUT);
  
  //deal with limit switches
  limitConfig(MIN_X);
  limitConfig(MAX_X);
  limitConfig(MIN_Y);
  limitConfig(MAX_Y);
  limitConfig(MIN_Z);
  limitConfig(MAX_Z);

  //set pin values to stop PWM
  digitalWrite(STEP_X, LOW); 
  digitalWrite(STEP_Y, LOW); 
  digitalWrite(STEP_Z, LOW);
  digitalWrite(DIR_X, LOW); 
  digitalWrite(DIR_Y, LOW); 
  digitalWrite(DIR_Z, LOW);

  //reset and leave on but dissabled
  disable_steppers(); 
  setStep(DEFAULT_STEP);
  myStepper_reset();
}

/* configure limit switches */
void limitConfig(uint8_t pin) {
  uint8_t p;
  p = (pin & ~(ACTIVE_HIGH | ACTIVE_LOW));
  if (p) pinMode(p, INPUT); //set to input if it's to be used
  if (pin & ACTIVE_LOW) digitalWrite(p, HIGH); //turn on internal pullup if sensing active high
  else digitalWrite(p, LOW); // turn off pullup
}

/* if limit switches are used, detect if they are activated*/
bool can_move(axis a) {
  uint8_t p;
  p = (a->min_pin & ~(ACTIVE_HIGH | ACTIVE_LOW));
  if ((a->min_pin & ACTIVE_HIGH) && digitalRead(p)) return false;
  if ((a->min_pin & ACTIVE_LOW)  &&!digitalRead(p)) return false;
  p = (a->max_pin & ~(ACTIVE_HIGH | ACTIVE_LOW));
  if ((a->max_pin & ACTIVE_HIGH) && digitalRead(p)) return false;
  if ((a->max_pin & ACTIVE_LOW)  &&!digitalRead(p)) return false;
  return true;
}
  

void setStep(uint8_t s) {
  switch (s) {
  case full:
    digitalWrite(MS1, LOW); 
    digitalWrite(MS2,LOW); 
    digitalWrite(MS3,LOW);
    break;
  case half:
    digitalWrite(MS1, HIGH); 
    digitalWrite(MS2,LOW); 
    digitalWrite(MS3,LOW);
    break;
  case quarter:
    digitalWrite(MS1, LOW); 
    digitalWrite(MS2,HIGH); 
    digitalWrite(MS3,LOW);
    break;
  case eighth:
    digitalWrite(MS1, HIGH); 
    digitalWrite(MS2,HIGH); 
    digitalWrite(MS3,LOW);
    break;
  case sixteenth:
    digitalWrite(MS1, HIGH); 
    digitalWrite(MS2,HIGH); 
    digitalWrite(MS3,HIGH);
    break;
  default:
    error("Inavlid stepType");
    break;
  }
}


void myStepper_off(void) {
  digitalWrite(RST, LOW);
  digitalWrite(SLP, LOW);
}

void myStepper_on(void) {
  digitalWrite(RST, HIGH);
  digitalWrite(SLP, HIGH);
  delay(250); //wait a bit
}

void myStepper_reset(void) {
  myStepper_off();
  delay(250);
  myStepper_on();
}



//str: token in the form of Xnnn
//old: head of object chain else null
//returns: head of object chain

struct command_t command_list[MAX_COMMANDS];
uint8_t commandLength = 0;

void addObj(uint8_t *str) {
  command c;
  if (commandLength == MAX_COMMANDS) {
     error("addObj FULL");
     return;
  }
  c = &command_list[commandLength++];
  c->type   = str[0];
  c->value  = strtod((const char*)&str[1], NULL);
}

void purge_commands() {
  commandLength = 0;
}


void parse_commands(uint8_t *str) {
  uint8_t *token;
  
  do {
    token = (uint8_t*)strtok((char*)str, " \t"); //split on spaces and tabs
    str = NULL;
    if (token) addObj(token);
  } while (token);
}


//returns zero if value does not exist.
double getValue(const char x) {
  int i;
  //find entry
  for (i=0; i<commandLength; i++) {
    if (command_list[i].type == x) break;
  } 
  //did we find or run out?
  if (i==commandLength) return 0;
  
  return command_list[i].value;
}


bool command_exists(const char x) {
  for (int i=0; i<commandLength; i++) {
    if (command_list[i].type == x) return 1;
  } 
  return 0;
}



//steps per inch or mm
float _units[3] = {
  X_STEPS_PER_MM,Y_STEPS_PER_MM,Z_STEPS_PER_MM};
float curve_section = CURVE_SECTION_MM;

//our feedrate variables.
float feedrate = 0.0;
long feedrate_micros = 0;



void setXYZ(FloatPoint *fp) {
  fp->x = (command_exists('X')) ? (getValue('X') + ((abs_mode) ? 0 : xaxis->current_units)) :   
  xaxis->current_units;
  fp->y = (command_exists('Y')) ? (getValue('Y') + ((abs_mode) ? 0 : yaxis->current_units)) :   
  yaxis->current_units;
  fp->z = (command_exists('Z')) ? (getValue('Z') + ((abs_mode) ? 0 : zaxis->current_units)) :   
  zaxis->current_units;
}



//Read the string and execute instructions
void process_string(uint8_t  *instruction) {
  uint8_t code;
  uint16_t k;
  float temp;
  //command commands = NULL;
  FloatPoint fp;


  //the character / means delete block... used for comments and stuff.
  if (instruction[0] == '/') 	{
    Serial.println("ok");
    return;
  }

  enable_steppers();
  purge_commands(); //clear old commands
  parse_commands(instruction); //create linked list of arguments
  if (command_exists('G')) {
    code = getValue('G');

    switch(code) {
    case 0: //Rapid Motion
      setXYZ(&fp);
      set_target(&fp);
      r_move(0); //fast motion in all axis
      break;
    case 1: //Coordinated Motion
      setXYZ(&fp);
      set_target(&fp);
      if (command_exists('F')) _feedrate = getValue('F'); //feedrate persists till changed.
      r_move( _feedrate );
      break;
    case 2://Clockwise arc
    case 3://Counterclockwise arc
      FloatPoint cent;
      float angleA, angleB, angle, radius, length, aX, aY, bX, bY;

      //Set fp Values
      setXYZ(&fp);
      // Centre coordinates are always relative
      cent.x = xaxis->current_units + getValue('I');
      cent.y = yaxis->current_units + getValue('J');

      aX = (xaxis->current_units - cent.x);
      aY = (yaxis->current_units - cent.y);
      bX = (fp.x - cent.x);
      bY = (fp.y - cent.y);

      if (code == 2) { // Clockwise
        angleA = atan2(bY, bX);
        angleB = atan2(aY, aX);
      } 
      else { // Counterclockwise
        angleA = atan2(aY, aX);
        angleB = atan2(bY, bX);
      }

      // Make sure angleB is always greater than angleA
      // and if not add 2PI so that it is (this also takes
      // care of the special case of angleA == angleB,
      // ie we want a complete circle)
      if (angleB <= angleA) angleB += 2 * M_PI;
      angle = angleB - angleA;

      radius = sqrt(aX * aX + aY * aY);
      length = radius * angle;
      int steps, s, step;
      steps = (int) ceil(length / curve_section);

      FloatPoint newPoint;
      for (s = 1; s <= steps; s++) {
        step = (code == 3) ? s : steps - s; // Work backwards for CW
        newPoint.x = cent.x + radius * cos(angleA + angle * ((float) step / steps));
        newPoint.y = cent.y + radius * sin(angleA + angle * ((float) step / steps));
        newPoint.z = zaxis->current_units;
        set_target(&newPoint);

        // Need to calculate rate for each section of curve
        feedrate_micros = (feedrate > 0) ? feedrate : getMaxFeedrate();

        // Make step
        r_move(feedrate_micros);
      }

      break;
    case 4: //Dwell
      //delay((int)getValue('P'));
      break;
    case 20: //Inches for Units
      _units[0] = X_STEPS_PER_INCH;
      _units[1] = Y_STEPS_PER_INCH;
      _units[2] = Z_STEPS_PER_INCH;
      curve_section = CURVE_SECTION_INCHES;
      calculate_deltas();
      break;
    case 21: //mm for Units
      _units[0] = X_STEPS_PER_MM;
      _units[1] = Y_STEPS_PER_MM;
      _units[2] = Z_STEPS_PER_MM; 
      curve_section = CURVE_SECTION_MM;
      calculate_deltas();
      break;
    case 28: //go home.
      set_target(&zeros);
      r_move(getMaxFeedrate());
      break;
    case 30://go home via an intermediate point.
      //Set Target
      setXYZ(&fp);
      set_target(&fp);
      //go there.
      r_move(getMaxFeedrate());
      //go home.
      set_target(&zeros);
      r_move(getMaxFeedrate());
      break;
    case 81: // drilling operation
      temp = zaxis->current_units;
      //Move only in the XY direction
      setXYZ(&fp);
      set_target(&fp);
      zaxis->target_units = temp;
      calculate_deltas();
      r_move(getMaxFeedrate());
      //Drill DOWN
      zaxis->target_units = getValue('Z') + ((abs_mode) ? 0 : zaxis->current_units);
      calculate_deltas();
      r_move(getMaxFeedrate());
      //Drill UP
      zaxis->target_units = temp;
      calculate_deltas();
      r_move(getMaxFeedrate());
    case 90://Absolute Positioning
      abs_mode = true;
      break;
    case 91://Incremental Positioning
      abs_mode = false;
      break;
    case 92://Set as home
      set_position(&zeros);
      break;
    case 93://Inverse Time Feed Mode
      break;  //TODO: add this 
    case 94://Feed per Minute Mode
      break;  //TODO: add this
    default:
      Serial.print("huh? G");
      Serial.println(code,DEC);
    }
  }
  if (command_exists('M')) {
    code = getValue('M');
    switch(code) {
    case 3: // turn on motor
    case 4:
      motor_on();
      break;
    case 5: // turn off motor
      motor_off();
      break;
    case 82:
      DDRC |= _BV(1);
      PORTC &= ~_BV(1);
      DDRC &= ~_BV(0);
      PORTC |= _BV(0);
      // setup initial position
      for (int i=0; i<20; i++) {
        k=0;
        PORTB |= _BV(5); //go down
        while(PINC & _BV(0)) {
          PORTB |= _BV(2);
          delayMicroseconds(1);
          PORTB &= ~_BV(2);
          delayMicroseconds(200);
          k++;
        }
        //print result for this point
        Serial.println(k,DEC);
        PORTB &= ~_BV(5);  //move up to origin        
        while (k--) {
          PORTB |= _BV(2);
          delayMicroseconds(1);
          PORTB &= ~_BV(2);
          delayMicroseconds(12.5*stepping);
        }
      }
      break;
    case 81:
      DDRC |= _BV(1);
      PORTC &= ~_BV(1);
      DDRC &= ~_BV(0);
      PORTC |= _BV(0);
      while(1) {
        if (PINC & _BV(0)) Serial.println("high");
        else Serial.println("low");
      }
      break;
    case 80: //plot out surface of milling area
      DDRC |= _BV(1);
      PORTC &= ~_BV(1);
      DDRC &= ~_BV(0);
      PORTC |= _BV(0);
      // setup initial position
      fp.x = 0;
      fp.y = 0;
      fp.z = 0;
      set_target(&fp);
      set_position(&fp);
      r_move(0);
      for (int i=0; i<160; i+=2) {
        for (float j=0; j<75; j+=2) {
          fp.x=i;
          fp.y=j;
          fp.z=0;
          set_target( &fp );
          r_move( 0 );
          k=0;
          PORTB &= ~(_BV(5)); //go down
          while(PINC & _BV(0)) {
            PORTB |= _BV(2);
            delayMicroseconds(1);
            PORTB &= ~_BV(2);
            delayMicroseconds(200);
            k++;
          }
          //print result for this point
          Serial.print(i,DEC);
          Serial.print(",");
          Serial.print(j,DEC);
          Serial.print(",");
          Serial.println(k,DEC);
          PORTB |= _BV(5);  //move up to origin        
          while (k--) {
            PORTB |= _BV(2);
            delayMicroseconds(1);
            PORTB &= ~_BV(2);
            delayMicroseconds(200);
          }
        }
      }
      break;
    case 90: //plot out surface of milling area
      DDRC |= _BV(1);
      PORTC &= ~_BV(1);
      DDRC &= ~_BV(0);
      PORTC |= _BV(0);
      // setup initial position
      fp.x = 0;
      fp.y = 135;
      fp.z = 0;
      set_target(&fp);
      set_position(&fp);
      r_move(0);
      for (int i=0; i<1; i++) {
        for (float j=135; j!=0; j-=.25) {
          fp.x=i;
          fp.y=j;
          fp.z=0;
          set_target( &fp );
          r_move( 0 );
          k=0;
          PORTB |= _BV(5); //go down
          while(PINC & _BV(0)) {
            PORTB |= _BV(2);
            delayMicroseconds(1);
            PORTB &= ~_BV(2);
            delayMicroseconds(200);
            k++;
          }
          //print result for this point
          Serial.print(i,DEC);
          Serial.print(",");
          Serial.print(j,DEC);
          Serial.print(",");
          Serial.println(k,DEC);
          PORTB &= ~_BV(5);  //move up to origin        
          while (k--) {
            PORTB |= _BV(2);
            delayMicroseconds(1);
            PORTB &= ~_BV(2);
            delayMicroseconds(200);
          }
        }
      }
      break;
    case 98: //M98 Find Z0 where it is one step from touching.
      DDRC |= _BV(1);
      PORTC &= ~_BV(1);
      DDRC &= ~_BV(0);
      PORTC |= _BV(0);
      PORTB |= _BV(5);
      while(PINC & _BV(0)) {
        PORTB |= _BV(2);
        delayMicroseconds(1);
        PORTB &= ~_BV(2);
        delayMicroseconds(200);
      }
      break;
    case 99: //M99 S{1,2,4,8,16} -- set stepping mode
      if (command_exists('S')) {
        code = getValue('S');
        if (code == 1 || code == 2 || code == 4 || code == 8 || code == 16) {
          stepping = code;
          setStep(stepping);
          break;
        }
      }
    default:
      Serial.print("huh? M");
      Serial.println(code,DEC);
    }
  }
  Serial.println("ok");//tell our host we're done.
}



















void bzero(uint8_t *ptr, uint8_t len) {
  for (uint8_t i=0; i<len; i++) ptr[i] = 0;
}

void init_steppers(){
  //turn them off to start.
  disable_steppers();

  // setup data
  xaxis = &xaxis_data;
  yaxis = &yaxis_data;
  zaxis = &zaxis_data;

  axis_array[0] = xaxis;
  axis_array[1] = yaxis;
  axis_array[2] = zaxis;

  bzero((uint8_t*)&xaxis_data, sizeof(struct axis_t)); 
  bzero((uint8_t*)&yaxis_data, sizeof(struct axis_t)); 
  bzero((uint8_t*)&zaxis_data, sizeof(struct axis_t)); 

  // configure pins
  xaxis->step_pin = STEP_X;
  yaxis->step_pin = STEP_Y;
  zaxis->step_pin = STEP_Z;

  xaxis->min_pin  = MIN_X;
  yaxis->min_pin  = MIN_Y;
  zaxis->min_pin  = MIN_Z;

  xaxis->max_pin  = MAX_X;
  yaxis->max_pin  = MAX_Y;
  zaxis->max_pin  = MAX_Z;

  xaxis->direct_step_pin = _STEP_X;
  yaxis->direct_step_pin = _STEP_Y;
  zaxis->direct_step_pin = _STEP_Z;

  //figure our stuff.
  calculate_deltas();
}


axis nextEvent(void) {
  if (xaxis->nextEvent < yaxis->nextEvent) {
    return (xaxis->nextEvent <= zaxis->nextEvent) ? xaxis : zaxis;
  } 
  else {
    return (yaxis->nextEvent <= zaxis->nextEvent) ? yaxis : zaxis;
  }
}

/*
void fast_move(void) {
 axis a;
 uint8_t i;
 uint32_t starttime;
 
 xaxis->timePerStep = (1E6*60.0) / (MAX_X_FEEDRATE * X_STEPS_PER_INCH * stepping);
 yaxis->timePerStep = (1E6*60.0) / (MAX_Y_FEEDRATE * Y_STEPS_PER_INCH * stepping);
 zaxis->timePerStep = (1E6*60.0) / (MAX_Z_FEEDRATE * Z_STEPS_PER_INCH * stepping);
 
 // setup axis
 for (i=0;i<3;i++) {
 a = axis_array[i];
 if (axis_array[i]->delta_steps) {
 a->nextEvent = a->timePerStep; //1st event happens halfway though cycle.
 } 
 else {
 a->nextEvent = 0xFFFFFFFF;
 }
 }
 // start move
 starttime = micros();
 while (xaxis->delta_steps || yaxis->delta_steps || zaxis->delta_steps) {
 a = nextEvent();
 while (micros() < (starttime + a->nextEvent) ); //wait till next action is required
 if (can_move(a)) {
 _STEP_PORT |= a->direct_step_pin;
 //need to wait 1uS
 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
 _STEP_PORT &= ~a->direct_step_pin;
 }
 
 if (--a->delta_steps) {
 a->nextEvent += a->timePerStep;
 } 
 else {
 a->nextEvent = 0xFFFFFFFF; 
 }
 }
 
 //we are at the target
 xaxis->current_units = xaxis->target_units;
 yaxis->current_units = yaxis->target_units;
 zaxis->current_units = zaxis->target_units;
 calculate_deltas();
 }
 */



void r_move(float feedrate) {
  uint32_t starttime,duration;
  float distance;
  axis a;
  uint8_t i;

  //uint8_t sreg = intDisable();

  if (!feedrate ) {
    xaxis->timePerStep = (1E6*60.0) / (MAX_X_FEEDRATE * X_STEPS_PER_INCH * stepping);
    yaxis->timePerStep = (1E6*60.0) / (MAX_Y_FEEDRATE * Y_STEPS_PER_INCH * stepping);
    zaxis->timePerStep = (1E6*60.0) / (MAX_Z_FEEDRATE * Z_STEPS_PER_INCH * stepping);
  } 
  else {
    // if (feedrate > getMaxFeedrate()) feedrate = getMaxFeedrate();
    // distance / feedrate * 60000000.0 = move duration in microseconds
    distance = sqrt(xaxis->delta_units*xaxis->delta_units + 
      yaxis->delta_units*yaxis->delta_units + 
      zaxis->delta_units*zaxis->delta_units);
    duration = ((distance * 60000000.0) / feedrate); //in uS
  }

  // setup axis
  for (i=0;i<3;i++) {
    a = axis_array[i];
    if (axis_array[i]->delta_steps) {
      if (feedrate) a->timePerStep = duration / axis_array[i]->delta_steps;
      a->nextEvent = (a->timePerStep>>1); //1st event happens halfway though cycle.
    } 
    else {
      a->nextEvent = 0xFFFFFFFF;
    }
  }

  starttime = myMicros();  
  // start move
  while (xaxis->delta_steps || yaxis->delta_steps || zaxis->delta_steps) {
    a = nextEvent();
    while (myMicros() < (starttime + a->nextEvent) ); //wait till next action is required
    if (can_move(a)) {
      _STEP_PORT |= a->direct_step_pin;
      //need to wait 1uS
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      _STEP_PORT &= ~a->direct_step_pin;
    }
    if (--a->delta_steps) {
      a->nextEvent += a->timePerStep;
    } 
    else {
      a->nextEvent = 0xFFFFFFFF; 
    }
  }

  //we are at the target
  xaxis->current_units = xaxis->target_units;
  yaxis->current_units = yaxis->target_units;
  zaxis->current_units = zaxis->target_units;
  calculate_deltas();

  //Serial.println("DDA_move finished");
  //intRestore(sreg);
}

void set_target(FloatPoint *fp){
  xaxis->target_units = fp->x;
  yaxis->target_units = fp->y;
  zaxis->target_units = fp->z;
  calculate_deltas();
}

void set_position(FloatPoint *fp){
  xaxis->current_units = fp->x;
  yaxis->current_units = fp->y;
  zaxis->current_units = fp->z;
  calculate_deltas();
}


long to_steps(float steps_per_unit, float units){
  return steps_per_unit * units * stepping;
}

void calculate_deltas() {
  //figure our deltas. 
  axis a;
  int i;

  for (i=0; i<3; i++) {
    a = axis_array[i];
    a->delta_units = a->target_units - a->current_units;
    a->delta_steps = to_steps(_units[i], abs(a->delta_units)); //XXX make x_units a vector
    a->direction = (a->delta_units < 0) ? BACKWARD : FORWARD;

    switch(i) {
    case 0: 
      digitalWrite(DIR_X, (a->direction==FORWARD) ? LOW : HIGH); 
      break;
    case 1: 
      digitalWrite(DIR_Y, (a->direction==FORWARD) ? LOW : HIGH); 
      break;
    case 2: 
      digitalWrite(DIR_Z, (a->direction==FORWARD) ? HIGH : LOW); 
      break;
    }
  }
}


uint16_t getMaxFeedrate() {
  uint16_t temp = _getMaxFeedrate();
  return (_units[0] == X_STEPS_PER_MM) ? (temp*25.4) : temp;
}

uint16_t _getMaxFeedrate() {
  if (!xaxis->delta_steps) {
    if (!yaxis->delta_steps) return MAX_Z_FEEDRATE;
    if (!zaxis->delta_steps) return MAX_Y_FEEDRATE;
    return min(MAX_Z_FEEDRATE, MAX_Y_FEEDRATE);
  }
  if (!yaxis->delta_steps) {
    if (!xaxis->delta_steps) return MAX_Y_FEEDRATE;
    if (!zaxis->delta_steps) return MAX_X_FEEDRATE;
    return min(MAX_X_FEEDRATE, MAX_Y_FEEDRATE);
  }
  if (!zaxis->delta_steps) {
    if (!yaxis->delta_steps) return MAX_X_FEEDRATE;
    if (!xaxis->delta_steps) return MAX_Y_FEEDRATE;
    return min(MAX_X_FEEDRATE, MAX_Y_FEEDRATE);
  }
  return min(MAX_X_FEEDRATE, min(MAX_Y_FEEDRATE, MAX_Z_FEEDRATE));
}






int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

