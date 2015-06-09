//================
// spindle control
//================

void StartSpindle(boolean clockwise) {
  // start spindle
  //====================================
  pinMode(MOTOR_ON_PIN, OUTPUT);
  digitalWrite(MOTOR_ON_PIN, HIGH);
}

void StopSpindle() {
  // stop the spindle
  //=================
  digitalWrite(MOTOR_ON_PIN, LOW);
}
