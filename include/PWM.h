#include <xc.h>
#include <stdint.h>


#ifndef PWM_H
#define PWM_H

void PWM_init(uint8_t pinName);
void PWM_dutyCycle(uint8_t pin, uint8_t duty);
void PWM_off(uint8_t pin);
void PWM_on(uint8_t pin);


#endif  // PWM_H
