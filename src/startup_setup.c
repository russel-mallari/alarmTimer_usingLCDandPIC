#include <xc.h>
#include <stdint.h>
#include "OscPeripheral_setup.h"
#include "interruptRoutines.h"
#include "timers.h"
#include "UART.h"
#include "RTCC.h"
#include "PWM.h"
#include "defines.h"
#include "macros.h"


void startup_setup(void) {
    disableInterrupt();
    HFIntOsc_init();
    startUp_init();
    timer0_setup();
    TX_init(2400, 250000);
    RTCC_init();
  
    // ALARM_DONE -> saves the status of alarm when power is cut off
    // If an alarm is not performed before a power cut-off, it will be set again at start-up.
    if(ALARM_DONE == 0)
    {
        RTCC_setAlarm(ALRMHR, ALRMMIN);
    }
    
    PWM_init(BUZZER);
    PWM_off(BUZZER);
    //PWM_init(BACKLIGHT);
    enableInterrupt();
    
    //EEPROM_WRITE(7, 1);
}


