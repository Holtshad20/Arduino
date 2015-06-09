#define NUMREADINGS 5

//**********************Setup Variables for the IR Sensor*********************
//****************************************************************************
int IRpin = 5;
unsigned long IRDT = 0;
int ledpin = 13;
float distance = 0.00;
//****************************************************************************
//****************************************************************************

//**********************Setup Variables for Gyro and Accel********************
//****************************************************************************
int readings[NUMREADINGS]; // the readings from the analog input
int index = 0; // the index of the current reading
int total = 0; // the running total
double average = 0.00; // the average
int rVal = 0;
int refPin = 1;
int rPin = 0;
int yPin = 2;
//*****************************************************************************
//*****************************************************************************

//********************Setup other Balancing Variables**************************
//*****************************************************************************
double vTorque = 0.0;
int m1Dir1 = 8;
int m1Dir2 = 9;
int m1PWM = 11;
int m2Dir1 = 7;
int m2Dir2 = 6;
int m2PWM = 10;

double vAngle = 0.0;
double vHighPass = 0.0;
double vLowPass = 0.0;
double vSetpoint = 0.00;
double vRate = 0.0;
double last_angle = 0.0;
double Pterm = 0.0;
double Iterm = 0.0;
double Istate = 0.0;
double Dterm = 0.0;
double Pgain = 1.9;
double Igain = 0.12;
double Dgain = 2.7;

int mydt = 5;
unsigned long last_PID = 0;
unsigned long lastread = 0;
double vYaxis = 0.00;

//*****************************************************************************
//*****************************************************************************

//***********************Variables for driving*********************************
//*****************************************************************************
#define encoder0PinA 3
#define encoder0PinB 4
int vrDist = 0;
int vlDist = 0;
int last_vrDist = 0;
int last_vlDist = 0;
int vrencodeb = 0;
double vrSpeed = 0;
double last_vrSpeed = 0;
int vlSpeed = 0;
int vTurn = 0;
int vTurnR = 0;
int vTurnL = 0;
int vDrive = 0;
int vAdjust = 0;
int vBalAdjust = 0;
int vrDirection = 0;
int last_vrDirection = 0;
int myBaldt = 50;
unsigned long lastBalupdate = 0;
double sPterm = 0.0;
double sIterm = 0.0;
double sIstate = 0.0;
double sDterm = 0.0;
double sPgain = 3.5;
double sIgain = 1.0;
double sDgain = 4.5;
double vrTarget_Speed = 0;

void setup() {
  Serial.begin(9600); // initialize serial communication with computer
  //Setup Motor pins*******************
  pinMode(m1Dir1,OUTPUT);
  pinMode(m1Dir2,OUTPUT);
  pinMode(m2Dir1,OUTPUT);
  pinMode(m2Dir2,OUTPUT);
  pinMode(m1PWM,OUTPUT);
  pinMode(m2PWM,OUTPUT);
  //Calibrate balance point************
  delay(100);
  for (int a = 0; a < 20; a ++) { 
    readings[a] = analogRead(yPin); // read from the sensor 
    if (a != 5) total += readings[a]; // add the reading to the total 
    delay(40); 
    Serial.println(readings[a]); 
  } 
  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinA, HIGH); // turn on pullup resistor 
  digitalWrite(encoder0PinB, HIGH); 
  attachInterrupt(1, rencoder, FALLING); 
  rVal = analogRead(refPin); 
  average = analogRead(yPin);
//  vSetpoint = average – rVal;
} 
  
void loop() { 
  Complimentaryfilter();
  Calculate_Torque();
  Calculate_Speed();
  Read_IR();
} 

void Calculate_Torque() { 
  if ((millis()-last_PID) >= mydt) { // sample every dt ms -> 1000/dt hz.
    last_PID = millis();
    Pterm = vAngle * Pgain;
    Istate = (Istate + vAngle);
    Iterm = Istate * Igain;
    Dterm = (vAngle – last_angle) * Dgain;

    vTorque = Pterm + Dterm + Iterm;

    last_angle = vAngle;

    vTorque = constrain(vTorque,-255,255);


    Drive_Motor(vTorque); //Send torque to motor drive code
  }
  //*************End Setup PID Loop************************************************
  //*******************************************************************************
  //*******************************************************************************
}

