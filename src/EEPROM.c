#include <xc.h>
#include <stdint.h>

uint8_t EEPROM_read(uint8_t address)
{
    NVMCON1 |= (1 << 6);
    NVMADRH = 0xF0;
    NVMADRL = address;
    NVMCON1 |= (1 << 0);
   
    return NVMDATL;
}

void EEPROM_write(uint8_t address, uint8_t eeprom_data)
{
    
    NVMDATH = 0;
    NVMDATL = eeprom_data;
    NVMCON1 |= (1 << 6);
    NVMADRH = 0xF0;
    NVMADRL = address;
    NVMCON1 |= (1 << 2);    // enable write
    INTCON = (INTCON & ~(1 << 7)) & ~(1 << 6);  // disable interrupts
    
    // unlock sequence
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1 |= (1 << 1);    //initiate write
    
    while(NVMCON1bits.WR == 1);
    INTCON |= (1 << 7) | (1 << 6);
}
