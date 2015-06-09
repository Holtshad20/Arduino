void init_ardupilot(void)
{
  gps_init_baudrate();
  Serial.begin(9600); 

  //Declaring pins

  pinMode(5,INPUT); // Mode pin (not used)
  pinMode(11,OUTPUT); // Simulator Output pin (not used)
  pinMode(MUX_PIN,OUTPUT);  //MUX pin, applies only to modified Hardware !
  pinMode(BLUE_LED,OUTPUT); // LOCK LED pin in ardupilot board, indicates valid GPS data
  pinMode(YELLOW_LED,OUTPUT);// Status LED, blinks, when valid satellite fix data is received
  pinMode(SERVO1_IN_PIN,INPUT); // Throttle input from RC Rx (only used for RC control)
  pinMode(SERVO2_IN_PIN,INPUT); // Rudder input from RC Rx  (only used for RC control)
  
#ifdef RADIO_CONTROL
  init_RC_control();         // Initialize Radio control
#endif

  switch_to_ardupilot();     // default servo control by Ardupilot
}

void init_startup_parameters(void)
{
  //yeah a do-while loop, checks over and over again until we have valid GPS position and lat is diferent from zero. 
  //I re-verify the Lat because sometimes fails and sets home lat as zero. This way never goes wrong.. 
  do
  {
    gps_parse_nmea(); //Reading and parsing GPS data
  }
  while(((fix_position < 0x01) || (lat == 0)));

  //Another verification
  gps_new_data_flag=0;

  do
  {
    gps_parse_nmea(); //Reading and parsing GPS data  
  }
  while((gps_new_data_flag&0x01 != 0x01) & (gps_new_data_flag&0x02 != 0x02)); 
  rudder_control(); //I've put this here because i need to calculate the distance to the next waypoint, otherwise it will start at waypoint 2.
}

