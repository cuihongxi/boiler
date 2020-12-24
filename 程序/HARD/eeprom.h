#ifndef _EEPROM_H
#define _EEPROM_H

#include "uhead.h"


/////////////////////////////////////////////////////////////////


void EEPROM_SectorErase(u16 EE_address);
void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 length);
u8 EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 length);
// ÉÈÇøÈ«Ð´0
u8 EEPROM_write_Zero(u16 EE_address);
#endif
