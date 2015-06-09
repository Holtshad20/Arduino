/*
 * InputCapture
 * uses timer hardware to measure pulses on pin 8
 */

const int inputCapturePin = 8;     // input pin cannot be changed

const int prescale = 8;            // prescale factor (each tick 0.5 us @16MHz)
const byte prescaleBits = B010;    // see Table 18-1 or Datasheet
// calculate time per counter tick in ns
const long  precision = (1000000/(F_CPU/1000)) * prescale  ;   

const int numberOfEntries = 64;    // the number of pulses to measure

volatile byte index = 0;
volatile unsigned int results[numberOfEntries]; // note this is 16 bit value

/* ICR interrupt vector */
ISR(TIMER1_CAPT_vect)
{
  TCNT1 = 0;                            // reset the counter
  if( index != 0 || bitRead(TCCR1B ,ICES1) == true)   // wait for rising edge
  {                                     // falling edge was detected
    results[index] = ICR1;              // save the input capture value
    index = index + 1;
  }
  TCCR1B ^= _BV(ICES1);            // toggle bit to trigger on the other edge
}

void setup() {
  Serial.begin(9600);
  pinMode(inputCapturePin, INPUT); // ICP pin (digital pin 8 on Arduino) as input

  TCCR1A = 0 ;                    // Normal counting mode
  TCCR1B = prescaleBits ;         // set prescale bits
  TCCR1B |= _BV(ICES1);           // enable input capture

  bitSet(TIMSK1,ICIE1);           // enable input capture interrupt for timer 1
  Serial.println( precision);     // report duration of each tick in microseconds
}

// this loop prints the number of pulses in the last second, showing min 
// and max pulse widths
void loop() {
  if(index >= numberOfEntries)
  {
    Serial.println("Durations in Microseconds are:") ;
    for( byte i=0; i < numberOfEntries; i++)
    {
      long duration;
      duration = results[i] * precision; // pulse duration in nanoseconds
      Serial.println(duration / 1000);   // duration in microseconds
    }
    while(1)    // loop so the print statements are executed once only
      ;
  }
}
      
    
