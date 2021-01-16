#include <stdint.h>
#include <xc.h>
#include "macros.h"
#include "defines.h"
#include "UART.h"

void timer0_setup(void) {
    
    clearBit(T0CON0, 7);    //disable timer
    setBit(T0CON0, 4);      // 16 bit timer
    writeBitField(T0CON0, 0b00001111, 0);    // no postscaler
    
    TMR0H = TIMER_REG_HIGH_BYTE;
    TMR0L = TIMER_REG_LOW_BYTE;
    writeBitField(T0CON1, 0b11100000, 0b01000000);  // Fosc / 4  as source;
    clearBit(T0CON1, 4);  //synchronous to Fosc, active-low
    writeBitField(T0CON1, 0b00001111, 0);   //no prescaler
    setBit(T0CON0, 7);  //enable timer
    clearBit(PIR0, 5);  //clear flag
    setBit(PIE0, 5);    //enable timer interrupt
} 

