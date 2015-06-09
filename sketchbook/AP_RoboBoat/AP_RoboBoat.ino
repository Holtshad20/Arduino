/* By Chris Anderson, Jordi Munoz, modified by Harald Molle for the use on model boats */
/* Nov/27/2010
/* Version 1.1 */
/* Released under an Apache 2.0 open source license*/
/* Project home page is at http://DIYdrones.com 
/* We hope you improve the code and share it with us at DIY Drones!*/


#include "defines.h"
#include "waypoints.h"

// Global variables definition
int waypoints;  // waypoint counter 

unsigned int integrator_reset_counter = 0;  // counter variable (in seconds) for the Integrator holdoff-time after a waypoint switch

byte current_wp = 0; //This variable stores the actual waypoint we are trying to reach.. 

int wp_bearing = 0; //Bearing to the current waypoint (in degrees)
unsigned int wp_distance = 0; // Distance to the current waypoint (in meters)

//GPS obtained information
float lat = 0; //Current Latitude
float lon = 0; //Current Longitude
unsigned long time; // curent UTC time
float ground_speed = 0;   // Speed over ground 
int  course = 0;          // Course over ground 
int alt = 0;             //Altitude above sea 

// Flag variables
byte jumplock_wp = 0; // When switching waypoints this lock will allow only one transition..
byte gps_new_data_flag = 0; // A simple flag to know when we've got new gps data.

// rudder setpoint variable, holds the calculated value for the rudder servo
int rudder_setpoint = 0;

byte fix_position = 0; // Flag variable for valid gps position

// Arduino Startup, entry point after power-on
void setup()
{
  
  init_ardupilot(); // Initialize the hardware specific peripherals
    
  waypoints = sizeof(wps) / sizeof(LONLAT); // calculate the number of waypoints 
  
  Init_servo(); //Initalize the servos, see "Servo_Control" tab.
  
  test_rudder(); //Just move the servo to see, that there is something living
  bldc_arm_throttle();  // Initialize the BLDC controller 
 
  print_header(); //print the header line on the debug channel
  
  delay(500);  // wait until UART Tx Buffer is surely purged
  
  init_startup_parameters();  // Wait for first GPS Fix
  
  test_rudder();   // Move rudder-servo to see, that the launch-time is close
  
  bldc_start_throttle(); // start the motor 
  
  delay (5000); // go the first five seconds without GPS control, to get the direction vector stabilized         
  
  init_startup_parameters(); // re-synchronize GPS
 
}

// Program main loop starts here

// Arduino main loop
void loop()
{

#ifdef RADIO_CONTROL   
    if (check_radio() > 0) 
     {   
      switch_to_radio(); 
      while (check_radio() > 0);  // Wait until Radio is switched off
      switch_to_ardupilot(); // servo control back to ardupilot 
     }     
#endif

  gps_parse_nmea(); // parse incoming NMEA Messages from GPS Module and store relevant data in global variables
 
  if((gps_new_data_flag & 0x01) == 0x01)    //Checking new GPS "GPRMC" data flag in position 
  {
    digitalWrite(YELLOW_LED, HIGH); // pulse the yellow LED to indicate a received GPS sentence
    gps_new_data_flag &= (~0x01); //Clearing new data flag... 
    rudder_control(); // Control function for steering the course to next waypoint 
    if (integrator_reset_counter++ < WP_TIMEOUT)    // Force I and D part to zero for WP_TIMEOUT seconds after each waypoint switch
     reset_PIDs();
 
    send_to_ground();   /*Print values on datalogger, if attached, just for debugging*/
  } // end if gps_new_data...


  // Ensure that the autopilot will jump ONLY ONE waypoint
  
    if((wp_distance < WP_RADIUS) && (jumplock_wp == 0x00)) //Checking if the waypoint distance is less than WP_RADIUS m, and check if the lock is open
    {
      current_wp++; //Switch the waypoint
      jumplock_wp = 0x01; //Lock the waypoint switcher.
      integrator_reset_counter = 0;
     
      if(current_wp >= waypoints)    // Check if we've passed all the waypoints, if yes stop motor
       finish_mission();          
     } // end if wp_distance...
 
  digitalWrite(YELLOW_LED,LOW);  //Turning off the status LED
} // end loop ()

