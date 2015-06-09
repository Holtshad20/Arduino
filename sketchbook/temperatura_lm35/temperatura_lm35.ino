/*******************************************************
* Example how to use LM35 Temperature Sensor for Arduino 
* in a full-range schema
* You can see the schema or datasheet or at link:
* http://www.ik0jre.net
* Author IK0JRE - Giuseppe Aquilani
* ik0jre@ik0jre.net
* You can use and modify this software under GPL licence 
* and I hope you write my name too in your software.
* We can measure celsius degree in full range scale 
* from -500 mV to 1500 mV 
* for these schema we need read a virtual ground or
* subtract a fixed value of 0.9 mV from analog read.
* For tollerance of electronic components I preferr
* read virtual ground value
* NOTE: from National datasheet in the full range schema
* the limits are
* LM35/LM35A     -55 +150 C
* LM35C/LM35CA   -40 +110 C
* LM35D            0 +100 C
* I tested schema and software with LM35DZ and it works
* under -10 and up +120
*******************************************************/

//ATMEGA328 analog resolution is Vcc/1024
//#define FFACT ((float) 5 / 1024) //analog resolution at +5V

#define MAX_AV_READ  500        //n. read for average
const float SUPPLY_VOLTAGE = 500; // 5V * 100 (LM35 costant)

#define tA0  0      //Arduino analog input 0 connect to +out pin
#define tA1  1      //Arduino analog input 1 connect to -out pin
float tC0;          //A0 read temp var
float tC1;          //A1 read temp var
float tCalc;        //var need for calc celsius degrees
#define FCORR 0.5   // only for test if we don't have precision read
                    //because supply voltage is not exactly +5v
/*******************************************************
* Arduino needs a minimal Setup 
*******************************************************/
void setup()
{
 analogReference(DEFAULT);  // set voltage reference at default 5.0 V for Arduino 1 
 Serial.begin(9600);        //open serial port at 9600 bps
}

/*******************************************************
* Loop
*******************************************************/
void loop()
{
 int i;
 // because at328 analog read is unstable
 // we neeed an average from many read
 // I found in 500 read a good solution between precision
 // and speed
 // in debug mode you can apply a multimeter at 
 // pin +out -out in schema for verify mV read from Arduino 
 
 for(i=0;i<MAX_AV_READ;i++){
    tC0 += analogRead(tA0) ;    //read +out sensor pin
    delay(5);                   //atmega need 10uS delay but we preferr 5mS 
    tC1 += analogRead(tA1) ;    //for full range scale -55 to + 155 C
    }                           //must read -out pin (virtual Vss)
    
  tC0 /=MAX_AV_READ;   //average
  tC1 /=MAX_AV_READ;   //average

  tCalc = tC0 - tC1 ;  //subtract virtual ground value
 // now we can calc celsius degrees 
  tCalc = ( tCalc * SUPPLY_VOLTAGE /1024) - FCORR; 
 //only for debug next three line show millVolt out from LM35
Serial.print(tC0);           //show mV reads at A0 input
Serial.print(" ");           //print space
Serial.print(tC1);           //show mV reads at A1 input

//next show temperature in Celsius scale
Serial.print(" C ");
Serial.println(tCalc);

//delay(500);  //with average don't need delay                           
}
