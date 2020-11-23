# Game and Watch Backlight Test: GPIO Control

This is a simple test firmware for the Nintendo Game and Watch.

It demonstrates how to toggle the display's backlight LEDs; there are three of them.

# Building and Running

This program can be build and flashed like a normal bare-metal STM32 program.

Run `make` to build the program using the `arm-none-eabi-gcc` toolchain, and flash it using OpenOCD / ST-util / etc.

The Game and Watch ships with readout protection enabled, so you can only load programs like this after wiping the stock firmware and unlocking the chip (`stm32h7x unlock 0` in an OpenOCD console). Note that unlocking the chip **PERMANENTLY** erases the stock firmware. You won't be able to do a "factory reset" unless you back up the original internal Flash bank first.
