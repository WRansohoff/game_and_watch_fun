#include "global.h"
#include "sspi.h"

// Core system clock speed.
uint32_t SystemCoreClock = 64000000;

// (Semihosting)
extern void initialise_monitor_handles();

// Reset handler: set the stack pointer and branch to main().
__attribute__( ( naked ) ) void reset_handler( void ) {
  // Set the stack pointer to the 'end of stack' value.
  __asm__( "LDR r0, =_estack\n\t"
           "MOV sp, r0" );
  // Branch to main().
  __asm__( "B main" );
}

/**
 * Main program.
 */
int main( void ) {
  // Copy initialized data from .sidata (Flash) to .data (RAM)
  memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
  // Clear the .bss section in RAM.
  memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

  // Enable floating-point unit.
  SCB->CPACR    |=  ( 0xF << 20 );

  // (Semihosting)
  initialise_monitor_handles();

  // Disable interrupts.
  __disable_irq();

  // Enable GPIOA, GPIOB, GPIOD, GPIOE clocks.
  RCC->AHB4ENR  |=  ( RCC_AHB4ENR_GPIOAEN |
                      RCC_AHB4ENR_GPIOBEN |
                      RCC_AHB4ENR_GPIODEN |
                      RCC_AHB4ENR_GPIOEEN );
  // Enable all internal RAM banks.
  RCC->AHB2ENR  |=  ( RCC_AHB2ENR_AHBSRAM1EN |
                      RCC_AHB2ENR_AHBSRAM2EN );
  // Disable window watchdog clock.
  RCC->APB3ENR  &= ~( RCC_APB3ENR_WWDGEN );
  // Disable Octo-SPI clock.
  RCC->AHB3ENR  &= ~( RCC_AHB3ENR_OSPI1EN );

  // Set D12 to floating input, B1/B2/E11 to high-speed output.
  GPIOB->MODER    &= ~( ( 3 << ( 1 * 2 ) ) |
                        ( 3 << ( 2 * 2 ) ) );
  GPIOB->AFR[ 0 ] &= ~( ( 0xF << ( 1 * 4 ) ) |
                        ( 0xF << ( 2 * 4 ) ) );
  GPIOB->MODER    |=  ( ( 1 << ( 1 * 2 ) ) |
                        ( 1 << ( 2 * 2 ) ) );
  GPIOB->OSPEEDR  |=  ( ( 3 << ( 1 * 2 ) ) |
                        ( 3 << ( 2 * 2 ) ) );
  GPIOD->MODER    &= ~( 3 << ( 12 * 2 ) );
  GPIOD->AFR[ 1 ] &= ~( 0xF << ( ( 12 - 8 ) * 4 ) );
  GPIOD->PUPDR    &= ~( 3 << ( 12 * 2 ) );
  GPIOE->MODER    &= ~( 3 << ( 11 * 2 ) );
  GPIOE->AFR[ 1 ] &= ~( 0xF << ( ( 11 - 8 ) * 4 ) );
  GPIOE->MODER    |=  ( 1 << ( 11 * 2 ) );
  GPIOE->OSPEEDR  |=  ( 3 << ( 11 * 2 ) );
  // Initialize unused IO2/IO3 in 'high' position.
  GPIOA->MODER    &= ~( 3 << ( 1 * 2 ) );
  //GPIOA->MODER    |=  ( 1 << ( 1 * 2 ) );
  //GPIOA->ODR      |=  ( 1 << 1 );
  GPIOE->MODER    &= ~( 3 << ( 2 * 2 ) );
  //GPIOE->MODER    |=  ( 1 << ( 2 * 2 ) );
  //GPIOE->ODR      |=  ( 1 << 2 );
  // Try B12, E3, D4, D8 high.
  GPIOB->MODER    &= ~( 3 << ( 12 * 2 ) );
  GPIOB->MODER    |=  ( 1 << ( 12 * 2 ) );
  GPIOB->ODR      |=  ( 1 << 12 );
  GPIOD->MODER    &= ~( 3 << ( 4 * 2 ) );
  GPIOD->MODER    |=  ( 1 << ( 4 * 2 ) );
  GPIOD->ODR      |=  ( 1 << 4 );
  GPIOD->MODER    &= ~( 3 << ( 8 * 2 ) );
  GPIOD->MODER    |=  ( 1 << ( 8 * 2 ) );
  GPIOD->ODR      |=  ( 1 << 8 );
  GPIOE->MODER    &= ~( 3 << ( 3 * 2 ) );
  GPIOE->MODER    |=  ( 1 << ( 3 * 2 ) );
  GPIOE->ODR      |=  ( 1 << 3 );

  CS_HI();
  SCK_HI();
  MOSI_LO();

  // Pet the independent watchdog.
  IWDG1->KR = 0xAAAA;

  // Attempt to read the status register twice.
  CS_LO();
  sspi_w( 0x05 );
  uint8_t resp = sspi_r();
  CS_HI();
  printf( "%02X\r\n", resp );
  CS_LO();
  sspi_w( 0x05 );
  resp = sspi_r();
  CS_HI();
  printf( "%02X\r\n", resp );

  // disable Quad-I/O if necessary.
  if ( resp & 0x40 ) {
    // Enable writes.
    CS_LO();
    sspi_w( 0x06 );
    CS_HI();
    // Write status/config registers.
    CS_LO();
    sspi_w( 0x01 );
    sspi_w( 0x00 );
    sspi_w( 0x00 );
    CS_HI();
    // Read back the status register a few times.
    for ( int i = 0; i < 4; ++i ) {
      CS_LO();
      sspi_w( 0x05 );
      resp = sspi_r();
      CS_HI();
      printf( "%02X\r\n", resp );
    }
  }

  // Main program.
  while( 1 ) {
    // Pet the independent watchdog.
    IWDG1->KR = 0xAAAA;

    // Pull CS low and send the 'READ' command with an address of 0.
    CS_LO();
    sspi_w( 0x03 );
    // Replace the following byte with '0x01', '0x02', etc. if
    // you are reading parts of the chip at a time.
    sspi_w( 0x00 );
    sspi_w( 0x00 );
    sspi_w( 0x00 );
    uint8_t bb8 = 0;
    // Read out the Flash, one byte at a time.
    //for ( int i = 0; i < ( 1024 * 1024 ); ++i ) {
    // (Or only read a fraction of the Flash at a time,
    //  since there seem to be stability issues)
    for ( int i = 0; i < ( 1024 * 64 ); ++i ) {
      bb8 = sspi_r();
      printf( "%02X", bb8 );
      if ( ( i % 1024 == 0 ) ) {
        // Pet the independent watchdog.
        IWDG1->KR = 0xAAAA;
      }
    }
    // Pull CS high.
    CS_HI();

    // Done.
    printf( "\r\nDone\r\n" );
    while ( 1 ) { __asm__( "nop" ); }
  }
  return 0; // lol
}
