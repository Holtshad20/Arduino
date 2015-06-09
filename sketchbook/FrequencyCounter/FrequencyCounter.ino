/*
 Frequency Counter Arduino Sketch
 by: Jim Lindblom
 SparkFun Electronics
 License: Beerware
 
 This isn't the most accurate frequency counter, but it's simple to
 program and understand, and it works in a pinch. I've tested it to
 be accurate to at least 96% (usually around 99.5%). It should be 
 able to measure frequencies up to about 6.5MHz(8MHz optimally)
 
 The circuit: Powered at 5V (5V LCD), Arduino running at 16MHz
 D2 - RS (LCD)
 D3 - R/W (LCD)
 D4 - E (LCD)
 D5 - Frequency input
 D6 - DB4 (LCD)
 D7 - DB5 (LCD)
 D8 - DB6 (LCD)
 D9 - DB7 (LCD)
 D10 - Gate of NPN transistor (Collector tied to 5V, emitter tied to LCD backlight pin)

For the LiquidCrystal library, much thanks to:
   David A. Mellis
   Limor Fried (http://www.ladyada.net)
   Tom Igoe
*/

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 6, 7, 8, 9);
int brightness;
unsigned int tovf1 = 0;
unsigned long frequency = 0;
unsigned long temp = 0;

// Timer 1 is our counter
// 16-bit counter overflows after 65536 counts
// tovfl will keep track of how many times we overflow
ISR(TIMER1_OVF_vect)
{
  tovf1++;
}

void setup() {
  pinMode(5, INPUT);  // This is the frequency input
  pinMode(10, OUTPUT);  // Backlight control pin
  digitalWrite(10, HIGH);  // Turn backlight on

  // Timer 1 will be setup as a counter
  // Maximum frequency is Fclk_io/2
  // (recommended to be < Fclk_io/2.5)
  // Fclk_io is 16MHz
  TCCR1A = 0;
  // External clock source on D5, trigger on rising edge:
  TCCR1B = (1<<CS12) | (1<<CS11) | (1<<CS10);	
  // Enable overflow interrupt
  // Will jump into ISR(TIMER1_OVF_vect) when overflowed:
  TIMSK1 = (1<<TOIE1);
  
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  
  // Print a splash screen to the LCD.
  lcd.print("FrequencyCounter");
  lcd.setCursor(0, 1);
  lcd.print("      v1.0      ");
  delay(2000);
}

void loop() {
  // Delay a second. While we're delaying Counter 1 is still
  // reading the input on D5, and also keeping track of how
  // many times it's overflowed
  delay(1000);  
  
  lcd.clear();
  
  frequency = (TCNT1H<<8)|TCNT1L;
  frequency = (TCNT1H<<8)|TCNT1L;
  
  // Correct weird counter bug
  // A small range of frequencies (~30k-50k) are getting
  // 42949 appended to the front of them
  // Will look into this but this works for now
  if (frequency > 40000000)
    frequency -= 4294900000;
  
  // 65536 (2^16) is maximum of counter
  // We'll multiply that by how many times we overflowed
  temp = 65536*(unsigned long)tovf1;
  
  // Add the overflow value to frequency
  frequency += temp;
  
  // Print the proper amount of spaces to make it look pretty
  lcd.setCursor(0,1);
  if (frequency < 100)
    lcd.print("      ");
  else if (frequency < 10000)
    lcd.print("     ");
  else if (frequency < 1000000)
    lcd.print("    ");
  else
    lcd.print("   ");
  lcd.print(frequency);
  lcd.print(" Hz");

  // Reset all counter variables and start over
  TCNT1H = 0;
  TCNT1L = 0;
  tovf1 = 0;
}