//*********************Drive Motors**********************************************
//*******************************************************************************
//*******************************************************************************
int Drive_Motor(int vTorque) {
if (vTorque >= 0) //indicates drive motors forward
{
if (vTurn >= 0) //indicates turn to the right
{
vTurnR = vTorque + vTurn;
vTurnL = vTorque – vTurn;
digitalWrite(m1Dir1, LOW); //Signals H-Bridge to move power forward
digitalWrite(m1Dir2, HIGH);

if (vTurnL < 0)
{
digitalWrite(m2Dir1, HIGH); //Signals H-Bridge to move power forward
digitalWrite(m2Dir2, LOW);
}
else
{
digitalWrite(m2Dir1, LOW); //Signals H-Bridge to move power forward
digitalWrite(m2Dir2, HIGH);
}
vTurnL = abs(vTurnL);
vTurnR = abs(vTurnR);
analogWrite(m1PWM,vTurnR);
analogWrite(m2PWM,vTurnL);
}

if (vTurn < 0) //indicates turn to the left
{
vTurnR = vTorque + vTurn;
vTurnL = vTorque – vTurn;
digitalWrite(m1Dir1, LOW); //Signals H-Bridge to move power forward
digitalWrite(m1Dir2, HIGH);

if (vTurnL < 0)
{
digitalWrite(m2Dir1, HIGH); //Signals H-Bridge to move power forward
digitalWrite(m2Dir2, LOW);
}
else
{
digitalWrite(m2Dir1, LOW); //Signals H-Bridge to move power forward
digitalWrite(m2Dir2, HIGH);
}
vTurnR = abs(vTurnR);
vTurnL = abs(vTurnL);
analogWrite(m1PWM,vTurnR);
analogWrite(m2PWM,vTurnL);
}
}

else if (vTorque < 0) 
{ 
vTorque = abs(vTorque); 

if (vTurn >= 0) //indicates turn to the right
{
vTurnR = vTorque – vTurn;
vTurnL = vTorque + vTurn;
digitalWrite(m2Dir1, HIGH); //Signals H-Bridge to move power forward
digitalWrite(m2Dir2, LOW);

if (vTurnR < 0)
{
digitalWrite(m1Dir1, LOW); //Signals H-Bridge to move power forward
digitalWrite(m1Dir2, HIGH);
}
else
{
digitalWrite(m1Dir1, HIGH); //Signals H-Bridge to move power forward
digitalWrite(m1Dir2, LOW);
}
vTurnL = abs(vTurnL);
vTurnR = abs(vTurnR);
analogWrite(m1PWM,vTurnR);
analogWrite(m2PWM,vTurnL);
}

if (vTurn < 0) //indicates turn to the left
{
vTurnR = vTorque – vTurn;
vTurnL = vTorque + vTurn;
digitalWrite(m2Dir1, HIGH); //Signals H-Bridge to move power forward
digitalWrite(m2Dir2, LOW);

if (vTurnR < 0) { 
digitalWrite(m1Dir1, LOW); //Signals H-Bridge to move power forward 
digitalWrite(m1Dir2, HIGH); } 
else { digitalWrite(m1Dir1, HIGH); //Signals H-Bridge to move power forward 
digitalWrite(m1Dir2, LOW); 
} 
vTurnR = abs(vTurnR); 
vTurnL = abs(vTurnL); 
analogWrite(m1PWM,vTurnR); 
analogWrite(m2PWM,vTurnL); 
} 
} 
//******** End of Motor Drive Function******************************************* 
//******************************************************************************* 
//******************************************************************************* 
} 
//**************************Complimentary Filtering************************************* 
//******************************************************************************* 
void Complimentaryfilter() { 
  if((millis()-lastread) >= mydt) // sample every dt ms -> 1000/dt hz.
{
lastread = millis();

rVal = analogRead(refPin);

vRate = (analogRead(rPin))- (rVal + 14.00); // read from the gyro sensor
vYaxis = (analogRead(yPin))-(rVal – vSetpoint);

//Calculate angle

vAngle += vRate * 0.1;
vHighPass = 0.97 * vAngle;
vLowPass = 0.03 * vYaxis;
vAngle = vHighPass + vLowPass;

}
//***********************************************************************
//*******************End Kalman Filtering********************************
}

//********************Calculate encoder************************************
//*************************************************************************

void rencoder()
{
if (digitalRead(encoder0PinB) == HIGH) vrDist ++;
if (digitalRead(encoder0PinB) == LOW) vrDist –;
}

//********************Calculate speed**************************************
//*************************************************************************

void Calculate_Speed()
{
if((millis()-lastBalupdate) >= myBaldt)
{
lastBalupdate = millis();

vrSpeed = vrDist – last_vrDist;
vrSpeed = (vrSpeed / myBaldt);
vrSpeed = vrSpeed * 15.5;

last_vrDist = 0;
vrDist = 0;

sPterm = vrSpeed * sPgain;
sIstate = (sIstate + vrSpeed);
sIterm = sIstate * sIgain;
sDterm = (vrSpeed – last_vrSpeed) * sDgain;

last_vrSpeed = vrSpeed;

vrSpeed = 0.0;

vrSpeed = sPterm + sDterm;

vrSpeed = map(vrSpeed,-400,400,-30,30);

if (distance < 50.0){ 
  vrTarget_Speed = 5; 
  digitalWrite(ledpin, HIGH); 
} else{ 
  vrTarget_Speed = 0; 
  digitalWrite(ledpin, LOW); 
  } 
vrSpeed = vrSpeed + vrTarget_Speed; //
Serial.println(vrSpeed); 
vAngle = vAngle – vrSpeed; 
} 
} 
//********************Read IR********************************************** 
//************************************************************************* 
void Read_IR() { 
  if((millis()-IRDT) >= 100)
{
IRDT = millis();
float volts = analogRead(IRpin)*0.0048828125; // value from sensor * (5/1024) – if running 3.3.volts then change 5 to 3.3
distance = 65*pow(volts, -1.10);
//Serial.println(distance);
}
}
