#include <PID.h>
#include <SENSORS.h>
#define Kp 0.5
#define Ki 0.2
#define Kd 0.6
#define dt 50
#define target 0

long posi;

PID pid;
void setup() 
{
  init_sensors();
  Serial.begin(9600);
}
int gain;
int moterl=0,moterr=0;
void loop() 
{
  Serial.println(gain);
  
  read_sensor();
  
  gain = pid.pid(target,posi,Kp,Ki,Kd,dt);
 
 
}











//##################################################################


  void init_sensors()
  {
    
  for(int i=2;i<=6;i++)
  {
    pinMode(i,INPUT);
  }
  
  }
  


//##################################################################

void read_sensor()
{
  if(digitalRead(2)==HIGH)
  {
    posi=-2;
  }
  
   if(digitalRead(3)==HIGH)
   {
     posi=-1;
   }
   
    if(digitalRead(2)==HIGH && digitalRead(3)==HIGH)
    {
      posi=-1;
    }
    
    
  if(digitalRead(6)==HIGH)
  {
    posi= 2;
  }
  
   if(digitalRead(5)==HIGH)
   {
     posi= 1;
   }
   
   if(digitalRead(6)==HIGH && digitalRead(5)==HIGH)
    {
      posi= 1;
    }
    
    
    if(digitalRead(4)==HIGH)
    {
      posi=0; 
    }
    
    if(digitalRead(2)==HIGH && digitalRead(3)==HIGH && digitalRead(4)==HIGH && digitalRead(5)==HIGH && digitalRead(6)==HIGH)
    {
     //special function like line lost take action 
    }
    
    if(digitalRead(2)==LOW && digitalRead(3)==LOW && digitalRead(4)==LOW && digitalRead(5)==LOW && digitalRead(6)==LOW)
    {
      posi=3;
     //special function like line lost take action 
    }
     
}
//###################################################################
