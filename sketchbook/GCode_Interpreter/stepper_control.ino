//================
// Stepper Control
//================

long milli_delay;

boolean panic;

boolean x_can_step;
boolean y_can_step;
boolean z_can_step;

// our direction vars
//===================
boolean x_direction = true;
boolean y_direction = true;
boolean z_direction = true;

// Heartbeat variables
//====================
int g_Beat;
boolean g_Heart;

void init_steppers() {

  // turn them off to start
  //=======================
  disable_steppers();

  // init our points
  //================
  current_units.x = 0.0;
  current_units.y = 0.0;
  current_units.z = 0.0;
  target_units.x = 0.0;
  target_units.y = 0.0;
  target_units.z = 0.0;
  current_steps.x = 0.0;
  current_steps.y = 0.0;
  current_steps.z = 0.0;
  
  inches = true;

  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);

  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_MIN_PIN, INPUT);
  pinMode(Z_MAX_PIN, INPUT);

  pinMode(HEARTBEAT_PIN, OUTPUT);

  // set the pullup resistors
  //=========================
  digitalWrite(PANIC_STOP_PIN, HIGH);
  digitalWrite(PANIC_OVER_PIN, HIGH);
  
  // start in not-panic mode
  //========================
  panic = false;

  // figure our stuff.
  //=================
  calculate_deltas();
  
  // initialise Heartbeat
  //=====================
  InitialiseHeartBeat(false);
}

void dda_move(long micro_delay)
{
  long max_delta;
  long x_counter;
  long y_counter;
  long z_counter;

  // initialise Heartbeat
  //=====================
  InitialiseHeartBeat(true);

  // enable our steppers
  //====================
  enable_steppers();

  // figure out our deltas
  //======================
  max_delta = max(delta_steps.x, delta_steps.y);
  max_delta = max(delta_steps.z, max_delta);

  // init stuff
  //===========
  x_counter = -max_delta/2;
  y_counter = -max_delta/2;
  z_counter = -max_delta/2;

  // our step flags
  //===============
  x_can_step = false;
  y_can_step = false;
  z_can_step = false;

  if (micro_delay >= 16383)
    milli_delay = micro_delay / 1000;
  else
    milli_delay = 0;

  if (DebugLevel > 2) {
    Serial.print("MaxDelta: ");
    Serial.print(max_delta, DEC);
    Serial.print(" MilliDelay: ");
    Serial.print(milli_delay, DEC);
    Serial.print(" MicroDelay: ");
    Serial.println(micro_delay, DEC);
  }
  // do our DDA line!
  do {
    PanicStop();
    x_can_step = can_step(current_steps.x, target_steps.x, x_direction);
    y_can_step = can_step(current_steps.y, target_steps.y, y_direction);
    z_can_step = can_step(current_steps.z, target_steps.z, z_direction);

    if (x_can_step) {
      x_counter += delta_steps.x;

      if (x_counter > 0) {
        do_step(X_STEP_PIN);
        x_counter -= max_delta;

        if (x_direction)
          current_steps.x++;
        else
          current_steps.x--;
      }
    }

    if (y_can_step) {
      y_counter += delta_steps.y;

      if (y_counter > 0) {
        do_step(Y_STEP_PIN);
        y_counter -= max_delta;

        if (y_direction)
          current_steps.y++;
        else
          current_steps.y--;
      }
    }

    if (z_can_step) {
      z_counter += delta_steps.z;

      if (z_counter > 0) {
        do_step(Z_STEP_PIN);
	z_counter -= max_delta;

        if (z_direction)
          current_steps.z++;
        else
          current_steps.z--;
      }
    }
		
    // wait for next step
    //===================
    if (milli_delay > 0)
      delay(milli_delay);			
    else
      delayMicroseconds(micro_delay);
    Heartbeat();
  } while (x_can_step || y_can_step || z_can_step);
	
  // set our current to target
  //==============================
  current_units.x = target_units.x;
  current_units.y = target_units.y;
  current_units.z = target_units.z;
  calculate_deltas();

  // reset Heartbeat
  //================
  InitialiseHeartBeat(false);
}

void InitialiseHeartBeat(boolean Light){
  // prepare Heartbeat
  // in desired state
  //==================
  g_Beat = 0;
  g_Heart = Light;
  if (g_Heart)
    digitalWrite(HEARTBEAT_PIN, HIGH);
  else
    digitalWrite(HEARTBEAT_PIN, LOW);
}

void Heartbeat(){
  // a) flash LED on PIN 13 to indicate still alive
  // b) send char over serial interface to stop it timing out
  //=========================================================

  g_Beat++;
  if (g_Beat > 1000) {
    g_Beat = 0;
    g_Heart = ! g_Heart;
    
    // flash the LED
    //==============
    if (g_Heart) {
      digitalWrite(HEARTBEAT_PIN, HIGH);
      // keep serial port alive
      //=======================
      Serial.print("~");
    }
    else
      digitalWrite(HEARTBEAT_PIN, LOW);
  }
}

bool can_step(long current, long target, byte direction) {
  if (target == current)
    // stop us if we're on target
    //===========================
    return false;
  return true;
}

