#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "UART.h"
#include "defines.h"
#include "globals.h"
#include "macros.h"
#include "combine.h"


void extractDigits(uint8_t firstTwoDigits, uint8_t secondTwoDigits) {
    digit_ones  = extractDigitOnes(firstTwoDigits);
    digit_tens = extractDigitTens(firstTwoDigits);
    digit_hundreds = extractDigitOnes(secondTwoDigits);
    digit_thousands = extractDigitTens(secondTwoDigits);
}

uint8_t extractDigitOnes(uint8_t num){
    num = num % 10;
    return num;
}
uint8_t extractDigitTens(uint8_t num) {
    num = num / 10;
    return num;
}

bool isPressed(uint8_t *regAddress, uint8_t pinNum) {
    bool isReleased;
   
    if(checkBit(*regAddress, pinNum) == 0) {
         __delay_ms(1);
         while(checkBit(*regAddress, pinNum) == 0);
         __delay_ms(1);
         isReleased = 1;
    }
    else {
         isReleased = 0;
    }
    return isReleased;
}


void clock_init(void) {
    subMode = RUN_MODE;
    stopResumeFlag = RESUME_MODE;
                
    TX_word("cLoc");
    __delay_ms(500);
                
    minutesCopy = minutes;
    hoursCopy = hours;
    extractDigits(minutesCopy, secondsCopy);
    //updateDigits();
    
    // TMR0H = TIMER_REG_HIGH_BYTE; // load timer registers  
    TMR0H = TIMER_REG_HIGH_BYTE;
    TMR0L = TIMER_REG_LOW_BYTE;
   // TMR0L = TIMER_REG_LOW_BYTE; 
    setBit(T0CON0, 7);   // enable timer
    clearBit(PIR0, 5);  // clear flag
    setBit(PIE0, 5); // enable timer0 interrupt
}

void countUp_init(void) {
    subMode = SET_MODE;
    stopResumeFlag = RESUME_MODE;
    
    TX_word("ctup");
    __delay_ms(500);

    extractDigits(setSecondsCtUp, setMinutesCtUp);
    //updateDigits();
    
    TMR0H = TIMER_REG_HIGH_BYTE; // load timer registers  
    TMR0L = TIMER_REG_LOW_BYTE; 
    setBit(T0CON0, 7);   // enable timer
    clearBit(PIR0, 5);  // clear flag
    setBit(PIE0, 5); // enable timer0 interrupt
        
}

void countDown_init(void) {
    subMode = SET_MODE;
    stopResumeFlag = RESUME_MODE;
    
     TX_word("ctdo");
    __delay_ms(500);

    extractDigits(setSecondsCtDown, setMinutesCtDown);
    //updateDigits();
    
    TMR0H = TIMER_REG_HIGH_BYTE; // load timer registers  
    TMR0L = TIMER_REG_LOW_BYTE; 
    setBit(T0CON0, 7);   // enable timer
    clearBit(PIR0, 5);  // clear flag
    setBit(PIE0, 5); // enable timer0 interrupt   
}

void updateDigits(void) {
    
        TX_char(digit_thousands + 48);    
        TX_char(digit_hundreds + 48);     
        TX_char(digit_tens + 48);             
        TX_char(digit_ones + 48);   
    
        if((mode_current == COUNT_UP_MODE) || (mode_current == COUNT_DOWN_MODE)) {
            if(subMode == SET_MODE) {
                TX_char(controlPoint);
            }
            else {
                TX_char('.');
            }
        }
        else
            TX_char(',');       // switch off decimal point if clock mode
    
        if(mode_current == CLOCK_MODE ) {
            if(subMode == RUN_MODE) {   // blink colon while in run Mode
                TX_char(controlColon);
            }
            else
                TX_char(':');       // just switch on colon on setMode
        }
        else {
            TX_char(';');       // if not clock mode, switch off colon
        }
    
        TX_char('\0');
    
    
}

