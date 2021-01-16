#include <xc.h>
#include <stdint.h>
#include "defines.h"
#include "macros.h"


void PWM_init(uint8_t pinName)
{
    if(pinName == BUZZER_PIN)      // BUZZER is on pin RB0. Users CCP1 and TMR2
    {
        clearBit(ANSELB, 0);
        setBit(TRISB, 0);
        
        RB0PPS = 0x09;  
        PR2 = 30;
        CCPR1H = 0;
        CCPR1L = 63;
    
        clearBit(CCP1CON, 7);
        clearBit(CCP1CON, 4);   // right aligned
        writeBitField(CCP1CON, 0b00001111, 0b00001111);
        clearBit(PIR4, 1);
        writeBitField(T2CLKCON, 0b00001111, 0b00000001);
        writeBitField(T2CON, 0b01110000, 0);
    }
    else if(pinName == BACKLIGHT_OUTPUT_PIN)   // BACKLIGHT is on pin RB1(high current source pin)
    {
        clearBit(ANSELB, 1);
        setBit(TRISB, 1);
        
        RB1PPS = 0x0A;  
        PR4 = 30;
        CCPR2H = 0;
        CCPR2L = 63;
    
        clearBit(CCP2CON, 7);
        clearBit(CCP2CON, 4);
        writeBitField(CCP2CON, 0b00001111, 0b00001111);
        writeBitField(CCPTMRS0, 0b00001100, 0b00001000);
        setBit(PIR4, 3);
        writeBitField(T4CLKCON, 0b00001111, 0b00000001);
        writeBitField(T4CON, 0b01110000, 0);
    }
}

//PWM duty cycle
void PWM_dutyCycle(uint8_t pin, uint8_t duty) {
    if(pin == BUZZER_PIN)
    {
        CCPR1L = duty;
        CCPR1H = 0;
    }
    else if(pin == BACKLIGHT_OUTPUT_PIN)
    {
        CCPR2L = duty;
        CCPR2H = 0;
    }
}


void PWM_off(uint8_t pin)
{
    if(pin == BUZZER_PIN)
    {
        clearBit(CCP1CON, 7);
        clearBit(T2CON, 7);
        setBit(TRISB, 0);
    }
    else if(pin == BACKLIGHT_OUTPUT_PIN)
    {
        clearBit(CCP2CON, 7);
        clearBit(T4CON, 7);
        setBit(TRISB, 1);
    }
}

void PWM_on(uint8_t pin)
{
    if(pin == BUZZER_PIN)
    {
        clearBit(PIR4, 1);
        setBit(T2CON, 7);
        while(checkBit(PIR4, 1) == 0);
        setBit(CCP1CON, 7);
        clearBit(TRISB, 0);
    }
    else if(pin == BACKLIGHT_OUTPUT_PIN)
    {
        clearBit(PIR4, 3);
        setBit(T4CON, 7);
        while(checkBit(PIR4, 3) == 0);
        setBit(CCP2CON, 7);
        clearBit(TRISB, 1);
    }
}