void do_step(byte step_pin) {
  digitalWrite(step_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(step_pin, LOW);
}

bool read_switch(byte pin) {
  // dual read as crude debounce
  //============================

  if ( SENSORS_INVERTING )
    return !digitalRead(pin) && !digitalRead(pin);
  else
    return digitalRead(pin) && digitalRead(pin);
}

long to_steps(float steps_per_unit, float units) {
  return steps_per_unit * units;
}

void set_target(float x, float y, float z) {
  target_units.x = x;
  target_units.y = y;
  target_units.z = z;

  calculate_deltas();
}

void set_position(float x, float y, float z) {
  current_units.x = x;
  current_units.y = y;
  current_units.z = z;

  calculate_deltas();
}

void calculate_deltas() {
  // figure our deltas
  //==================

  delta_units.x = abs(target_units.x - current_units.x);
  delta_units.y = abs(target_units.y - current_units.y);
  delta_units.z = abs(target_units.z - current_units.z);

  // set our steps current, target, and delta
  //=========================================
  current_steps.x = to_steps(x_units, current_units.x);
  current_steps.y = to_steps(y_units, current_units.y);
  current_steps.z = to_steps(z_units, current_units.z);

  target_steps.x = to_steps(x_units, target_units.x);
  target_steps.y = to_steps(y_units, target_units.y);
  target_steps.z = to_steps(z_units, target_units.z);

  delta_steps.x = abs(target_steps.x - current_steps.x);
  delta_steps.y = abs(target_steps.y - current_steps.y);
  delta_steps.z = abs(target_steps.z - current_steps.z);

  // what is our direction
  //======================
  x_direction = (target_units.x >= current_units.x);
  y_direction = (target_units.y >= current_units.y);
  z_direction = (target_units.z >= current_units.z);

  if (DebugLevel > 2) {
    Serial.print("currentX: ");
    Serial.print(current_steps.x, 4);
    Serial.print(" targetX: ");
    Serial.print(target_steps.x, 4);
    Serial.print(" direction: ");
    Serial.println(x_direction, DEC);
    Serial.print("currentY: ");
    Serial.print(current_steps.y, 4);
    Serial.print(" targetY: ");
    Serial.print(target_steps.y, 4);
    Serial.print(" direction: ");
    Serial.println(y_direction, DEC);
  }

  //set our direction pins as well
  //==============================
  digitalWrite(X_DIR_PIN, x_direction);
  digitalWrite(Y_DIR_PIN, y_direction);
  digitalWrite(Z_DIR_PIN, z_direction);
}

long calculate_feedrate_delay(float feedrate) {
  // calculate feed rate delay depending on distance and feed rate
  //==============================================================

  float delay;
  long master_steps;
  float distance;
  
  // how long is our traverse distance
  //==================================
  distance = sqrt(delta_units.x*delta_units.x + delta_units.y*delta_units.y + delta_units.z*delta_units.z);
  master_steps = 0;

  // find the dominant axis
  //=======================
  if (delta_steps.x > delta_steps.y) {
    if (delta_steps.z > delta_steps.x)
      master_steps = delta_steps.z;
    else
      master_steps = delta_steps.x;
  }
  else {
    if (delta_steps.z > delta_steps.y)
      master_steps = delta_steps.z;
    else
      master_steps = delta_steps.y;
  }

  // calculate delay between steps in microseconds.  this is sort of tricky, but not too bad.
  // the formula has been condensed to save space.  here it is in english:
  // distance / feedrate * 60000000.0 = move duration in microseconds
  // move duration / master_steps = time between steps for master axis.
  //===================================================================

  delay = ((distance * 60000000.0) / feedrate) / master_steps;
  return delay;	
}

long getMaxSpeed() {
   // get maximum speed
   //==================

  if (delta_steps.z > 0)
    // Z move so use max Z speed
    //==========================
    return calculate_feedrate_delay(FastFeedRateZ);
  else
    // use max XY speed
    //=================
    return calculate_feedrate_delay(FastFeedRateXY);
}

void disable_steppers() {
  // disable all steppers
  //=====================

  digitalWrite(X_ENABLE_PIN, HIGH);
  digitalWrite(Y_ENABLE_PIN, HIGH);
  digitalWrite(Z_ENABLE_PIN, HIGH);
  if (DebugLevel > 2) {
    if (steppers) {
      Serial.println("disable steppers");
    }
  }
  steppers = false;
}

void enable_steppers() {
  // enable all steppers
  //====================

  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);
  if (DebugLevel > 2) {
    if (!steppers) {
      Serial.println("enable steppers");
    }
  }
  steppers = true;
}

bool PanicStop() {
  // check for and handle PANIC buttons
  //===================================
  
  if (digitalRead(PANIC_STOP_PIN) == LOW) {
    // user hit PANIC STOP button
    //===========================
    disable_steppers();
    if (!panic) {
      panic = true;
      Serial.println("PANIC");
    }

    // now wait for "end-of-panic"
    //============================
    while (digitalRead(PANIC_OVER_PIN) == HIGH) {
      // wait for user
      //==============
    }

    // user hit "panic over" button
    //=============================
    enable_steppers();
    if (panic) {
      panic = false;
      Serial.println("PANIC end");
    }
  }
}

