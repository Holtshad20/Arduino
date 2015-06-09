// Tutorial de Menu LCD por diagrama de estados
// Maurício Féo Rivello
// engenheirando.com
// Assista o vídeo sobre:
// http://www.youtube.com/watch?v=mcuReWQt_oo
 
#include <LiquidCrystal595.h>
 
// Definição dos pinos dos botões, luz do display e buzzer
#define Buzzer   6
#define Display  7
#define bMenu    A0  // 
#define bChange  A1  // Executar açao no menu 
#define bUp      A2  // Voltar menu
#define bDown    A3  // Avançar menu
 
#define bMenu0   90  // Valor de referência que a
#define bChange0 91  // função CheckButton() passa
#define bUp0     92  // indicando que um botão foi
#define bDown0   93  // solto
 
boolean aMenu, aChange, aUp, aDown;  // Grava o ultimo valor lidos nos botões.
// Utilizado pela função Checkbutton p/ identificar quando há uma alteração no estado do pino dos botões
 
int variavel;  // variavel a ser alterada pelo menu
char state=1;  // variável que guarda posição atual do menu
LiquidCrystal595 lcd(2,3,4);  // Declaração do objeto tipo lcd
 
//============================================== SETUP
void setup()
{
  lcd.begin(16, 2);  // Iniciando a biblioteca do LCD
 
  pinMode(Display,OUTPUT);  // Luz de fundo do display
  pinMode(Buzzer, OUTPUT);  // Buzzer
  pinMode(bMenu,  INPUT);   // Botões
  pinMode(bChange,INPUT);
  pinMode(bUp,    INPUT);
  pinMode(bDown,  INPUT);
 
  digitalWrite(bMenu,  HIGH);  // Aciona o pull-up interno
  digitalWrite(bChange,HIGH);  // dos botões
  digitalWrite(bUp,    HIGH);
  digitalWrite(bDown,  HIGH);
  digitalWrite(Display,HIGH);  // Liga a luz do display.
}
 
//==============================================
//============================================== LOOP
void loop()
{
  switch (state) {  // Define checa qual tela atual
 
  case 1:          // executado quando na TELA 1
     switch (CheckButton()) {
       case bChange:
            Feo();
            break;
       case bUp:
            lcd.clear(); Set_state(4); // antes de mudar de tela, é necessário limpar o
            break;                     // display com a função lcd.clear()
       case bDown:
            lcd.clear(); Set_state(2);
            break;
     default:             // Caso nenhum botão tenha sido apertado, ela executa a set_state
            Set_state(1); // mesmo assim para atualizar o display.
  }
  break;
 
  case 2:          // executado quando na TELA 2
     switch (CheckButton()) {
       case bChange:  // Apita o buzzer quando o botão bChange é pressinado na tela 2
            digitalWrite(Buzzer,HIGH);
            delay(200);
            digitalWrite(Buzzer,LOW);
            break;
       case bUp:
            lcd.clear(); Set_state(1);
            break;
       case bDown:
            lcd.clear(); Set_state(3);
            break;
     default:
            Set_state(2);
  }
  break;
 
  case 3:          // executado quando na TELA 3
     switch (CheckButton()) {
       case bChange:
            digitalWrite(Display,LOW);
            delay(1000);
            digitalWrite(Display,HIGH);
            break;
       case bUp:
            lcd.clear(); Set_state(2);
            break;
        case bDown:
             lcd.clear(); Set_state(4);
             break;
     default:
            Set_state(3);
  }
  break;
 
  case 4:          // executado quando na TELA 4
     switch (CheckButton()) {
       case bMenu:
            lcd.clear(); variavel++;
            break;
       case bChange:
            lcd.clear(); variavel--;
            break;
       case bUp:
            lcd.clear(); Set_state(3);
            break;
       case bDown:
            lcd.clear(); Set_state(1);
            break;
    default:
           Set_state(4);
  }
  break;
 
  default: ;
  }
}
//============================================== FIM da função LOOP
//==============================================
 
