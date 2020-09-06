#include <xc.h>
#include <stdint.h>
#include "defines.h"
#include "macros.h"


//PWM init
    // set a pin as pwm
    // parameters: pin Number
    // 
void PWM_init(uint8_t pinName)
{
    if(pinName == BUZZER)      // BUZZER is on pin RB0. Users CCP1 and TMR2
    {
        
    //ANSELBbits.ANSB0 = 0;
        clearBit(ANSELB, 0);
    //TRISBbits.TRISB0 = 1;
        setBit(TRISB, 0);
        
        RB0PPS = 0x09;  
        PR2 = 30;
        CCPR1H = 0;
        CCPR1L = 63;
    
    //CCP1CONbits.CCP1EN = 0;
        clearBit(CCP1CON, 7);
    
    //CCP1CONbits.CCP1FMT = 0;    // right aligned
        clearBit(CCP1CON, 4);
    /*
    CCP1CONbits.CCP1MODE0 = 1;
    CCP1CONbits.CCP1MODE1 = 1;
    CCP1CONbits.CCP1MODE2 = 1;
    CCP1CONbits.CCP1MODE3 = 1;
    */
        writeBitField(CCP1CON, 0b00001111, 0b00001111);
        //PIR4bits.TMR2IF = 0;
        clearBit(PIR4, 1);
        //T2CLKCONbits.CS = 1;
        writeBitField(T2CLKCON, 0b00001111, 0b00000001);
        //T2CONbits.CKPS = 0;
        writeBitField(T2CON, 0b01110000, 0);
    
    }
    else if(pinName == BACKLIGHT)   // BACKLIGHT is on pin RB1(high current source pin)
    {
        //ANSELBbits.ANSB0 = 0;
        clearBit(ANSELB, 1);
    //TRISBbits.TRISB0 = 1;
        setBit(TRISB, 1);
        
        RB1PPS = 0x0A;  
        PR4 = 30;
        CCPR2H = 0;
        CCPR2L = 63;
    
    //CCP1CONbits.CCP1EN = 0;
        clearBit(CCP2CON, 7);
    
    //CCP1CONbits.CCP1FMT = 0;    // right aligned
        clearBit(CCP2CON, 4);
    /*
    CCP1CONbits.CCP1MODE0 = 1;
    CCP1CONbits.CCP1MODE1 = 1;
    CCP1CONbits.CCP1MODE2 = 1;
    CCP1CONbits.CCP1MODE3 = 1;
    */
        writeBitField(CCP2CON, 0b00001111, 0b00001111);
    
    //CCPTMRS0bits.C2TSEL0 = 0;
    //CCPTMRS0bits.C2TSEL1 = 1;
        writeBitField(CCPTMRS0, 0b00001100, 0b00001000);
        //PIR4bits.TMR4IF = 0;
        setBit(PIR4, 3);
    //T4CLKCONbits.CS = 1;
        writeBitField(T4CLKCON, 0b00001111, 0b00000001);
    //T4CONbits.CKPS = 0;
        writeBitField(T4CON, 0b01110000, 0);
    }
}

//PWM duty cycle
void PWM_dutyCycle(uint8_t pin, uint8_t duty) {
    if(pin == BUZZER)
    {
        CCPR1L = duty;
        CCPR1H = 0;
    }
    else if(pin == BACKLIGHT)
    {
        CCPR2L = duty;
        CCPR2H = 0;
    }
}
    // set duty cycle of a pwm pin

void PWM_off(uint8_t pin)
{
    if(pin == BUZZER)
    {
        clearBit(CCP1CON, 7);
        //CCP off
        //timer off
        clearBit(T2CON, 7);
        //set as input
        setBit(TRISB, 0);
    }
    else if(pin == BACKLIGHT)
    {
        clearBit(CCP2CON, 7);
        //CCP off
        //timer off
        clearBit(T4CON, 7);
        //set as input
        setBit(TRISB, 1);
    }
}
// Off pwm

void PWM_on(uint8_t pin)
{
    if(pin == BUZZER)
    {
        //PIR4bits.TMR2IF = 0;
        clearBit(PIR4, 1);
        //T2CONbits.ON = 1;
        setBit(T2CON, 7);
        while(checkBit(PIR4, 1) == 0);
        //CCP1CONbits.CCP1EN = 1;
        setBit(CCP1CON, 7);
        //TRISBbits.TRISB0 = 0;
        clearBit(TRISB, 0);
    }
    else if(pin == BACKLIGHT)
    {
        //PIR4bits.TMR4IF = 0;
        clearBit(PIR4, 3);
        //T4CONbits.ON = 1;
        setBit(T4CON, 7);
        while(checkBit(PIR4, 3) == 0);
        //CCP2CONbits.CCP2EN = 1;
        setBit(CCP2CON, 7);
        //TRISBbits.TRISB0 = 0;
        clearBit(TRISB, 1);
    }
}
// On pwm


