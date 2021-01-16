#include <xc.h>
#include <stdint.h>
#include "macros.h"

void TX_char(uint8_t data) {
    TX1REG = data;
    while(checkBit(PIR3, 4) == 0);
}

void TX_word(uint8_t *word){
    while(*word != '\0') {
        TX_char(*word);
        ++word;
    }
    TX_char(',');       //blank the colon and semicolon
    TX_char(';');
    TX_char('\0');
}

/*
    Baudrate is in bps and OscFreq is in Hertz
*/
void TX_init(uint32_t baudRate, uint32_t OscFreq) {      
    SP1BRGL = (OscFreq / (4 * baudRate)) - 1;     
    SP1BRGH = 0;
    RC6PPS = 0x0D;
    setBit(TX1STA, 2);      // BRG HIGH SPEED
    setBit(BAUD1CON, 3);    // 16 bit mode
    clearBit(TX1STA, 4);    //asynchronous mode
    setBit(RC1STA, 7);      //enable UART
    setBit(TRISC, 6);
    clearBit(ANSELC, 6);    // set as digital I/O
    setBit(TX1STA, 5);  // TX enable
    clearBit(PIR3, 4);
}

