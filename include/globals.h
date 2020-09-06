#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include <stdbool.h>

extern volatile int8_t seconds;
extern volatile uint8_t minutes;
extern volatile uint8_t hours;
extern volatile int8_t secondsCopy;        // seconds, minutes, hours will be read/write in many places
extern volatile uint8_t minutesCopy;        // Thus, don't directly read/write them.
extern volatile uint8_t hoursCopy;
 
extern volatile uint8_t setSecondsCtUp;
extern volatile uint8_t setMinutesCtUp;
     
extern volatile uint8_t setSecondsCtDown;
extern volatile uint8_t setMinutesCtDown;
     
extern volatile uint8_t setMinutesClock;
extern volatile uint8_t setHoursClock; 
     

extern volatile uint8_t mode_current;
extern volatile uint8_t mode_previous;
 
extern volatile uint8_t subMode;
extern volatile uint8_t subMode_previous;
 
extern volatile uint8_t stopResumeFlag;
extern volatile uint8_t stopResumeFlag_previous;
 
 
extern volatile uint8_t digit_ones;
extern volatile uint8_t digit_tens;
extern volatile uint8_t digit_hundreds;
extern volatile uint8_t digit_thousands;
 
extern volatile bool stopFlag;

extern volatile bool ledState;
extern volatile char controlColon;
extern volatile char controlPoint;

extern volatile uint8_t updateClock;
extern volatile uint8_t updateDisplay;

extern volatile uint8_t ticksCounter;
extern volatile uint8_t encoderCounter;
extern volatile uint8_t digit_value;

extern volatile uint8_t encoderRotation;
extern volatile uint8_t otherPinEncoder_state;

extern volatile uint8_t encoderCounter_ClockWise;
extern volatile uint8_t encoderCounter_counterClockWise;

extern volatile bool sleepFlag;

//extern volatile uint8_t alarmFlag_1;
extern volatile uint8_t alarmFlag_1;

#endif  // GLOBALS_H    