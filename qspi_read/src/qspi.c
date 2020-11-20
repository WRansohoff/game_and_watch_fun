#include "qspi.h"


// Use 'status-polling' mode to wait for Flash register status.
void qspi_reg_wait( uint8_t reg, uint32_t msk, uint32_t mat ) {
  // Disable the peripheral.
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_EN );
  // Set the 'mask', 'match', and 'polling interval' values.
  OCTOSPI1->PSMKR = msk;
  OCTOSPI1->PSMAR = mat;
  OCTOSPI1->PIR   = 0x10;
  // Set the 'auto-stop' bit to end the transaction after a match.
  OCTOSPI1->CR   |=  ( OCTOSPI_CR_APMS );
  // Clear instruction, mode and transaction phases.
  OCTOSPI1->CCR  &= ~( OCTOSPI_CCR_IMODE |
                       OCTOSPI_CCR_DMODE |
                       OCTOSPI_CCR_ADMODE );
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_FMODE );
  // Set 4-wire instruction and data modes, and auto-polling mode.
  OCTOSPI1->CCR  |=  ( ( 3 << OCTOSPI_CCR_IMODE_Pos ) |
                       ( 3 << OCTOSPI_CCR_DMODE_Pos ) );
  OCTOSPI1->CR   |=  ( 2 << OCTOSPI_CR_FMODE_Pos );
  // Enable the peripheral.
  OCTOSPI1->CR   |=  ( OCTOSPI_CR_EN );
  // Set the given 'read register' instruction to start polling.
  OCTOSPI1->IR    = reg;
  // Wait for a match.
  while ( OCTOSPI1->SR & OCTOSPI_SR_BUSY ) {};
  // Acknowledge the 'status match flag.'
  OCTOSPI1->FCR  |=  ( OCTOSPI_FCR_CSMF );
  // Un-set the data mode and disable auto-polling.
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_FMODE );
  OCTOSPI1->CCR  &= ~( OCTOSPI_CCR_DMODE | OCTOSPI_CCR_IMODE );
  // Disable the peripheral.
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_EN );
}

// Enable writes on the QSPI Flash. Must be done before every
// erase / program operation.
void qspi_wen() {
  // Disable the peripheral.
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_EN );
  // Clear the instruction, mode, and transaction phases.
  OCTOSPI1->CCR  &= ~( OCTOSPI_CCR_IMODE |
                       OCTOSPI_CCR_DMODE |
                       OCTOSPI_CCR_ADMODE );
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_FMODE );
  // Set 4-wire instruction mode.
  OCTOSPI1->CCR  |=  ( 3 << OCTOSPI_CCR_IMODE_Pos );
  // Enable the peripheral and send the 'write enable' command.
  OCTOSPI1->CR   |=  ( OCTOSPI_CR_EN );
  OCTOSPI1->IR    = 0x06;
  // Wait for the transaction to finish.
  while ( OCTOSPI1->SR & OCTOSPI_SR_BUSY ) {};
  // Disable the peripheral.
  OCTOSPI1->CCR  &= ~( OCTOSPI_CCR_IMODE );
  OCTOSPI1->CR   &= ~( OCTOSPI_CR_EN );
  // Wait until 'writes enabled' is set in the config register.
  qspi_reg_wait( 0x05, 0x43, 0x42 );
}

// Erase a 4KB sector. Sector address = ( snum * 0x1000 )
void qspi_erase_sector( uint32_t snum ) {
  // TODO: No point in implementing this until
  // I've finished dumping the existing Flash image.
}

// Write one word of data (4 bytes) to a QSPI Flash chip.
void qspi_write_word( uint32_t addr, uint32_t data ) {
  // TODO: No point in implementing this until
  // I've finished dumping the existing Flash image.
}

// Write a page (or less) of data. This method will return without
// action if `start` is in a different page from `start + len`.
void qspi_write_page( int start, int len, uint8_t* data ) {
  // TODO: No point in implementing this until
  // I've finished dumping the existing Flash image.
}
