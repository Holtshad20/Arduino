//---------------------------------------------------------------------------

#include <vcl.h>
 #include <vcl\registry.hpp> //É necessário incluir esse arquivo para disponibilizar TRegistry.
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TForm2 *Form2;

//Declarações das variáveis globais

HANDLE Recebe;
DCB dcb;
COMMTIMEOUTS CommTimeouts;
bool LerDadosSerial=0;
//Definindo funções que serão usadas

bool AbrirPorta(char *NomePorta);
bool ConfiguraControle(void);
bool ConfiguraTimeOuts(void);
bool EscreveDados(char* outputData,const unsigned int sizeBuffer,unsigned long& length);
DWORD WINAPI LeDados(LPVOID arg);

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
     TRegistry *Registro = new TRegistry; //Cria e aloca espaço na memória para o objeto.
     TStringList *Lista = new TStringList; //Cria e aloca espaço na memória para o objeto.
     Registro->RootKey = HKEY_LOCAL_MACHINE; //Define chave raiz.
     Registro->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false); //Abre a chave.
     //Obtém uma string contendo todos os nomes de valores associados com a chave atual.
     Registro->GetValueNames(Lista);

     //Count é a quantidade de portas instaladas no sistema.
     for(int indice=0; indice <= Lista->Count-1; indice++)
         //Pega nos nomes das portas e ediciona-as no ComboBox1.
         Form1->ComboBoxPorta->Items->Add(Registro->ReadString( Lista->Strings[indice] ) );

     if( Form1->ComboBoxPorta->Items->Count > 0)  //Se há ítens.
         Form1->ComboBoxPorta->ItemIndex = 0; //Exibir o primeiro nome da porta no ComboBox.
     Lista->Free();    //Libera memória.
     Registro->CloseKey();    //Fecha a chave.
     Registro->Free();    //Libera memória.
     BotaoConectar->Enabled=true;
     BotaoDesconectar->Enabled=false;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::BotaoConectarClick(TObject *Sender)
{
        if(AbrirPorta(ComboBoxPorta->Text.c_str())==false)
        {
                Application->MessageBox("Falha ao Abrir Porta","Erro", MB_OK);
        }
        if(ConfiguraControle()==false)
        {
                Application->MessageBox("Falha ao configurar Porta","Erro", MB_OK);
        }
        if(ConfiguraTimeOuts()==false)
        {
                Application->MessageBox("Falha ao configurar Time Out","Erro", MB_OK);
        }
        LerDadosSerial=1;
        Recebe=CreateThread(0,0,LeDados,(LPVOID) 0,0,0);
        BotaoConectar->Enabled=false;
        BotaoDesconectar->Enabled=true;
        Timer1->Interval=200;
        ComboBoxPorta->Enabled=false;

}

//---------------------------------------------------------------------------


//Abre a Porta Serial COMx
bool AbrirPorta(char *NomePorta)
{
   Form2->hCom = CreateFile(
             NomePorta,
             GENERIC_READ | GENERIC_WRITE,
             0, // dispositivos comm abertos com acesso exclusivo
             NULL, // sem atributos de segurança
             OPEN_EXISTING, // deve usar OPEN_EXISTING
             0, //Entrada e saída sem ovelap.
             NULL // hTemplate deve ser NULL para comm
          );
   if(Form2->hCom == INVALID_HANDLE_VALUE)
   {
      return false;
   }
   return true;
}

//------------------------------------------------------------------------------

//CONFIGURA PORTA SERIAL.
bool ConfiguraControle(void)
{
   if(!GetCommState(Form2->hCom, &dcb))
   {
      return false;
   }
   dcb.BaudRate = CBR_9600; 
   dcb.ByteSize = 8;
   dcb.Parity = NOPARITY;
   dcb.StopBits = ONESTOPBIT;

   if( SetCommState(Form2->hCom, &dcb) == 0 )
   {
      return false;
   }
   return true;
}

//------------------------------------------------------------------------------

