int UP = 7;
int DOWN = 8;
int LAMP = 3;   
int dimming = 128;
int counts = 7;
int dimmer[7] = { 1, 105, 100, 75, 50, 25, 10 };
int i = 0;

void setup()
{
  //Serial.begin(9600);
  //delay(1000);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LAMP, OUTPUT);
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, HIGH);
  attachInterrupt(0, zero_crosss_int, RISING);
}

void loop()
{
  if (!digitalRead(UP) || !digitalRead(DOWN))
  {
    if (verifica_botao(UP))
    {
      if (i < (counts - 1)) i++;
      //Serial.print("UP: ");
      //Serial.println(i, DEC);
    }
    else if (verifica_botao(DOWN))
    {
      if (i > 0) i--;
      //Serial.print("DOWN: ");
      //Serial.println(i, DEC);
    }
  }
  //Serial.print("VALOR: ");
  //  Serial.println(i, DEC);
  dimming = dimmer[i];
  delay(100);
}

void zero_crosss_int()
{
  int dimtime = (65 * dimming);     
  delayMicroseconds(dimtime);   
  digitalWrite(LAMP, HIGH);  
  delayMicroseconds(8.33);        
  digitalWrite(LAMP, LOW);   
}

byte verifica_botao(byte botao)
{
  if (!digitalRead(botao))
  {
    unsigned long tempo = millis() + (150 - dimming);
    while(tempo > millis())
    {
     if (digitalRead(botao)) return LOW;
    }
    return HIGH;
  }
  else
    return LOW;
}
