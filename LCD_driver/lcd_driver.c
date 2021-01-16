/*
 * File:   UART.c
 * Author: User
 *
 * Created on March 29, 2020, 5:15 PM
 */


// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO7         // Oscillator Selection bits (EC oscillator, CLKO function on RA6)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF         // Watchdog Timer Enable bit (WDT enabled) 
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = ON    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

#include <stdint.h>
#include <xc.h>
#include <stdbool.h>
#include <pic18lf4520.h>



#define _XTAL_FREQ (250000)


#define SYSCLOCK_SELECT  (0B10 << 0U)       // intosc block
#define INTOSC_FREQ_SELECT (0B010 << 4U)    // 4Mhz ,previous 0.250MHz(010)
#define INTOSC_SOURCE (1U << 7U)        
#define TMR0_LOAD   252      // load on timer0 register 5ms ISR  (16ms - 131)
#define BUFFER_LEN  7

// needed for setting timer0 register
#define _XTAL_FREQ_NORM     (_XTAL_FREQ / 1000000)
#define PRESCALER           256
#define OVERFLOW_TIMER0_MSEC     5
#define LOAD_TIMER0         (256 - ((4  * 1000 * OVERFLOW_TIMER0_MSEC) / (PRESCALER * _XTAL_FREQ_NORM)))

// needed for setting baud rate of UART
#define BAUD_RATE		2400		//bps
#define LOAD_BAUD_REGISTER		((_XTAL_FREQ / 4(BAUD_RATE)) - 1)  

void TX_char(uint8_t data);
void __interrupt() RX_char(void);
void TX_word(uint8_t word[]);

uint8_t const digit_val[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0xFF, 0x6F,
                             0x58, 0x38, 0x5C, 0x78, 0x5E, 0x1C, 0x73, 0x40, 0xF7, 0xD4, 0x50                                                       
                            };
uint8_t volatile char_data;
uint8_t volatile rx_flag = 0;
uint8_t volatile char_data;
uint8_t volatile char_buffer[BUFFER_LEN];
uint8_t volatile stopFlag = 0;
uint8_t volatile blankDone = 0;
uint8_t volatile sleepFlag = 0;



void main(void) {
    // initiating oscillator
    OSCCON = (OSCCON & (0B11111100)) | (SYSCLOCK_SELECT);
    OSCCON = (OSCCON & (0B10001111)) | (INTOSC_FREQ_SELECT);

    // initiating I/O ports
    ADCON0bits.ADON = 0;
    ADCON1bits.PCFG0 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 1;		//set ports as digital I/O
    TRISB = 0x00;		
    TRISD = 0x00;
    TRISE &= ~(1U << 1U);
    TRISE &= ~(1U << 0U);
    TRISE &= ~(1U << 2U);
    TRISA = TRISA & 0B01000000;		// isolate RA7 and RA6 (CLK I/O)
    TRISC = TRISC & 0B11000000;		// RC7 and RC6 are UART pins
    
    // initiating UART
    SPBRG = 25;     //9.6 kbps , previous(4Mhz, 103)
    SPBRGH = 0;
    RCSTA |= (1U << 7U);
    TRISC |= (1U << 6U) | (1U << 7U);   // set RC6, RC7 as inputs
    BAUDCONbits.WUE = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;
    TXSTAbits.SYNC = 0;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;  // cleared because RCIF is set after RCIE is enabled
    RCSTAbits.CREN = 1;
    
    
    // initiating timer0    
    T0CON &= ~(1U << 7U); //timer off
    T0CON |= (1U << 6U);    // 8 bit timer
    TMR0L = TMR0_LOAD;     //timer will overflow at 5ms
    T0CON &= ~(1U << 3U);     // prescaler enable
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;       // prescaler of 32 (previous 128(110)
    T0CON &= ~(1U << 5U);    // get input on intosc 
    T0CON |= (1U << 7U);    // timer on 
    INTCONbits.TMR0IE = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    

    // set display at power-up
    PORTEbits.RE0 = 0;
    PORTB = digit_val[6];       // blank display
    PORTD = digit_val[8];
    PORTC = digit_val[5];
    PORTA = digit_val[2];
    uint8_t char_input = 0;
    uint8_t PORTC_copy = 0;
    uint8_t PORTA_copy = 0;
    uint8_t symbol_num[BUFFER_LEN - 1] = {0};
    
    
    LATAbits.LATA7 = 0;
    TX_char('a');
    
    while(1) {
		
        if(rx_flag == 1) {
            rx_flag = 0;
            
           
            for(uint8_t i = 0; i < (BUFFER_LEN - 1); ++i) {
                
                uint8_t char_buffer_copy;
                char_buffer_copy = char_buffer[i];
                
                switch(char_buffer_copy){
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        symbol_num[i] = char_buffer_copy - 48;
                        break;
                    case 'c':
                        symbol_num[i] = 10;
                        break;
                    case 'L':
                        symbol_num[i] = 11;
                        break;
                    case 'o':
                        symbol_num[i] = 12;
                        break;
                    case 't':
                        symbol_num[i] = 13;
                        break;
                    case 'd':
                        symbol_num[i] = 14;
                        break;
                    case 'u':
                        symbol_num[i] = 15;
                        break;
                    case 'p':
                        symbol_num[i] = 16;
                        break;
                    case '-':
                        symbol_num[i] = 17;
                        break;
                    case 's':
                        symbol_num[i] = 5;                   
                        break;
                    case '.':
                        symbol_num[i] = 1;
                        break;
                    case ',':
                        symbol_num[i] = 0;
                        break;
                    case  ':':
                        symbol_num[i] = 1;
                        break;
                    case  ';':
                        symbol_num[i] = 0;
                        break;
                    case  'a':
                        symbol_num[i] = 18;
                        break;
                    case  'm':
                        symbol_num[i] = 19;
                        break;
                    case  'r':
                        symbol_num[i] = 20;
                        break;
                    case 'x':
                       
                        // flag the stop flag
                        stopFlag = 1;
                        break;
                    default:
                        symbol_num[i] = 1;  // blank if character is unidentified
                        break;
                }
            
           }
            
            // if not stopFlag
            if(stopFlag == 0) 
            {
            INTCONbits.GIE = 0;				// this section should not be interrupted because this dictates what 
            INTCONbits.PEIE = 0;				// value will be displayed
            PORTEbits.RE0 = 0;
            PORTB = digit_val[symbol_num[0]];
            PORTD = (PORTD & 0B10000000) | digit_val[symbol_num[1]];
            PORTC_copy = digit_val[symbol_num[2]];
            PORTC = PORTC_copy;
            PORTD = (PORTD & 0B01111111) | ((PORTC_copy << 1U) & 0B10000000);   // move RC6 to RD7
            PORTA_copy = digit_val[symbol_num[3]];
            PORTA = PORTA_copy;
            PORTB = (PORTB & 0B01111111) | ((PORTA_copy << 1U) & 0B10000000);   // move RA6 to RB7
            PORTEbits.RE1 = symbol_num[4]; 
            PORTEbits.RE2 = symbol_num[5];
            INTCONbits.GIE = 1;
            INTCONbits.PEIE = 1;
            }
            // else
            
            else if(stopFlag == 1)
            {
                
                // blank the display
        
                PORTEbits.RE0 = 1;
                PORTEbits.RE1 = 1;
                PORTEbits.RE2 = 1;
                PORTA = 0xFF;
                PORTB = 0xFF;
                PORTC = 0xFF;
                PORTD = 0xFF;
            }
                
            }
    
        if((stopFlag == 1) && (blankDone == 1))
        {
            stopFlag = 0;
            blankDone = 0;
            BAUDCONbits.WUE = 1;
            sleepFlag = 1;
            SLEEP();
        }
   
    }
}

