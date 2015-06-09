/*
 * HardwareCounting sketch
 *
 * uses pin 5 on 168/328, pin 47 on Mega
 */

unsigned int count;


unsigned int getCount()
{
   TCCR1B= 0 ;           // Gate Off  / Counter Tn stopped
   count = TCNT1;
   TCNT1 = 0;

   bitSet(TCCR1B ,CS12);  // Counter Clock source is external pin
   bitSet(TCCR1B ,CS11);  // Clock on rising edge
   bitSet(TCCR1B ,CS10);  // you can clear this bit for falling edge

   return count;
}


void setup()
{
  Serial.begin(9600);
  digitalWrite(5, HIGH);
  // hardware counter setup (see ATmega data sheet for details)
  TCCR1A=0;        // reset timer/counter control register A
  getCount();      // this will start the clock
}

void loop()
{
  delay(1000);
  Serial.println(getCount());
}
      
    
