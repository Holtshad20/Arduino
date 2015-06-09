#include <S4power.h>

S4Power s4power;

int count = 0;

void setup() {
  s4power.Config();
}

void loop(){
   count = -20;
   s4power.M1.speed = count;
   s4power.M1.Update();
   delay (2000);
   
   count = 20;
   s4power.M1.speed = count;
   s4power.M1.Update();
   delay (2000);
}


