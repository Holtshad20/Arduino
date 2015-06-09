// constant for mm conversions
//============================
#define MM_PER_INCH 25.4

//=====================================
// define the parameters of our machine
//=====================================
#define X_STEPS_PER_INCH 32000
#define Y_STEPS_PER_INCH 32000
#define Z_STEPS_PER_INCH 32000

//==================================
//our maximum feedrates units/minute
//==================================
#define FAST_XY_FEEDRATE_INCH 20.0
#define FAST_Z_FEEDRATE_INCH  2.0

//=======================
// Units in curve section
//=======================
#define CURVE_SECTION_MM 0.5

//=========================================
// Set to one if sensor outputs inverting 
// (ie: 1 means open, 0 means closed)
// RepRap opto endstops are *not* inverting
//=========================================
#define SENSORS_INVERTING 0

//===============================================
// digital i/o pin assignment
// odd choices are for convenience to connectors
// NB Arduino pins 14-19 correspond to analog 0-5
//===============================================

//============================
// pin 0 used for serial comms
// pin 1 used for serial comms
//============================
#define SERIAL_RX_PIN 0
#define SERIAL_TX_PIN 1

//=======================================================
// shared enable pin (for rapid shutdown off all motors!)
//=======================================================
#define X_ENABLE_PIN 8
#define Y_ENABLE_PIN 17  // sb8 - not used - defined below for panic stop
#define Z_ENABLE_PIN 19  // sb8 - not used - defined below for LCD

//===========================
// cartesian bot pins X-plane
// 6 7 A0 A1
//===========================
#define X_STEP_PIN 7  //Blue
#define X_DIR_PIN 6   //Yellow
#define X_MIN_PIN 14  // not used
#define X_MAX_PIN 15  // not used

//===========================
// cartesian bot pins Y-plane
// 2 3 4 5
//===========================
#define Y_STEP_PIN 2  //Blue
#define Y_DIR_PIN 3   //Yellow
#define Y_MIN_PIN 5   // not used
#define Y_MAX_PIN 4   // not used

//==================================================
// pin 13 used for heartbeat as it already has a LED
// flashes light on and off while processing commands
// to keep port from timing out
//==================================================
# define HEARTBEAT_PIN 13

//===========================
// cartesian bot pins Z-plane
// 9 10 11 12
//===========================
#define Z_STEP_PIN 9  //Blue
#define Z_DIR_PIN 10  //Yellow
#define Z_MIN_PIN 5  // sb 11 - not used
#define Z_MAX_PIN 4  // sb 12 - not used

//==============
// motor control
// A4
//==============
#define MOTOR_ON_PIN 18           // HIGH = ON, LOW = OFF

//========================================
// motor direction doesn't have a pin yet!
//========================================
// #define MOTOR_DIRECTION_PIN 20    // HIGH = Clockwise, LOW = Anti-clockwise

//======================
// panic stop/reset
// A2 A3
//======================
#define PANIC_STOP_PIN 16  // not used
#define PANIC_OVER_PIN 17  // not used

//==================================================
// pin 19 (A5) is used for serial output to display
//==================================================
# define LCD_SERIAL_PIN 19 - not used
