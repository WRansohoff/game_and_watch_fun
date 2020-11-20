/* Label for the program's entry point */
ENTRY(reset_handler)

/* End of RAM/Start of stack. Use the RAMA3 bank. */
_estack = 0x240A0000;

MEMORY
{
    FLASH    ( rx )    : ORIGIN = 0x08000000, LENGTH = 128K
    RAM_BKP  ( rxw )   : ORIGIN = 0x38800000, LENGTH = 4K
    SRDRAM   ( rxw )   : ORIGIN = 0x38000000, LENGTH = 32K
    SRAM2    ( rxw )   : ORIGIN = 0x30010000, LENGTH = 64K
    SRAM1    ( rxw )   : ORIGIN = 0x30000000, LENGTH = 64K
    RAMA3    ( rxw )   : ORIGIN = 0x240A0000, LENGTH = 384K
    RAMA2    ( rxw )   : ORIGIN = 0x24040000, LENGTH = 384K
    RAM      ( rxw )   : ORIGIN = 0x24000000, LENGTH = 256K
    DTCMRAM  ( rxw )   : ORIGIN = 0x20000000, LENGTH = 128K
    ITCMRAM  ( rxw )   : ORIGIN = 0x00000000, LENGTH = 64K
}

INCLUDE "sections.ld"
