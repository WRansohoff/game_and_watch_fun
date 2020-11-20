# Game and Watch Test RAM Firmware

This is a minimal program designed to read the contents of the Flash chip in a Nintendo Game and Watch.

It uses software-driven SPI, which is much slower than using the OCTOSPI peripheral. But I haven't been able to read out the Flash memory in OCTOSPI memory-mapped mode yet. I'm probably forgetting a flag somewhere, but it's also possible that that internal memory region is protected by the chip's readout protection.

Currently, it only reads out the SPI Flash status register in an endless loop. It appears to read `0x40`, which indicates that quad-SPI mode is enabled in the stock firmware.

After performing a hard reset, the SPI Flash chip seems to stop responding. I'm not sure why that happens yet, but if you want to try running this, you should do so before immediately after connecting to the device, without performing a full reset.
