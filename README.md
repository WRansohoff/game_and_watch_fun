# Nintendo Game and Watch Investigations 
Since @ghidraninja pointed out that these devices use an STM32H7 chip with an exposed debugging port, I decided to try playing around with them.

They have readout protection level 1 enabled, but it looks like they probably execute code off of their external SPI Flash chip, so it shouldn't be too hard to back up the firmware for your personal use.

For now, there's only a minimal program to run on the device without erasing its Flash memory, but I'm hoping to add a program for dumping the contents of the SPI Flash chip before long.

In the meantime, I've been poking at the stock firmware's registers to see how the pins are connected and how the peripherals are configured. Here's what I've found so far:

# Fitting a Debug Connector

This involves a little bit of fine soldering, but I think it's worthwhile to add a debug port if you're going to be reprogramming this board very often. From my perspective, it's one of the most attractively-priced STM32H7 development kits that I've ever seen. A display, battery, *and* case for $50? Nice.

Fortunately, there's a bit of room between the speaker and the battery for a small connector. I used a 2mm-pitch row of pins, but you could probably fit 0.1" headers, especially if you omit the `V++` positive voltage pin.

![Connector Placement](img/gw_connector_dremel.png)

**BE SURE TO DISCONNECT THE BATTERY BEFORE SOLDERING ANYTHING!** The `V++` pin is right next to the `GND` one, and not shorting power lines together is a good rule of thumb. You can pluck the connector with red and white wires off of the board; it "clicks" in and out of the board's plastic connector from the top.

You can still power the device from the USB-C port while the battery is disconnected, so it's not a bad idea to leave it unplugged while you are debugging the device. That way, un-plugging the USB cable turns into an easy way to power-cycle the board.

