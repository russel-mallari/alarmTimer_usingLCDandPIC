#ifndef UART_H
#define UART_H

#include <xc.h>
#include <stdint.h>

void TX_char(uint8_t data);
void TX_word(uint8_t *word);
void TX_init(uint32_t baudRate, uint32_t OscFreq);

#endif  // UART_H