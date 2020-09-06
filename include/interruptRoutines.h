#ifndef INTERRUPT_ROUTINES_H
#define INTERRUPT_ROUTINES_H

void __interrupt() oneSecondUpdate(void);
void disableInterrupt(void);
void enableInterrupt(void);

#endif // INTERRUPT_ROUTINES_H