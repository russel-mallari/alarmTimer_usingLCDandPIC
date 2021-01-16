#include <stdint.h>
#include <xc.h>
#include "macros.h"

void RTCC_init(void) {
    //setup clock
    writeBitField(RTCCON, 0x03, 0);     // crystal oscillator
    //enable rtcc
    setBit(RTCCON, 5);      //enable write to RTC registers. This bit should be set so RTC can be enable.
    setBit(RTCCON, 7);      //enable RTC module
    while(checkBit(RTCCON, 7) == 0);    //wait for the oscillator to stabilize
    clearBit(RTCCON, 5);    //lock the registers for writing
    
}

uint8_t RTCC_read(volatile uint8_t *RTCC_register) {
    setBit(RTCCON, 5);      //enable write to RTC registers. This bit should be set so RTC can be enable.
    setBit(RTCCON, 7);      //enable RTC module
    while(checkBit(RTCCON, 7) == 0);    //wait for the oscillator to stabilize
    while(checkBit(RTCCON, 4) == 1);        // wait for the synch before reading
    clearBit(RTCCON, 5);    //lock the registers for writing
    return *RTCC_register;
}

void RTCC_write(volatile uint8_t *RTCC_REG, uint8_t value) {
    setBit(RTCCON, 5);      //enable write to RTC registers. This bit should be set so RTC can be enable.
    clearBit(RTCCON, 7);      //disable RTC module
    while (checkBit(RTCCON, 4) == 1);
    *RTCC_REG = value ;
    setBit(RTCCON, 7);      //enable RTC module
    while(checkBit(RTCCON, 7) == 0); 
    clearBit(RTCCON, 5);
}

uint8_t decimalToBCD(uint8_t decimal) {
    uint8_t ones = 0;
    uint8_t tens = 0;
    uint8_t BCD = 0;

    tens = decimal / 10;
    ones = decimal % 10;
    BCD =  (BCD | (tens << 4)) | (ones);

    return BCD;
}

void RTCC_setAlarm(uint8_t alarm_hour, uint8_t alarm_mins) {
    
    setBit(RTCCON, 5);      // enable write to RTC registers. This bit should be set so RTC can be enable.
    clearBit(RTCCON, 7);    // disable RTCC to prevent updating timer registers when setting alarm.
    clearBit(ALRMCON, 7);   // disable ALARM to prevent false alarm
  
    clearBit(ALRMCON, 6);     // don't repeat
    writeBitField(ALRMCON, 0B00111100, 0B00011000);     // Alarm Mask (Bit 2-5)
    ALRMRPT = 0x00;     // don't repeat
    
    ALRMHR = alarm_hour;
    ALRMMIN = alarm_mins;
    ALRMSEC = 0x01;     // alarm on 00 has a bug due to silicon.
    
    setBit(ALRMCON, 7);
    setBit(RTCCON, 7);
    while(checkBit(RTCCON, 7) == 0);
    clearBit(RTCCON, 5);
    clearBit(PIR8, 6);      // clear RTCC to prevent false alarm
    setBit(PIE8, 6);        // enable RTCC interrupt
    setBit(INTCON, 6);      // 
    setBit(INTCON, 7);      // enable peripheral and global interrupts
}

uint8_t BCDtoDecimal(uint8_t BCDNumber) {
    uint8_t upperNibble, lowerNibble;
    uint8_t decimalNumber;
    lowerNibble = BCDNumber & 0x0F;
    upperNibble = (BCDNumber & 0xF0) >> 4;
    decimalNumber = upperNibble * 10 + lowerNibble;
    
    return decimalNumber;
}