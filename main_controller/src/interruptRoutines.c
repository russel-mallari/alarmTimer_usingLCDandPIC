#include <xc.h>
#include <stdint.h>
#include "macros.h"
#include "RTCC.h"
#include "defines.h"
#include "globals.h"
#include "UART.h"

void __interrupt() interrupt_routines(void) {
    
    if(IOCAFbits.IOCAF2 == 1) {
        IOCAFbits.IOCAF2 = 0;
        PIE0bits.IOCIE = 0;
        
        if(PORTAbits.RA7 == 0) {
            ++encoderCounter_ClockWise;
            encoderCounter_counterClockWise = 0;
        }
        else {
            ++encoderCounter_counterClockWise;
            encoderCounter_ClockWise = 0;
        }
        PIE0bits.IOCIE = 1;
    }
    
    else if(PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        PIE0bits.TMR0IE = 0;
        T0CON0bits.T0EN = 0;
    
        ++ticksCounter;
        updateDisplay = 1;
        updateClock = TRUE;    // read RTC every 250ms
    
        if(ticksCounter == 10) {
            ticksCounter = 0;
            ledState = ~ledState;
    
            if(mode_current == CLOCK_MODE) {
                if(controlColon == ':') {
                    controlColon = ';';
                }
                else {
                    controlColon = ':';
                }
        
                if((stopFlag == FALSE) && (subMode == RUN_MODE)) {
            
                }
            }
      
            
            if(mode_current == COUNT_UP_MODE) {
                if(controlPoint == ',')
                    controlPoint = '.';
                else
                    controlPoint = ',';
        
                if((stopFlag == FALSE) && (subMode == RUN_MODE)) {
                    ++seconds;
                    if(seconds == 60) {
                        seconds = 0;
                        ++minutes;
                    }
                    if(minutes == 99) {
                        minutes = 0;
                    }
                }
            }
        
            else if(mode_current == COUNT_DOWN_MODE) {
                if(controlPoint == ',')
                    controlPoint = '.';
                else
                    controlPoint = ',';
        
                if((stopFlag == FALSE) && (subMode == RUN_MODE)) {
                    if((seconds == 0) && (minutes == 0)) {
                        ;               // do nothing
                    }
                    else {
                        --seconds;
                        if(seconds == -1) {
                            seconds = 59;
                            --minutes;
                        }
                    }
                }
            }
        }
    
        TMR0H = TIMER_REG_HIGH_BYTE;
        TMR0L = TIMER_REG_LOW_BYTE;
        T0CON0bits.T0EN = 1;
        PIR0bits.TMR0IF = 0;
        PIE0bits.TMR0IE = 1;
    }
    
    else if((PIR8bits.RTCCIF == 1) && (PIE8bits.RTCCIE == 1))
    {
        if(PIR8bits.RTCCIF == 1) {
            PIR8bits.RTCCIF = 0;
            alarmFlag_1 = TRUE;
           
            clearBit(ALRMCON, 7);
            PIE8bits.RTCCIE = 1;
            ALARM_DONE = TRUE;
        }
    }
    
    // added sleep
    else if((PIE0bits.INTE == 1) && (PIR0bits.INTF == 1))
    {
        PIR0bits.INTF = 0;
        sleepFlag = ~sleepFlag; 
    }
}

void disableInterrupt(void) {
    INTCON &= ~((uint8_t)1 << (uint8_t)7);   // global interrupt disable
    INTCON &= ~((uint8_t)1 << (uint8_t)6);   // periperhal interrupt disable
}

void enableInterrupt(void) {
    INTCON |= ((uint8_t)1 << (uint8_t)7);   // global interrupt enable
    INTCON |= ((uint8_t)1 << (uint8_t)6);   // periperhal interrupt enable
}