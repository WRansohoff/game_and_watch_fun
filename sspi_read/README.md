# Game and Watch Test RAM Firmware

This is a minimal program designed to read the contents of the Flash chip in a Nintendo Game and Watch.

It uses software-driven SPI, which is much slower than using the OCTOSPI peripheral. But I haven't been able to read out the Flash memory in OCTOSPI memory-mapped mode yet. I'm probably forgetting a flag somewhere, but it's also possible that that internal memory region is protected by the chip's readout protection.

After performing a hard reset, the SPI Flash chip seems to stop responding. I'm not sure why that happens yet, but if you want to try running this, you should do so immediately after connecting to the device, without performing a full reset.

The SPI Flash data is printed out in a stream of hex digits over the ARM Cortex-M "semihosting" print output. It seems to work, but I haven't tried to analyze or verify the output yet. It'll take a few minutes to run, because there are 1024 * 1024 * 2 = 2097152 hex digits to print.

When I run this, the program usually crashes approximately 1/8 of the way through, printing:

stm32h7x.cpu0 -- clearing lockup after double fault

Presumably this is caused by the same sort of artefact of the stock firmware that causes the SPI Flash chip to reject commands after a hard reset. Or maybe it's because I leave the battery unplugged and the chip browns out after a period of high activity from the power that the USB cable can supply. I'm not sure, but you can run it 8 times with different starting addresses if that happens to you.

This repository **DOES NOT** contain the actual contents of the Flash chip. It's definitely okay to back up this sort of data for your own use (in the US), but I feel like it might not be okay to publish it on GitHub.
