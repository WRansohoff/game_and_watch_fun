SECTIONS
{
  /* The vector table goes to the start of flash. */
  .vector_table :
  {
    . = ALIGN(4);
    KEEP (*(.vector_table))
    . = ALIGN(4);
  } >FLASH

  /* The 'text' section contains the main program code. */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    KEEP (*(.init))
    KEEP (*(.fini))
    KEEP (*(.eh_frame))
    . = ALIGN(4);
  } >FLASH

  /* Sections required by the standard libraries. */
  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH
  .ARM :
  {
    *(.ARM.exidx*)
  } >FLASH

  /* The 'rodata' section contains read-only data,
   * constants, strings, information that won't change. */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* The 'data' section is space set aside in RAM for
   * things like variables, which can change. */
  _sidata = .;
  .data : AT(_sidata)
  {
    . = ALIGN(4);
    /* Mark start/end locations for the 'data' section. */
    _sdata = .;
    *(.data)
    *(.data*)
    _edata = .;
    . = ALIGN(4);
  } >RAM

  /* The 'bss' section is similar to the 'data' section,
   * but its space is initialized to all 0s at the
   * start of the program. */
  .bss :
  {
    . = ALIGN(4);
    /* Also mark the start/end of the BSS section. */
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

  /* ITCM RAM region.
   * Tightly-coupled memory on the instruction bus. */
  _siitcm = .;
  .itcm : AT(_siitcm)
  {
    . = ALIGN(4);
    _sitcm = .;
    KEEP (*(.itcm_irqs))
    . = ALIGN(4);
    _eitcm = .;
  } >ITCMRAM
  . = _siitcm + ( _eitcm - _sitcm );

  /* DTCM RAM region.
   * Tightly-coupled memory on the data bus. */
  _sidtcm = .;
  .dtcm : AT(_sidtcm)
  {
    . = ALIGN(4);
    _sdtcm = .;
    KEEP (*(.dtcm_vars))
    . = ALIGN(4);
    _edtcm = .;
  } >DTCMRAM
  . = _sidtcm + ( _edtcm - _sdtcm );

  /* Mark the end of statically-allocated RAM. */
  end = .;
  _end = end;
  __end = end;
}