//DEFINE TIMEOUTs
bool ConfiguraTimeOuts(void)
{
   if( GetCommTimeouts(Form2->hCom, &CommTimeouts) == 0 )
   {
      return false;
   }

   CommTimeouts.ReadIntervalTimeout = 5;
   CommTimeouts.ReadTotalTimeoutMultiplier = 1;
   CommTimeouts.ReadTotalTimeoutConstant = 5;
   CommTimeouts.WriteTotalTimeoutMultiplier = 5;
   CommTimeouts.WriteTotalTimeoutConstant = 5;

   if( SetCommTimeouts(Form2->hCom, &CommTimeouts) == 0 )
   {
      return false;
   }
   return true;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::BotaoDesconectarClick(TObject *Sender)
{
LerDadosSerial=0;
CloseHandle( Form2->hCom );  //Fecha a porta aberta anteriormente por CreateFile().
CloseHandle( Recebe );
BotaoConectar->Enabled=true;
BotaoDesconectar->Enabled=false;
ComboBoxPorta->Enabled=true;
Timer1->Interval=0;
Form2->Edit4->Text="Erro";
Form2->Edit5->Text="Erro";
Form2->Edit6->Text="Erro";
}
//---------------------------------------------------------------------------

DWORD WINAPI LeDados(LPVOID arg)
{
    String StrComandos;             //Armazena a string de comando lida da Serial.
    char BufferRecebe[1];  //Buffer temporário para trabalhar direto com ReadFile().
    DWORD BytesLidos;     //Para armazenar a quantidade de dados lidos.
    while(LerDadosSerial==1)
    {
        ReadFile( Form2->hCom, BufferRecebe, 1, &BytesLidos, NULL );
            if(BytesLidos > 0) //Se algum caracter foi lido.
            {
              BufferRecebe[1] = '\0';  //Finaliza string.
              StrComandos += BufferRecebe;
              if(BufferRecebe[0]=='\n')
              {
                StrComandos.Delete(StrComandos.Length()-1,StrComandos.Length());
                ///Tratar Os Dados
                if(StrComandos[1]=='S' && StrComandos[2]=='P')
                {
                        StrComandos.Delete(1,2);
                        float Aux=atof(StrComandos.c_str());
                        //Aux=0.0855327*Aux+12.5;
                        Form1->Edit1->Text=Aux;
                }
                if(StrComandos[1]=='P' && StrComandos[2]=='V')
                {
                        StrComandos.Delete(1,2);
                        float Aux=atof(StrComandos.c_str());
                        //Aux=0.0855327*Aux+12.5;
                        Form1->Edit2->Text=Aux;
                }
                if(StrComandos[1]=='M' && StrComandos[2]=='V')
                {
                        StrComandos.Delete(1,2);
                        float Aux=atof(StrComandos.c_str());
                        Aux=4*Aux;
                        Form1->Edit3->Text=Aux;
                }
                if(StrComandos[1]=='K' && StrComandos[2]=='D')
                {
                        StrComandos.Delete(1,2);
                        Form2->Edit6->Text=StrComandos;
                }
                if(StrComandos[1]=='K' && StrComandos[2]=='I')
                {
                        StrComandos.Delete(1,2);
                        Form2->Edit5->Text=StrComandos;
                }
                if(StrComandos[1]=='K' && StrComandos[2]=='P')
                {
                        StrComandos.Delete(1,2);
                        Form2->Edit4->Text=StrComandos;
                }
                StrComandos="";
              }

            }
      }

ExitThread(0);
return 0;
}



void __fastcall TForm1::GanhosdoControlador1Click(TObject *Sender)
{
        Form2->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
        char BufferEnvia[30];
        float aux=atof(Edit4->Text.c_str());
        //aux=((11.6914*aux)-(146.142857));
        sprintf(BufferEnvia,"SP%f\r\n",aux);
        DWORD BytesEscritos = 0;
        if(WriteFile(Form2->hCom, BufferEnvia, strlen(BufferEnvia), &BytesEscritos,NULL) == 0)
        {
                Application->MessageBox("Erro ao Enviar SetPoint","Erro", MB_OK);
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
        static float TempodoGrafico=0;
        Series1->AddXY(TempodoGrafico,atof(Form1->Edit1->Text.c_str()),"",clRed);// Plota a linha no gráfico do SP
        Series2->AddXY(TempodoGrafico,atof(Form1->Edit2->Text.c_str()),"",clGreen);//Plota a linha da variável PV
        Series3->AddXY(TempodoGrafico,atof(Form1->Edit3->Text.c_str()),"",clBlue);//Plota a linha da variável PV
        Chart1->BottomAxis->Increment=1;// VER SE É NECESSÁRIO ??????
        Chart1->BottomAxis->Scroll(1,true);
        TempodoGrafico+=0.2;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
        char BufferEnvia[30];
        if(RadioButton1->Checked==true){ sprintf(BufferEnvia,"T1\r\n"); }
        if(RadioButton2->Checked==true){ sprintf(BufferEnvia,"T2\r\n"); }
        if(RadioButton3->Checked==true){ sprintf(BufferEnvia,"T3\r\n"); }
        DWORD BytesEscritos = 0;
        if(WriteFile(Form2->hCom, BufferEnvia, strlen(BufferEnvia), &BytesEscritos,NULL) == 0)
        {
                Application->MessageBox("Erro ao Enviar SetPoint","Erro", MB_OK);
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{
     TRegistry *Registro = new TRegistry; //Cria e aloca espaço na memória para o objeto.
     TStringList *Lista = new TStringList; //Cria e aloca espaço na memória para o objeto.
     Registro->RootKey = HKEY_LOCAL_MACHINE; //Define chave raiz.
     Registro->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false); //Abre a chave.
     //Obtém uma string contendo todos os nomes de valores associados com a chave atual.
     Registro->GetValueNames(Lista);

     //Count é a quantidade de portas instaladas no sistema.
     for(int indice=0; indice <= Lista->Count-1; indice++)
         //Pega nos nomes das portas e ediciona-as no ComboBox1.
         Form1->ComboBoxPorta->Items->Add(Registro->ReadString( Lista->Strings[indice] ) );

     if( Form1->ComboBoxPorta->Items->Count > 0)  //Se há ítens.
         Form1->ComboBoxPorta->ItemIndex = 0; //Exibir o primeiro nome da porta no ComboBox.
     Lista->Free();    //Libera memória.
     Registro->CloseKey();    //Fecha a chave.
     Registro->Free();    //Libera memória.
}
//---------------------------------------------------------------------------


