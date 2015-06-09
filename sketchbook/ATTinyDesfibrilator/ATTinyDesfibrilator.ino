    #include <TimerOne.h>  
      
    // Protótipo de programador serial de alta tensão para ATtiny  
    // Adaptado do código de Wayne Holder:  
    // https://sites.google.com/site/wayneholder/attiny-fuse-reset-with-12-volt-charge-pump  
      
    #define LED      13  
      
    // Conexões para programação  
    #define  SCI     12    // Target Clock Input  
    #define  SDO     11    // Target Data Output  
    #define  SII     10    // Target Instruction Input  
    #define  SDI      9    // Target Data Input  
    #define  VCC      8    // Target VCC  
      
    // Comandos de leitura e escrita dos FUSES e LOCK  
    #define  HFUSE_RD  0x7A7E  
    #define  LFUSE_RD  0x686C  
    #define  EFUSE_RD  0x6A6E  
    #define  LOCK_RD   0x787C  
    #define  HFUSE_WR  0x747C  
    #define  LFUSE_WR  0x646C  
    #define  EFUSE_WR  0x666E  
      
    // Define ATTiny series signatures  
    #define  ATTINY13   0x9007  // L: 0x6A, H: 0xFF             8 pin  
    #define  ATTINY24   0x910B  // L: 0x62, H: 0xDF, E: 0xFF   14 pin  
    #define  ATTINY25   0x9108  // L: 0x62, H: 0xDF, E: 0xFF    8 pin  
    #define  ATTINY44   0x9207  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin  
    #define  ATTINY45   0x9206  // L: 0x62, H: 0xDF, E: 0xFF    8 pin  
    #define  ATTINY84   0x930C  // L: 0x62, H: 0xDF, E: 0xFFF  14 pin  
    #define  ATTINY85   0x930B  // L: 0x62, H: 0xDF, E: 0xFF    8 pin  
      
    // Definições para acesso direto aos pinos do Charge Pump  
    #define P1  0x04  // Pin D2  
    #define P2  0x08  // Pin D3  
    #define PWR 0x10  // Pin D4  
    #define GND 0x20  // Pin D5  
    #define REF 420   // valor do ADC para 12V  
      
    // Variaveis para controle do Charge Pump  
    volatile char phase = 0;  
    volatile char onOff = 0;  
    volatile char pwrOn = 0;  
      
    // Rotina de controle do Charge Pump  
    // Disparada pelo Timer 1  
    void ticker () {  
      if (onOff) {  
        DDRD = P1 | P2 | PWR | GND;  
        int volts = analogRead(A0);  
        if (volts < REF) {  
          if (phase) {  
            PORTD = P1 | PWR;  
          } else {  
            PORTD = P2 | PWR;  
          }  
          phase ^= 1;  
        } else {  
          pwrOn = 1;  
        }  
      } else {  
        pwrOn = 0;  
        DDRD = GND;  
        PORTD = GND;  
      }  
    }  
      
    // Iniciação  
    void setup() {  
      pinMode(LED, OUTPUT);  
        
      // Inicia os pinos de programação  
      pinMode(VCC, OUTPUT);  
      pinMode(SDI, OUTPUT);  
      pinMode(SII, OUTPUT);  
      pinMode(SCI, OUTPUT);  
      pinMode(SDO, OUTPUT);  
        
      // Inicia serial  
      Serial.begin(57600);  
      Serial.println("HVSP Prototipo");  
      Serial.println("I - Le identificacao");  
      Serial.println("F - Le os fuses");  
      Serial.println("L - Le o lock");  
      Serial.println("C - Chip Erase");  
      Serial.println("R - Reinicia fuses");  
      Serial.println("T - Trava");  
        
      // Inicia ADC e Timer1 para o Charge Pump  
      analogReference(DEFAULT);  
      Timer1.initialize(500);  
      Timer1.attachInterrupt(ticker);  
      
      Serial.println("Pronto");  
    }  
      
    // Programa principal  
    void loop() {  
      int c;  
      byte id[3];  
      unsigned int sig;  
        
      if (Serial.available() > 0) {  
        c = Serial.read();  
          
        // Entra no modo programação  
        digitalWrite(LED, HIGH);  
        pinMode(SDO, OUTPUT);  
        digitalWrite(SDI, LOW);  
        digitalWrite(SII, LOW);  
        digitalWrite(SDO, LOW);  
        onOff = 0;                // 12v Off  
        digitalWrite(VCC, HIGH);  // Vcc On  
        delayMicroseconds(20);  
        onOff = 1;                // 12v On  
        while (pwrOn == 0)  
          ;  
        delayMicroseconds(10);  
        pinMode(SDO, INPUT);      // Set SDO to input  
        delayMicroseconds(300);  
          
        // Trata o comando  
        switch (c) {  
          case 'I':  case 'i':  
            readSignature(id);  
            Serial.print("Identificacao: ");  
            Serial.print(id[0], HEX);  
            Serial.print(' ');  
            Serial.print(id[1], HEX);  
            Serial.print(' ');  
            Serial.println(id[2], HEX);  
            break;  
          case 'F':  case 'f':  
            Serial.print("LFUSE: ");  
            Serial.println(readFuse(LFUSE_RD), HEX);  
            Serial.print("HFUSE: ");  
            Serial.println(readFuse(HFUSE_RD), HEX);  
            Serial.print("EFUSE: ");  
            Serial.println(readFuse(EFUSE_RD), HEX);  
            break;  
          case 'L':  case 'l':  
            Serial.print("LOCK: ");  
            Serial.println(readFuse(LOCK_RD) & 3, HEX);  
            break;  
          case 'C':  case 'c':  
            Serial.println("Apagando...");  
            chipErase();  
            break;  
          case 'R':  case 'r':  
            Serial.println("Reiniciando fuses...");  
            sig = readSignature(id);  
            if (sig == ATTINY13) {  
              writeFuse(LFUSE_WR, 0x6A);  
              writeFuse(HFUSE_WR, 0xFF);  
            } else if (sig == ATTINY24 || sig == ATTINY44 || sig == ATTINY84 ||  
                       sig == ATTINY25 || sig == ATTINY45 || sig == ATTINY85) {  
              writeFuse(LFUSE_WR, 0x62);  
              writeFuse(HFUSE_WR, 0xDF);  
              writeFuse(EFUSE_WR, 0xFF);  
            }  
            break;  
          case 'T':  case 't':  
            Serial.println("Travando...");  
            writeLock (0);  
            break;  
        }  
      
        // espera concluir operação  
        while (!digitalRead(SDO))  
          ;  
            
        // sai do modo programação      
        digitalWrite(SCI, LOW);  
        digitalWrite(VCC, LOW);    // desliga Vcc  
        onOff = 0;                 // desliga 12V  
        digitalWrite(LED, LOW);  
        Serial.println("Pronto");  
      }  
    }  
      
    // Envia instrução e dado e lê resposta  
    byte shiftOut (byte dado, byte instr) {  
      int inBits = 0;  
        
      // Espera final da operação anterior  
      while (!digitalRead(SDO))  
        ;  
      unsigned int dout = (unsigned int) dado << 2;  
      unsigned int iout = (unsigned int) instr << 2;  
      for (int ii = 10; ii >= 0; ii--)  {  
        digitalWrite(SDI, !!(dout & (1 << ii)));  
        digitalWrite(SII, !!(iout & (1 << ii)));  
        inBits <<= 1;  
        inBits |= digitalRead(SDO);  
        digitalWrite(SCI, HIGH);  
        digitalWrite(SCI, LOW);  
      }  
      return inBits >> 2;  
    }  
      
    // Le um fuse (ou lock)  
    byte readFuse (unsigned int fuse)  
    {  
      shiftOut(0x04, 0x4C);  
      shiftOut(0x00, (byte) (fuse >> 8));  
      return shiftOut(0x00, (byte) fuse);  
    }  
      
    // Escreve em um fuse  
    void writeFuse (unsigned int fuse, byte val) {  
      shiftOut(0x40, 0x4C);  
      shiftOut( val, 0x2C);  
      shiftOut(0x00, (byte) (fuse >> 8));  
      shiftOut(0x00, (byte) fuse);  
    }  
      
    // Le a identificação do chip  
    unsigned int readSignature (byte *sig) {  
      shiftOut(0x08, 0x4C);  
      for (int ii = 0; ii < 3; ii++) {  
        shiftOut( ii, 0x0C);  
        shiftOut(0x00, 0x68);  
        sig[ii] = shiftOut(0x00, 0x6C);  
      }  
      return (sig[1] << 8) | sig[2];  
    }  
      
    // Faz apagamento completo do chip  
    void chipErase() {  
      shiftOut(0x80, 0x4C);  
      shiftOut(0x00, 0x64);  
      shiftOut(0x00, 0x6C);  
    }  
      
    // Escreve no lock  
    byte writeLock(byte val) {  
      shiftOut(0x20, 0x4C);  
      shiftOut(val, 0x2C);  
      shiftOut(0x00, 0x64);  
      shiftOut(0x00, 0x6C);  
    }  
