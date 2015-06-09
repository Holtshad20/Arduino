/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: Arduino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
boolean Checksum(char instruction[], int size);
boolean ProcessComment (char instruction[], int size);
boolean ProcessGcode (char instruction[], int size);
void DebugUnits();
void UnitsInches();
void UnitsMM();
boolean ProcessMcode(char instruction[], int size);
void init_process_string();
void process_string(char instruction[], int size);
double search_string(char key, char instruction[], int string_size);
bool has_command(char key, char instruction[], int string_size);
void StartSpindle(boolean clockwise);
void StopSpindle();
void init_steppers();
void dda_move(long micro_delay);
void InitialiseHeartBeat(boolean Light);
void Heartbeat();
bool can_step(long current, long target, byte direction);
void do_step(byte step_pin);
bool read_switch(byte pin);
long to_steps(float steps_per_unit, float units);
void set_target(float x, float y, float z);
void set_position(float x, float y, float z);
void calculate_deltas();
long calculate_feedrate_delay(float feedrate);
long getMaxSpeed();
void disable_steppers();
void enable_steppers();
bool PanicStop();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\GCode_Interpreter.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\_init.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\processComment.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\processGcode.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\processMcode.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\process_string.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\spindle_control.ino"
#include "E:\Documentos\Arduino\Sketches\GCode_Interpreter\stepper_control.ino"
