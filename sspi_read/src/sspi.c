#include "sspi.h"

/*
 * Write a byte of data using software SPI. For each bit:
 * 1. Pull the clock pin low.
 * 2. Set the 'MOSI' data pin to the correct value.
 * 3. Pull the clock pin high.
 */
inline void sspi_w( uint8_t dat ) {
  uint8_t sspi_i;
  // Send 8 bits, with the MSB first.
  for ( sspi_i = 0x80; sspi_i != 0x00; sspi_i >>= 1 ) {
    SCK_LO();
    if ( dat & sspi_i ) {
      MOSI_HI();
    }
    else {
      MOSI_LO();
    }
    SCK_HI();
  }
}

/*
 * Read a byte of data using software SPI. For each bit:
 * 1. Pull the clock pin low.
 * 2. Pull the clock pin high.
 * 3. Read the 'MISO' data pin.
 */
uint8_t sspi_r() {
  uint8_t sspi_i = 0x00;
  for ( int i = 7; i >= 0; --i ) {
    SCK_LO();
    SCK_HI();
    if ( MISO_R() ) {
      sspi_i |= ( 1 << i );
    }
  }
  return sspi_i;
}