void __interrupt() RX_char(void) {
    
    if((PIR1bits.RCIF == 1) && (PIE1bits.RCIE == 1)) {
        
                                                               // does not need to clear RCIF because it will be cleared by the hardware
        uint8_t static charCounter = 0;			// counter must preserve value on every call to RX ISR
        char_data = RCREG;
        
        
        
        
        
        if((char_data != '\0') && (charCounter < (BUFFER_LEN - 1))) {	// place to buffer while data is not null and buffer is not full
            char_buffer[charCounter] = char_data;
            ++charCounter;
        }
        else {
            if(charCounter == (BUFFER_LEN - 1)) {
                char_buffer[charCounter] = '\0';        // place null at end
            }
            
            // dump the dummy stream of characters
            // the dummy stream must be 6 characters only.
            if(sleepFlag == 1)
            {
                rx_flag = 0;
                sleepFlag = 0;
            }
            else
            {
                rx_flag = 1;
            }
            // start processing the buffer
            charCounter = 0;    // next input will begin at index 0
        }
    }

    // frame rate is 30Hz
    else if(INTCONbits.TMR0IF == 1) {     
            INTCONbits.T0IF = 0;		// clear the flag to prevent the ISR to be called again after exiting
            PORTEbits.RE0 = ~PORTEbits.RE0;		// toggle ports to drive LCD
            PORTEbits.RE1 = ~PORTEbits.RE1;
            PORTEbits.RE2 = ~PORTEbits.RE2;
            
            PORTB = ~PORTB;
            PORTD = ~PORTD;
            PORTC = ~PORTC;
            PORTA = ~PORTA;
            TMR0L = TMR0_LOAD;  	// reset timer
            
            if(stopFlag == 1)
            {
                blankDone = 1;
            }
            
            
    }
}

void TX_word(uint8_t word[]){
    uint8_t *ptrWord;
    ptrWord = word;
    while(*ptrWord != '\0') {
        TX_char(*ptrWord);
        ptrWord++;
    }
}

void TX_char(uint8_t data) {
    TXSTAbits.TXEN = 1;
    TXREG = data;
    while(PIR1bits.TXIF != 1);		// wait until buffer is emptied
}