// Device header file.
#include "stm32h7xx.h"
// Standard library includes.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _siitcm, _sidtcm, _sitcm, _sdtcm, _eitcm, _edtcm, _estack;

// Core system clock speed.
uint32_t SystemCoreClock = 64000000;
// Global systick counter.
volatile uint32_t systick = 0;

// Reset handler: set the stack pointer and branch to main().
__attribute__( ( naked ) ) void reset_handler( void ) {
  // Set the stack pointer to the 'end of stack' value.
  __asm__( "LDR r0, =_estack\n\t"
           "MOV sp, r0" );
  // Branch to main().
  __asm__( "B main" );
}

// SysTick interrupt handler.
void SysTick_IRQn_handler( void ) {
  ++systick;
}

// Simple blocking millisecond delay method.
void delay_ms( uint32_t ms ) {
  // Calculate the 'end of delay' tick value, then wait for it.
  uint32_t next = systick + ms;
  while ( systick < next ) { __WFI(); }
}

/**
 * Main program.
 */
int main( void ) {
  // Enable all RAM banks.
  RCC->AHB2ENR  |=  ( RCC_AHB2ENR_AHBSRAM1EN |
                      RCC_AHB2ENR_AHBSRAM2EN );
  RCC->AHB4ENR  |=  ( RCC_AHB4ENR_BKPRAMEN |
                      RCC_AHB4ENR_SRDSRAMEN );
  // Enable GPIOA peripheral.
  RCC->AHB4ENR  |=  ( RCC_AHB4ENR_GPIOAEN );

  // Copy initialized data from .sidata (Flash) to .data (RAM)
  memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
  // Clear the .bss section in RAM.
  memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

  // Enable floating-point unit.
  SCB->CPACR    |=  ( 0xF << 20 );

  // Configure SysTick to trigger every ms.
  SysTick_Config( SystemCoreClock / 1000 );

  // Set PA4, PA5, PA6 to output mode.
  GPIOA->MODER &= ~( ( 3 << ( 4 * 2 ) ) |
                     ( 3 << ( 5 * 2 ) ) |
                     ( 3 << ( 6 * 2 ) ) );
  GPIOA->MODER |=  ( ( 1 << ( 4 * 2 ) ) |
                     ( 1 << ( 5 * 2 ) ) |
                     ( 1 << ( 6 * 2 ) ) );
  // Turn all three backlight LEDs off.
  GPIOA->ODR   &= ~( ( 1 << 4 ) | ( 1 << 5 ) | ( 1 << 6 ) );

  // Done; toggle the backlight LEDs on and off in a pattern.
  int x = 0;
  while( 1 ) {
    delay_ms( 500 );
    GPIOA->ODR  =  ( ( ( x & 0x1 ) << 4 ) |
                     ( ( ( x & 0x2 ) >> 1 ) << 5 ) |
                     ( ( ( x & 0x4 ) >> 2 ) << 6 ) );
    ++x;
  }
  return 0; // lol
}
