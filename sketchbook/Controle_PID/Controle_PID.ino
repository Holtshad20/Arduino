/*
   Controle PIP
   Hugo Bernard Armond Vieira
   Email: hugubav@hotmail.com
   Projeto inacabado...
*/

int Atuacao;
float PV;
float KI=0.0003; 
float KP=10; 
float KD=0.0;
float SetPoint=300;
String inputString;
#define BufferSerial 10

void setup()  { 
  Serial.begin(9600);
  //analogReference(EXTERNAL);
} 

void loop() 
{ 
        PV=analogRead(A0);
        Atuacao=ControladorPID(KI, KP, KD, SetPoint, PV, 0.01, 0, 255);   
        analogWrite(12, Atuacao);  
        Serial.print("KP");
        Serial.println(KP);
        Serial.print("KI"); 
        Serial.println(KI);
        Serial.print("KD"); 
        Serial.println(KD); 
        Serial.print("SP");
        Serial.println(SetPoint);
        Serial.print("MV");
        Serial.println(Atuacao);
        Serial.print("PV");
        Serial.println(PV);
        EventoSerial();
        delay(100);
}

float ControladorPID(float KI, float KP, float KD, float SetPoint, float PV, float T, float Min, float Max)
{
       static float Io=0, Eo=0;
       float Erro, MV;
       float Proporcional, Integral, Derivativo;
       Erro=PV-SetPoint;
       Proporcional=KP*Erro;
       Integral=Io+KI*Erro*T;
       Derivativo=KD*(Erro-Eo)/T;
       MV=Integral+Proporcional+Derivativo+Min;
       if(MV>Max){ Integral=Io; MV=Max; }
       if(MV<Min){ Integral=Io; MV=Min; }
       Io=Integral;
       Eo=Erro;
       return MV;
}


void EventoSerial()
{
    while (Serial.available()) 
    {
        char inChar = (char) Serial.read(); 
        inputString += inChar;
        if (inChar == '\n' || inChar == '\r')
        {
            if(inputString[0]=='S' && inputString[1]=='P')
            {
              char Valor[BufferSerial]; 
              unsigned int i=2;
              while(inputString[i]!='\r')
              {  
                  Valor[i-2]=inputString[i];
                  i++; 
              }
              Valor[i]='0';
              SetPoint=atof(Valor);
           }
            if(inputString[0]=='K' && inputString[1]=='P')
            {
              char Valor[BufferSerial]; 
              unsigned int i=2;
              while(inputString[i]!='\r')
              {  
                  Valor[i-2]=inputString[i];
                  i++; 
              }
                KP=atof(Valor);
            }
            if(inputString[0]=='K' && inputString[1]=='I')
            {
              char Valor[BufferSerial]; 
              unsigned int i=2;
              while(inputString[i]!='\r')
              {  
                  Valor[i-2]=inputString[i];
                  i++; 
              }
              Valor[i]='0';
              KI=atof(Valor);
            }
            if(inputString[0]=='K' && inputString[1]=='D')
            {
              char Valor[BufferSerial]; 
              unsigned int i=2;
              while(inputString[i]!='\r')
              {  
                  Valor[i-2]=inputString[i];
                  i++; 
              }
                Valor[i]='0';
                KD=atof(Valor);
             }
            inputString="";
        } 
    }
}

