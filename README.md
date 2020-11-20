# Nintendo Game and Watch Investigations

Since @ghidraninja pointed out that these devices use an STM32H7 chip with an exposed debugging port, I decided to try playing around with them.

They have readout protection level 1 enabled, but it looks like they probably execute code off of their external SPI Flash chip, so it shouldn't be too hard to circumvent that and dump the firmware.

For now, there's only a minimal program to run on the device without erasing its Flash memory, but I'm hoping to add a program for dumping the contents of the SPI Flash chip before long.

In the meantime, I've been poking at the stock firmware's registers to see how the pins are connected and how the peripherals are configured. Here's what I've found so far:

# Pin Mappings and Peripheral Configurations

GPIOx: 0x58020000 + ( 0x400 * x )
MODER: +0x00
OTYPER: +0x04
OSPEEDR: +0x08
PUPDR: +0x0C
AFRL: +0x20
AFRH: +0x24

MODER = pin mode
0 = Floating input
1 = Output
2 = Alternate Function (Peripheral)
3 = Analog (ADC/DAC)

OTYPER = output type
0 = Push-Pull
1 = Open-Drain

OSPEEDR ~= "drive strength"
0 = Low
1 = Medium
2 = High
3 = Very High

PUPDR = pulling resistors
0 = None
1 = Pull-up
2 = Pull-down

AFRL/AFRH = alternate function mapping.

GPIOA:   0x58020000
MODER:   0xABAABFC8
OTYPER:  0x00000000
OSPEEDR: 0x0C000008
PUPDR:   0x64000001
AFRL:    0xE0000090
AFRH:    0x0000ECEE

GPIOB:   0x58020400
MODER:   0xA9AEABEA
OTYPER:  0x000000C0
OSPEEDR: 0x000000F8
PUPDR:   0x00000100
AFRL:    0x44B009B9
AFRH:    0x5E50EE0E

GPIOC:   0x58020800
MODER:   0xF3EBA3F2
OTYPER:  0x00000000
OSPEEDR: 0x00000000
PUPDR:   0x04000404
AFRL:    0xEE00000B
AFRH:    0x00000EA0

GPIOD:   0x58020C00
MODER:   0x0E21E1A4
OTYPER:  0x00000000
OSPEEDR: 0x02000000
PUPDR:   0x50440401
AFRL:    0x0E00E900
AFRH:    0x00090E00

GPIOE:   0x58021000
MODER:   0xBBBD2A6F
OTYPER:  0x00000000
OSPEEDR: 0x00400020
PUPDR:   0x00004000
AFRL:    0x06660900
AFRH:    0xE0E0B000

GPIOF-GPIOK regs: 0xFFFFFFFF

OCTOSPI1:
CR: +0x00: 0x30400301

Yay! The QSPI Flash chip is in memory-mapped mode! That should let us execute arbitrary code and get around the readout portection.
