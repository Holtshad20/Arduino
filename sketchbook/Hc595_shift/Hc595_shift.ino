#include <avr/io.h>
#include <util/delay.h>

#define DELAYTIME 200

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

#define number_of_74hc595s 1 //How many of the shift registers are there daisey chained?

#define HC595_PORT PORTB // Porta conectada ao 74HC595
#define HC595_DATA DDRB  // Porta de dados para o 74HC595
#define ST_CP PB0        // Storage Register Clock (latch)
#define SH_CP PB1        // Shift Register Clock   (clock)  
#define DS    PB2        // Serial Data Input      (data)

void hc595Init() {
    // define os bits da porta como saida
    setBit(HC595_DATA, ST_CP);
    setBit(HC595_DATA, SH_CP);
    setBit(HC595_DATA, DS);
    // inicializa o lcd
    shiftOut(0x00);
    _delay_ms(10);
}


void setup(){
    hc595Init();
}


void loop(){
    char counter = 0;
    while(1){
        shiftOut(0xAA); // envia 10101010 
	_delay_ms(DELAYTIME);				
        shiftOut(0x55); // envia 01010101 
	_delay_ms(DELAYTIME);
    }
}


void shiftOut(unsigned long data) {

  clearBit(HC595_PORT, ST_CP); // latch = low - preparar para receber dados
  
  for (int i = 0; i < (8 * number_of_74hc595s); i++){
    clearBit(HC595_PORT, SH_CP);
    clearBit(HC595_PORT, ((data&(0x01<<i))>>i) << DS );
    setBit(HC595_PORT, SH_CP);
    setBit(HC595_PORT, ((data&(0x01<<i))>>i) << DS );
  }
  setBit(HC595_PORT, ST_CP); // latch = high - enviar dados
}



