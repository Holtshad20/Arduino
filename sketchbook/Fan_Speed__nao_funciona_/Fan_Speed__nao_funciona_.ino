/***************************************/
// FAN SPEED Arduino
// Benoît ROUSSEAU juillet 2006
// - Mesure de la vitesse de rotation d'un
// ventillateur de CPU sous interruption.
/***************************************/

// variables et définitions
#define INT_0_PIN  2
#define EXTINT_LEVEL_LOW	0x00	// trigger on low level
#define EXTINT_EDGE_ANY		0x01	// trigger on any edge
#define EXTINT_EDGE_FALLING	0x02	// trigger on falling edge
#define EXTINT_EDGE_RISING	0x03	// trigger on rising edge
#define sbi(PORT,BIT) PORT|=_BV(BIT)   // macro pratique pour mettre un bit x à 1 (Set Bit In)

unsigned int NbTopsFan;
unsigned int MeasuredTopsFan;

/***************************************/
// Fonction appelée à chaque
// déclenchement de l'interruption 0
/***************************************/
SIGNAL(SIG_INTERRUPT0)
{
  NbTopsFan++;
}

/***************************************/
// iniInt0
/***************************************/
// Init. interrupt 0
/****************************************/
void initInt0 (byte config)
{
  sbi(GICR, INT0); //
  sbi(GIFR, INT0); //

  MCUCR &= ~((1<<ISC01) | (1<<ISC00)); // raz
  MCUCR |= config; //
}

/***************************************/
void setup()
{
  pinMode(INT_0_PIN, INPUT);
  Serial.begin(9600);
  initInt0 (EXTINT_EDGE_RISING);
};

void loop ()
{
    NbTopsFan = 0;
    delay (1000);
    MeasuredTopsFan = NbTopsFan;
    Serial.print (MeasuredTopsFan * 60, DEC);
    Serial.print (" rpm");
    Serial.print (13, BYTE);
};
 
