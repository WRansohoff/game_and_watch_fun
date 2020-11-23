# Game and Watch Backlight Test: DAC Control

This is a simple test firmware for the Nintendo Game and Watch.

It demonstrates how to use the DAC peripherals to adjust the display's brightness.

This is interesting and kind of surprising: it's the first time that I've seen a DAC (Digital-Analog Converter) peripheral used to control an LED's brightness. I was expecting to see a timer peripheral's PWM (Pulse-Width Modulation) mode used for controlling the backlight, but there's no timer output connected to pin A4, and these pins are configured in "analog" mode in the stock firmware.

DAC peripherals are kind of expensive and rare, and they don't usually have much drive strength on the output, so it seems a bit quirky to use them like this. On the bright side, the DAC peripherals are very easy to write code for.

# Building and Running

This program can be build and flashed like a normal bare-metal STM32 program.

Run `make` to build the program using the `arm-none-eabi-gcc` toolchain, and flash it using OpenOCD / ST-util / etc.

The Game and Watch ships with readout protection enabled, so you can only load programs like this after wiping the stock firmware and unlocking the chip (`stm32h7x unlock 0` in an OpenOCD console). Note that unlocking the chip **PERMANENTLY** erases the stock firmware. You won't be able to do a "factory reset" unless you back up the original internal Flash bank first.