You can probe the pins with a multimeter [like @ghidraninja did](https://www.youtube.com/watch?v=Rsi8p5gbaps), but since these mappings are public knowledge now:

![Connector Wiring](img/gw_conn_wires.png)

Enameled "magnet wire" isn't a bad choice for these connections, because the holes are so small and it's easy to work with. (It's not magnetic, it's just usually used in winding transformers.)

Once you've soldered everything together, you can glue and/or tape everything in place:

![Connector Assembly](img/gw_conn_soldered.png)

The way I glued this together, the two halves of the case can't move too far apart. I think it works okay, but you might want to glue the connector to the other side of the housing if that would bother you. Once you've verified that you can open an OpenOCD connection using the connector, snap the case back together and clean up any loose edges:

![Connector in Case](img/gw_connector_fixed.png)

# Pin Mappings and Peripheral Configurations

```
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

OCTOSPI1:  0x52005000
CR: +0x00: 0x30400301
```

# Tentative Pin Mapping Table

| Pin |    Mode    |   OType    | OSpeed |   PUPD    | Alt. Func. |
|:---:|:----------:|:----------:|:------:|:---------:|:----------:|
| A0  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| A1  | Alt. Func. | Push-Pull  | High   | None      | 9          |
| A2  | Input      | N/A        | N/A    | None      | 0          |
| A3  | Analog     | N/A        | N/A    | None      | 0          |
| A4  | Analog     | N/A        | N/A    | None      | 0          |
| A5  | Analog     | N/A        | N/A    | None      | 0          |
| A6  | Analog     | N/A        | N/A    | None      | 0          |
| A7  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| A8  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| A9  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| A10 | Alt. Func. | Push-Pull  | Low    | None      | 12         |
| A11 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| A12 | Analog     | N/A        | N/A    | None      | 0          |
| A13 | Alt. Func. | Push-Pull  | V.High | Pull-Up   | 0          |
| A14 | Alt. Func. | Push-Pull  | Low    | Pull-Down | 0          |
| A15 | Alt. Func. | Push-Pull  | Low    | Pull-Up   | 0          |
| B0  | Alt. Func. | Push-Pull  | High   | None      | 9          |
| B1  | Alt. Func. | Push-Pull  | V.High | None      | 11         |
| B2  | Alt. Func. | Push-Pull  | V.High | None      | 9          |
| B3  | Analog     | N/A        | N/A    | None      | 0          |
| B4  | Analog     | N/A        | N/A    | Pull-Up   | 0          |
| B5  | Alt. Func. | Push-Pull  | Low    | None      | 11         |
| B6  | Alt. Func. | Open-Drain | Low    | None      | 4          |
| B7  | Alt. Func. | Open-Drain | Low    | None      | 4          |
| B8  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| B9  | Analog     | N/A        | N/A    | None      | 0          |
| B10 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| B11 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| B12 | Output     | Push-Pull  | Low    | None      | 0          |
| B13 | Alt. Func. | Push-Pull  | Low    | None      | 5          |
| B14 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| B15 | Alt. Func. | Push-Pull  | Low    | None      | 5          |
| C0  | Alt. Func. | Push-Pull  | Low    | None      | 11         |
| C1  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| C2  | Analog     | N/A        | N/A    | None      | 0          |
| C3  | Analog     | N/A        | N/A    | None      | 0          |
| C4  | Analog     | N/A        | N/A    | None      | 0          |
| C5  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| C6  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| C7  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| C8  | Analog     | N/A        | N/A    | None      | 0          |
| C9  | Alt. Func. | Push-Pull  | Low    | None      | 10         |
| C10 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| C11 | Analog     | N/A        | N/A    | None      | 0          |
| C12 | Analog     | N/A        | N/A    | None      | 0          |
| C13 | Input      | N/A        | N/A    | Pull-Up   | 0          |
| C14 | Analog     | N/A        | N/A    | None      | 0          |
| C15 | Analog     | N/A        | N/A    | None      | 0          |
| D0  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| D1  | Output     | Push-Pull  | Low    | None      | 0          |
| D2  | Alt. Func. | Push-Pull  | Low    | None      | 9          |
| D3  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| D4  | Output     | Push-Pull  | Low    | None      | 0          |
| D5  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| D6  | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| D7  | Analog     | N/A        | N/A    | None      | 0          |
| D8  | Output     | Push-Pull  | Low    | None      | 0          |
| D9  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| D10 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| D11 | Input      | N/A        | N/A    | Pull-Up   | 0          |
| D12 | Alt. Func. | Push-Pull  | High   | None      | 9          |
| D13 | Analog     | N/A        | N/A    | None      | 0          |
| D14 | Input      | N/A        | N/A    | Pull-Up   | 0          |
| D15 | Input      | N/A        | N/A    | Pull-Up   | 0          |
| E0  | Analog     | N/A        | N/A    | None      | 0          |
| E1  | Analog     | N/A        | N/A    | None      | 0          |
| E2  | Alt. Func. | Push-Pull  | High   | None      | 9          |
| E3  | Output     | Push-Pull  | Low    | None      | 0          |
| E4  | Alt. Func. | Push-Pull  | Low    | None      | 6          |
| E5  | Alt. Func. | Push-Pull  | Low    | None      | 6          |
| E6  | Alt. Func. | Push-Pull  | Low    | None      | 6          |
| E7  | Input      | N/A        | N/A    | Pull-Up   | 0          |
| E8  | Output     | Push-Pull  | Low    | None      | 0          |
| E9  | Analog     | N/A        | N/A    | None      | 0          |
| E10 | Analog     | N/A        | N/A    | None      | 0          |
| E11 | Alt. Func. | Push-Pull  | Medium | None      | 11         |
| E12 | Analog     | N/A        | N/A    | None      | 0          |
| E13 | Alt. Func. | Push-Pull  | Low    | None      | 14         |
| E14 | Analog     | N/A        | N/A    | None      | 0          |
| E15 | Alt. Func. | Push-Pull  | Low    | None      | 14         |

Yay! The QSPI Flash chip is in memory-mapped mode! That should let us execute arbitrary code and get around the readout portection.
