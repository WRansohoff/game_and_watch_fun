# Game and Watch Test RAM Firmware

This is a minimal program designed to run in RAM on the STM32H7B0 chip inside of a Nintendo Game and Watch.

It is a starting point for playing with the platform without erasing its stock firmware.

# Building and Running

To build the firmware image, just run `make` in this directory. You will need to have Make installed, along with the bare-metal ARM toolchain (`arm-none-eabi-gcc`).

The resulting `main.elf` file can be used by OpenOCD. Navigate to the `openocd/` directory in this repository's top-level directory, and open a connection with your chosen debugger:

J-Link:

    openocd -f jlink.cfg -c "transport select swd" -f stm32h7x_dual_bank.cfg -c "gdb_memory_map disable" -c init -c "arm semihosting enable" -c halt

ST-Link:

    openocd -f stlink.cfg -f stm32h7x_dual_bank.cfg -c "gdb_memory_map disable"

In a separate terminal, open the compiled `main.elf` file in GDB:

    arm-none-eabi-gdb main.elf

Connect to the OpenOCD interface on port 3333, and debug the program normally:

    (gdb) target extended-remote :3333
    Remote debugging using :3333
    0xfffffffe in ?? ()
    (gdb) load
    Loading section .text, size 0x254 lma 0x0
    Start address 0x134, load size 596
    Transfer rate: 4768 bits in <1 sec, 596 bytes/write.
    (gdb) bt
    #0  reset_handler () at src/main.c:18
    (gdb) c
    Continuing.
    ^C
    Program received signal SIGINT, Interrupt.
    0x00000184 in main () at src/main.c:41
    41	    ++y;
    (gdb) p y
    $1 = 29431371
    (gdb) p x
    $2 = 42

From here, it shouldn't take too long to dump the contents of the SPI Flash chip (fingers crossed).