//============================================== CheckButton
char CheckButton() {
  if (aMenu!=digitalRead(bMenu)) {
      aMenu=!aMenu;
  if (aMenu) return bMenu0; else return bMenu;
  } else
  if (aChange!=digitalRead(bChange)) {
      aChange=!aChange;
  if (aChange) return bChange0; else return bChange;
  } else
  if (aUp!=digitalRead(bUp)) {
      aUp=!aUp;
  if (aUp) return bUp0; else return bUp;
  } else
  if (aDown!=digitalRead(bDown)) {
      aDown=!aDown;
  if (aDown) return bDown0; else return bDown;
  } else
  return 0;
}
 
//========================================================
//============================================== Set_state
void Set_state(char index) {
  state = index;  // Atualiza a variável state para a nova tela
  switch (state) {  // verifica qual a tela atual e exibe o conteúdo correspondente
    case 1: //==================== state 1
         lcd.setCursor(0,0);
         lcd.print("Segundos:");
         lcd.setCursor(0,1);
         lcd.print( millis()/1000 , DEC);  // mostra os segundos na tela
         lcd.print(" s");
         break;
    case 2: //==================== state 2
         lcd.setCursor(0,0);
         lcd.print("Beep!");
         break;
    case 3: //==================== state 3
         lcd.setCursor(0,0);
         lcd.print("  Aperte para");
         lcd.setCursor(0,1);
         lcd.print("    apagar");
         break;
    case 4: //==================== state 4
         lcd.setCursor(0,0);
         lcd.print("Variavel:");
         lcd.setCursor(0,1);
         lcd.print(variavel, DEC);  // mostra o valor de "variavel"
         break;
    default: ;
  }
}
 
// Função que mostra o nome "Féo" passando na tela.
// Apenas uma assinatura.
//=======================================================  Display Feo
void Feo() {
const byte a[2][15][8] = {
{{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00001,0b00001,0b00001,0b00011,0b00011},
{0b00000,0b11111,0b11111,0b11111,0b10000,0b10000,0b10000,0b11111},
{0b00000,0b11111,0b11110,0b11100,0b00000,0b00000,0b00000,0b11000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00001,0b00011,0b00110},
{0b00000,0b00110,0b01100,0b11000,0b00000,0b11100,0b00110,0b00010},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00001,0b00011,0b00110},
{0b00000,0b00000,0b00000,0b00000,0b11000,0b11100,0b00110,0b00011},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}},
{{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00111,0b00111,0b01110,0b01110,0b01100,0b11100,0b11100,0b11000},
{0b11111,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b10000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b01100,0b11111,0b11000,0b11000,0b01100,0b01100,0b00111,0b00001},
{0b00011,0b11100,0b00000,0b00000,0b00010,0b00110,0b11100,0b10000},
{0b01100,0b11000,0b11000,0b11000,0b01100,0b01100,0b00111,0b00011},
{0b00001,0b00001,0b00001,0b00011,0b00110,0b01100,0b11000,0b10000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000},
{0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000}}
};
byte b[2][15][8];
int k,i,j;

while (!digitalRead(bChange)) {
  lcd.setCursor(12,0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.setCursor(12,1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);
  lcd.write(8);
  for (k=0; k<2; k++)
  for (i=0; i<15; i++)
  for (j=0; j<8; j++)
  b[k][i][j]=a[k][i][j];
  for (j=0; j<55; j++) {
  lcd.createChar(1,  b[0][0]);
  lcd.createChar(2,  b[0][1]);
  lcd.createChar(3,  b[0][2]);
  lcd.createChar(4,  b[0][3]);
  lcd.createChar(5,  b[1][0]);
  lcd.createChar(6,  b[1][1]);
  lcd.createChar(7,  b[1][2]);
  lcd.createChar(8,  b[1][3]);
  for (i=0; i<15; i++) {
  for (k=0; k<8; k++) {
  b[0][i][k]=(b[0][i][k]<<1) + ((b[0][i+1][k]>>4)&(1));
 }
}
for (i=0; i<15; i++) {
for (k=0; k<8; k++) {
b[1][i][k]=(b[1][i][k]<<1) + ((b[1][i+1][k]>>4)&(1));
}
}
delay(15);
}
}
lcd.clear();
}
