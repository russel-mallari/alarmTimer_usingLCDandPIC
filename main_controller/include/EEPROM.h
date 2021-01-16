#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  

uint8_t EEPROM_read(uint8_t address);
void EEPROM_write(uint8_t address, uint8_t eeprom_data);

#endif	// EEPROM_H

