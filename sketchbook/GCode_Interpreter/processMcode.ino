boolean ProcessMcode(char instruction[], int size) {

  float ParameterValue;

  // find which M code
  //==================
  code = search_string('M', instruction, size);
  switch (code) {

//========================================================================

    case -1:  // no code found
      return true;
      break;

// M0 ========================================================================

    case 0: // program stop
      if (DebugLevel > 1)
        Serial.println("program stop");
      
      // finished with engines
      //======================
      StopSpindle();
      disable_steppers();
      return true;
      break;

// M2 ========================================================================

    case 2:  //program end
      if (DebugLevel > 1)
        Serial.println("program end");

      // finished with engines
      //======================
      StopSpindle();
      disable_steppers();
      return true;
      break;

// M3 ========================================================================

    case 3:  // turn spindle clockwise
      if (DebugLevel > 1)
        Serial.println("turn spindle clockwise");

      // gentlemen, start your engines
      //==============================
      StartSpindle(true);
      return true;
      break;

// M4 ========================================================================

    case 4:  // turn spindle anti-clockwise
      if (DebugLevel > 1)
        Serial.println("turn spindle anti-clockwise");

      // gentlemen, start your engines
      //==============================
      StartSpindle(false);
      return true;
      break;

// M5 ========================================================================

    case 5:  // stop spindle
      if (DebugLevel > 1)
        Serial.println("stop spindle");

      // stop the spindle
      //=================
      StopSpindle();
      return true;
      break;

// M110 ========================================================================

    case 110:  // set line number
      // since this arrives with an "N" line number, there's nothing to do!
      //===================================================================
      if (DebugLevel > 1) {
        Serial.print("set line number: ");
        Serial.println(line, DEC);
      }
      return true;
      break;

// M111 ========================================================================

    case 111:  // set debug level
      ParameterValue = search_string('P', instruction, size);
      DebugLevel = ParameterValue;
      if (DebugLevel > 1) {
        Serial.print("debug level ");
        Serial.println(DebugLevel, DEC);
      }
      return true;
      break;

// M114 ========================================================================

    case 114:  // get position
    
      // send current co-ordinates
      //==========================
      Serial.print("ok ");
      Serial.print(line, DEC);
      Serial.print(" C: X:");
      Serial.print(current_units.x, 4);
      Serial.print(" Y:");
      Serial.print(current_units.y, 4);
      Serial.print(" Z:");
      Serial.print(current_units.z, 4);

      // send limit switches
      //====================
      Serial.print(" XMin: ");
      Serial.print(read_switch(X_MIN_PIN), DEC);
      Serial.print(" XMax: ");
      Serial.print(read_switch(X_MAX_PIN), DEC);
      Serial.print(" YMin: ");
      Serial.print(read_switch(Y_MIN_PIN), DEC);
      Serial.print(" YMax: ");
      Serial.print(read_switch(Y_MAX_PIN), DEC);
      Serial.print(" ZMin: ");
      Serial.print(read_switch(Z_MIN_PIN), DEC);
      Serial.print(" ZMax: ");
      Serial.print(read_switch(Z_MAX_PIN), DEC);
      Serial.println();
      return false;
      break;

// M115 ========================================================================

    case 115:  // get firmware version
      // send version info
      //==================
      Serial.print("ok ");
      Serial.print(line, DEC);
      Serial.print(" PROTOCOL VERSION: ");
      Serial.print(FIRMWARE_VERSION);
      Serial.print(" FIRMWARE_NAME: mmcpix CNC");
      Serial.print(" MACHINE_TYPE: mmcpix");
      Serial.println(" EXTRUDER_COUNT: 0");
      return false;
      break;

// M226 ========================================================================

    case 226:  // Pause until user hits clear button
      if (DebugLevel > 1)
        Serial.println("pause");
      while (digitalRead(PANIC_OVER_PIN) == HIGH) {
        // wait for user
        //==============
      }
      // user hit "clear" button
      //=============================
      if (DebugLevel > 1)
        Serial.println("continue");
      return true;
      break;

// M254 ========================================================================

    case 254:  // Program start

      // reset debug level and line number
      //==================================
      DebugLevel = 0;
      line = 0;
      
      // and initialise the stepper sub-system
      //======================================
      init_steppers();
      
      // units = inches
      //===============
      UnitsInches();
      
      // absolute mode
      //==============
      abs_mode = true;
      return true;

// M255 ========================================================================

    case 255:  // request command completion status
    
      // set line to last completed
      //===========================
      line = LastLine;
      
      // return line number of last completed command
      //=============================================
      Serial.print("cp ");
      Serial.println(LastLine, DEC);
      return false;
      break;

// M?? ========================================================================

    default:
      Serial.print("Huh? M");
      Serial.println(code, DEC);
      return true;
  }
}   // end of M code processing
    //=========================



