
#define REVERSE_RUDDER 1 // normal = 0 and reverse = 1
#define MIDDLE_RUDDER 90  // central position of rudder servo in degrees, adjust for trim corrections if necessary

// Uncomment the next line if you want to have a "failsafe" RC control over your ship
//#define RADIO_CONTROL  // Radio control for failsafe


//Parameter set for the new catamarane hull

#define KP_HEADING 2.0       // proportional part of PID control
#define KI_HEADING 0.07      // integrator part of PID control
#define KD_HEADING 0.00001   // derivator part of PID control (not used)
#define MOTOR_SPEED 80  // around 5A with Roxxy Outrunner
#define WP_TIMEOUT 15 // Waypoint Timeout counter value in seconds
#define WP_RADIUS 15  // Radius for waypoint-hit in m

//PID max and mins
#define   HEADING_MAX 60  // Servo max position in degrees
#define   HEADING_MIN -60 // Servo min position in degrees

#define INTEGRATOR_LIMIT 240 // about +- 15 degrees

//Defining ranges of the servos (and ESC),Values are in useconds
#define MAX16_THROTTLE 2000 //ESC max position
#define MIN16_THROTTLE 1000 //ESC min position 

#define MAX16_RUDDER 2000 //Rudder Servo max position
#define MIN16_RUDDER 1000 //Rudder Servo min position


// Defining the IO pins, do not touch this settings
#define MUX_PIN 4
#define SERVO1_IN_PIN 2 // RC Receiver Throttle servo input
#define SERVO2_IN_PIN 3 // RC Receiver Rudder servo input
#define BLUE_LED   12   // GPS fix indicator LED
#define YELLOW_LED 13   // Status LED

#define THROTTLE 9
#define RUDDER 10

#define MOTOR_OFF 5 // Setting for ESC, that surely turns the motor off


