#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

#define _XTAL_FREQ 250000UL
#define COUNTER_CLOCKWISE (uint8_t)1
#define CLOCKWISE (uint8_t)2
#define NO_ROTATION (uint8_t)0

#define TIMER_TICK_MILLIS   (100U)
#define LOAD_TIMER_REG      (65535U - (uint16_t)((TIMER_TICK_MILLIS * _XTAL_FREQ) / 4000U))
#define TIMER_REG_LOW_BYTE  (uint8_t)(LOAD_TIMER_REG & 0x00FF)
#define TIMER_REG_HIGH_BYTE (uint8_t)((LOAD_TIMER_REG & 0xFF00) >> 8U)


// from the old
#define MODE_PIN 0
#define SET_RUN_PIN 1
#define INC_DEC_PIN 2
#define STOP_RESUME_PIN 3

#define MODE_ROLLBACK 3
#define SUBMODE_ROLLBACK 2

#define STOP_MODE   0
#define RESUME_MODE 1

#define CLOCK_MODE  0
#define COUNT_UP_MODE 1
#define COUNT_DOWN_MODE 2 
#define ADJUST_MODE 3

#define SET_MODE 0
#define RESET_MODE 1
#define RUN_MODE 3

#define BUZZER 0
#define BACKLIGHT 1
#define UPDATE_PWM_PIN 2

#define SET_ALARM   0
#define SET_CLOCK   1
#define ALARM_DONE  VB0GPR

#define FORMAT_PIN  3
#define FORMAT_24   0
#define FORMAT_12   1

#define AM_TIME  0
#define PM_TIME  1

#define SAVE 0
#define RECALL 1

#endif // DEFINES_H