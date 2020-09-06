#include <stdint.h>
#include <stdbool.h>


volatile int8_t seconds;
volatile uint8_t minutes;
volatile uint8_t hours;
volatile int8_t secondsCopy;        // seconds, minutes, hours will be read/write in many places
volatile uint8_t minutesCopy;        // Thus, don't directly read/write them.
volatile uint8_t hoursCopy;

volatile uint8_t setSecondsCtUp = 0;
volatile uint8_t setMinutesCtUp = 0;
    
volatile uint8_t setSecondsCtDown = 0;
volatile uint8_t setMinutesCtDown = 0;
    
volatile uint8_t setMinutesClock = 0;
volatile uint8_t setHoursClock = 0; 
    

volatile uint8_t mode_current;
volatile uint8_t mode_previous;

volatile uint8_t subMode;
volatile uint8_t subMode_previous;

volatile uint8_t stopResumeFlag;
volatile uint8_t stopResumeFlag_previous;


volatile uint8_t digit_ones;
volatile uint8_t digit_tens;
volatile uint8_t digit_hundreds;
volatile uint8_t digit_thousands;

volatile bool stopFlag;

volatile bool ledState = 1;

volatile char controlColon;
volatile char controlPoint;

volatile uint8_t updateClock;
volatile uint8_t updateDisplay;

volatile uint8_t ticksCounter;
volatile uint8_t encoderCounter;
volatile uint8_t digit_value;

volatile uint8_t encoderRotation;
volatile uint8_t otherPinEncoder_state;

volatile uint8_t encoderCounter_ClockWise;
volatile uint8_t encoderCounter_counterClockWise;

volatile bool sleepFlag;

volatile uint8_t alarmFlag_1;

//volatile uint8_t alarmFlag_1;