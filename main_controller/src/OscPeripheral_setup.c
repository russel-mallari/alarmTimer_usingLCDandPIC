#include <xc.h>
#include <stdint.h>
#include "macros.h"

void HFIntOsc_init(void) {
    while(checkBit(OSCSTAT, 6) == 0)
        ;   // check if HFINTOSC is ready
    writeBitField(OSCFRQ, 0x07, 0);         //
    writeBitField(OSCCON1, 0x0F, 2);        // divide by 4
}


void startUp_init(void) {
    /*  PMD0 contains NVM and System Clock disable bit. It's better to manipulate 
        this register by single bit to prevent overwriting NVM and System Clock Disable bit.
        
        NVM Disable bit    ->   bit 2
        System Clock Disable bit ->    bit 7
     */
    
    clearBit(PMD0, 0);
    setBit(PMD0, 1);
    setBit(PMD0, 3);
    setBit(PMD0, 4);
    setBit(PMD0, 5);
    setBit(PMD0, 6);
    
    PMD1 = 0xFE;
    PMD1bits.TMR2MD = 0;
    PMD1bits.TMR4MD = 0;
    PMD2 = 0x7F;
    PMD3 = 0xFF;
    PMD3bits.CCP1MD = 0;
    PMD3bits.CCP2MD = 0;
    PMD4 = 0xBF;
    PMD5 = 0xFF;
    
    ANSELA = 0;     // set PORTA as digital I/O
    TRISA = 0xFF;   // PORTA as input
    WPUA = 0b11011111;  // enable pull-up except on RA5 (Vbat)
    
    // PORTB <4:0> as input
    writeBitField(ANSELB, 0b01111111, 0);
    writeBitField(TRISB, 0b01111111, 0b01111111);
    writeBitField(WPUB, 0b01111111, 0b01111111);
    
    // PORTC <5:3> as input
    writeBitField(ANSELC, 0b00111111, 0);
    writeBitField(TRISC, 0b00111000, 0b00111000);
    writeBitField(WPUC, 0b00111000, 0b00111000);
    
    // RC6 (TX) and RC5 (RX) as inputs
    clearBit(ANSELC, 6);
    clearBit(ANSELC, 7);
    setBit(TRISC, 6);
    setBit(TRISC, 7);
    
    clearBit(ANSELC, 2);
    clearBit(TRISC, 2);
    
    //setBit(IOCAP, 2);
    setBit(IOCAN, 2);
    //setBit(IOCAN, 7);
    clearBit(IOCAF, 2);
    clearBit(IOCAF, 7);
    setBit(PIE0, 4);
    
    // external interrupt
    INTPPS = 0x0A;
    INTCONbits.INTEDG = 0; // falling edge
    PIR0bits.INTF = 0;
    PIE0bits.INTE = 1;   
}
