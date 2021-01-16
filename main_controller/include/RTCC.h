#ifndef RTCC_H
#define RTCC_H

#include <stdint.h>

uint8_t RTCC_read(volatile uint8_t *RTCC_register);
void RTCC_init(void);
void RTCC_write(volatile uint8_t *RTCC_REG, uint8_t value);
uint8_t decimalToBCD(uint8_t decimal);
void startUp_init(void);
void RTCC_setAlarm(uint8_t alarm_hour, uint8_t alarm_mins);

#endif  // RTCC_H