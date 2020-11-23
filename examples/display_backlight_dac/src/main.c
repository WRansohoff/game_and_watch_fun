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
  // Enable GPIOA, DAC1, DAC2 peripherals.
  RCC->AHB4ENR  |=  ( RCC_AHB4ENR_GPIOAEN );
  RCC->APB1LENR |=  ( RCC_APB1LENR_DAC12EN );
  RCC->APB4ENR  |=  ( RCC_APB4ENR_DAC2EN );

  // Copy initialized data from .sidata (Flash) to .data (RAM)
  memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
  // Clear the .bss section in RAM.
  memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

  // Enable floating-point unit.
  SCB->CPACR    |=  ( 0xF << 20 );

  // Configure SysTick to trigger every ms.
  SysTick_Config( SystemCoreClock / 1000 );

  // Set PA4, PA5, PA6 to analog mode.
  GPIOA->MODER |=  ( ( 3 << ( 4 * 2 ) ) |
                     ( 3 << ( 5 * 2 ) ) |
                     ( 3 << ( 6 * 2 ) ) );

  // Configure DAC1_OUT1, DAC1_OUT2, DAC2_OUT1.
  DAC1->DHR12R1 = 0x000;
  DAC1->DHR12R2 = 0x000;
  DAC2->DHR12R1 = 0x000;
  DAC1->CR     |=  ( DAC_CR_EN1 | DAC_CR_EN2 );
  DAC2->CR     |=  ( DAC_CR_EN1 );

  // Done; pulse the display between dark and light.
  // Below about 0x600-0x700ish, the display looks black.
  int x = 0x600;
  int dir = 1;
  while( 1 ) {
    // Set DAC outputs.
    // There are 3 backlight LEDs: left, right, and center.
    DAC1->DHR12R1 = ( x & 0xFFF );
    DAC1->DHR12R2 = ( x & 0xFFF );
    DAC2->DHR12R1 = ( x & 0xFFF );

    // Update brightness value.
    x += dir;
    if ( x > 0xFFF ) { dir = -1; }
    else if ( x < 0x600 ) { dir = 1; }
    delay_ms( 1 );
  }
  return 0; // lol
}
