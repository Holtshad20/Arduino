#include <S4power.h>
S4Power s4power;
void setup()
{
 s4power.Config();
}
void loop()
{
 for ( int count = -20; count < 20; count++ )
     {
         s4power.M1.speed = count;
         s4power.M2.speed = count;
         s4power.M3.speed = count;
         s4power.M4.speed = count;
         
         s4power.M1.Update();
         s4power.M2.Update();
         s4power.M3.Update();
         s4power.M4.Update();
         
         s4power.light.intensity = count;
         s4power.light.Update();
 
         delay (100);
     }
}


