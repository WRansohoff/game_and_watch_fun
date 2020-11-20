#ifndef _VVC_SSPI_H
#define _VVC_SSPI_H

#include "global.h"

// Macros for pin toggling.
// B1: MOSI, B2: SCK, D12: MISO, E11: NCS
#define CS_LO()   ( GPIOE->BSRR |= ( 1 << ( 11 + 16 ) ) );
#define CS_HI()   ( GPIOE->BSRR |= ( 1 << 11 ) );
#define SCK_LO()  ( GPIOB->BSRR |= ( 1 << ( 2 + 16 ) ) );
#define SCK_HI()  ( GPIOB->BSRR |= ( 1 << 2 ) );
#define MISO_R()  ( GPIOD->IDR & ( 1 << 12 ) )
#define MOSI_LO() ( GPIOB->BSRR |= ( 1 << ( 1 + 16 ) ) );
#define MOSI_HI() ( GPIOB->BSRR |= ( 1 << 1 ) );

// Write a byte of data using software SPI.
void sspi_w(uint8_t dat);
// Read a byte of data over software SPI.
uint8_t sspi_r();

#endif
