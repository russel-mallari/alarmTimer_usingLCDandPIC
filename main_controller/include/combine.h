#ifndef COMBINE_H
#define COMBINE_H

#include <stdint.h>
#include <stdbool.h>

void updateDigits(void);
void extractDigits(uint8_t firstTwoDigits, uint8_t secondTwoDigits);
uint8_t extractDigitOnes(uint8_t num);
uint8_t extractDigitTens(uint8_t num) ;
bool isPressed(uint8_t *regAddress, uint8_t pinNum);
bool getRawState(uint8_t pinNum);
void clock_init(void);
void countUp_init(void); 
void countDown_init(void);
uint8_t BCDtoDecimal(uint8_t BCDNumber);

#endif // COMBINE_H
    
