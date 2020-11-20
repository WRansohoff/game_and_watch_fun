// Project includes.
#include "global.h"
#include "qspi.h"

// Core system clock speed.
uint32_t SystemCoreClock = 64000000;

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

  // Enable OCTOSPI peripheral clocks: GPIOA,B,D,E, OCTOSPI1.
  RCC->AHB3ENR  |=  ( RCC_AHB3ENR_OSPI1EN );
  RCC->AHB4ENR  |=  ( RCC_AHB4ENR_GPIOAEN |
                      RCC_AHB4ENR_GPIOBEN |
                      RCC_AHB4ENR_GPIODEN |
                      RCC_AHB4ENR_GPIOEEN );

  // Configure octo-SPI pins:
  // PA1[9], PB1[11], PB2[9], PD12[9], PE2[9], PE11[11].
  GPIOA->MODER    &= ~( 0x3 << ( 1 * 2 ) );
  GPIOA->MODER    |=  ( 0x2 << ( 1 * 2 ) );
  GPIOA->OSPEEDR  |=  ( 0x2 << ( 1 * 2 ) );
  GPIOA->AFR[ 0 ] |=  ( 0x9 << ( 1 * 4 ) );
  GPIOB->MODER    &= ~( ( 0x3 << ( 1 * 2 ) ) |
                        ( 0x3 << ( 2 * 2 ) ) );
  GPIOB->MODER    |=  ( ( 0x2 << ( 1 * 2 ) ) |
                        ( 0x2 << ( 2 * 2 ) ) );
  GPIOB->OSPEEDR  |=  ( ( 0x3 << ( 1 * 2 ) ) |
                        ( 0x3 << ( 2 * 2 ) ) );
  GPIOB->AFR[ 0 ] |=  ( ( 0xB << ( 1 * 4 ) ) |
                        ( 0x9 << ( 2 * 4 ) ) );
  GPIOD->MODER    &= ~( 0x3 << ( 2 * 2 ) );
  GPIOD->MODER    |=  ( 0x2 << ( 2 * 2 ) );
  GPIOD->OSPEEDR  |=  ( 0x2 << ( 2 * 2 ) );
  GPIOD->AFR[ 0 ] |=  ( 0x9 << ( 2 * 4 ) );
  GPIOE->MODER    &= ~( ( 0x3 << ( 2 * 2 ) ) |
                        ( 0x3 << ( 11 * 2 ) ) );
  GPIOE->MODER    |=  ( ( 0x2 << ( 2 * 2 ) ) |
                        ( 0x2 << ( 11 * 2 ) ) );
  GPIOE->OSPEEDR  |=  ( ( 0x2 << ( 2 * 2 ) ) |
                        ( 0x1 << ( 11 * 2 ) ) );
  GPIOE->AFR[ 0 ] |=  ( 0x9 << ( 2 * 4 ) );
  GPIOE->AFR[ 1 ] |=  ( 0xB << ( ( 11 - 8 ) * 4 ) );

  // Configure Octo-SPI peripheral.
  // TODO.....

  // Main program.
  while( 1 ) {
    // TODO
  }
  return 0; // lol
}
