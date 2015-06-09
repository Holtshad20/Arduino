/**************************************************************
* Configuring the PWM hadware... If you want to understand this,
*  you must read the Data Sheet of atmega168..  
* The following functionsare optimized for speed. The Arduino Servo library
* may not work, because it consumes more processing time than this ones
***************************************************************/

void Init_servo(void)//This part will configure the PWM to control the servo 100% by hardware, and not waste CPU time.. 
{   
  digitalWrite(RUDDER,LOW);//Defining servo output pins
  pinMode(RUDDER,OUTPUT);
  digitalWrite(THROTTLE,LOW);
  pinMode(THROTTLE,OUTPUT);
  
  /*Timer 1 settings for fast PWM*/
  //Note: these strange strings that follow, like OCRI1A, are actually predefined Atmega168 registers. 
  // We load the registers and the chip does the rest.

    //Remember the registers not declared here remain zero by default... 
  TCCR1A =((1<<WGM11)|(1<<COM1B1)|(1<<COM1A1)); //Please read page 131 of DataSheet, we are changing the registers settings of WGM11,COM1B1,COM1A1 to 1 thats all... 
  TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11); //Prescaler set to 8, that give us a resolution of 2us, read page 134 of data sheet
  OCR1A = 2000; //the period of servo 1, remember 2us resolution, 2000/2 = 1000us the pulse period of the servo...    
  OCR1B = 3000; //the period of servo 2, 3000/2=1500 us, more or less is the central position... 
  ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000, 
  //must be 50hz because is the servo standard (every 20 ms, and 1hz = 1sec) 1000ms/20ms=50hz, elementary school stuff... 
}

/**************************************************************
 * Function to pulse the throttle servo
 ***************************************************************/
void pulse_servo_throttle(long angle)//Will convert the angle to the equivalent servo position... 
{
  //angle=constrain(angle,180,0);
  OCR1A = ((angle * (MAX16_THROTTLE - MIN16_THROTTLE)) / 180L + MIN16_THROTTLE) * 2L;
}

/**************************************************************
 * Function to pulse the yaw/rudder servo... 
 ***************************************************************/
void pulse_servo_rudder(long angle) // converts the angle to the equivalent servo position... 
{
 OCR1B = ((angle  *(MAX16_RUDDER - MIN16_RUDDER)) / 180L + MIN16_RUDDER) * 2L; 
}


void bldc_arm_throttle(void) // "arm" the BLDC controller for the throttle
{
  delay(2000);
  bldc_stop_throttle();  // then switch to approx. zero, Servo controller armed
  delay(4000);
}


void bldc_start_throttle(void)  // brushless Motor (Multiplex controller)
{
  pulse_servo_throttle(MOTOR_SPEED); // set Motor speed
}

// function to stop the Motor   // brushless Motor (Multiplex controller)
void bldc_stop_throttle(void)
{
  pulse_servo_throttle(MOTOR_OFF);  // switch to approx. zero
}

void test_rudder(void)
{
  pulse_servo_rudder(MIDDLE_RUDDER + HEADING_MIN);
  delay(1500);
  pulse_servo_rudder(MIDDLE_RUDDER + HEADING_MAX);
  delay(1500);
  pulse_servo_rudder(MIDDLE_RUDDER);
  delay(1500);
}


// Module to control the ArduPilot via Radio Control (RC)
// You have to use an RC equipment, that supports a failsafe functionality
// e.g. if the Transmitter is switched OFF, on the receiver channel there should be
// "silence" (either HIGH or LOW level)
// I actually have tested this with a 2.4GHZ SPEKTRUM System. 
// Analog Systems may always output some pulses due to erroneous received signals
// My cheap 27MHz Radio control did not work
// Please check thoroughly, before you make you first start!


// Function to Check, if there are pulses on the Rx rudder Input
// I took the rudder channel, because on the SPEKTRUM, the failsafe function
// outputs pulses on the throttle channel (default speed), when the Transmitter is OFF.
// This function checks for "silence" on the rudder channel. 
// If there is silence, the Transmitter is switched OFF and the control should be given to
// the ArduPilot




// Return 0 if no pulse available (timeout > 25ms)
int check_radio(void)  
{
  return (int) pulseIn(SERVO2_IN_PIN, HIGH, 25000); // Check, if there are pulses on the Rx rudder Input;
}


// Function to switch the Multiplexer to the ArduPilot
void switch_to_ardupilot (void)
{
  digitalWrite(MUX_PIN,HIGH);  //  servos controlled by Ardupilot  
}

// Function to switch the Multiplexer to the RC Receiver
void switch_to_radio (void)
{
  digitalWrite(MUX_PIN,LOW);  //  servos controlled by Radio control  
}





