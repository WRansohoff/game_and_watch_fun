#ifndef _VVC_QSPI_H
#define _VVC_QSPI_H

#include "global.h"

void qspi_reg_wait( uint8_t reg, uint32_t msk, uint32_t mat );
void qspi_wen();
void qspi_erase_sector( uint32_t snum );
void qspi_write_word( uint32_t addr, uint32_t data );
void qspi_write_page( int start, int len, uint8_t* data );

#endif
