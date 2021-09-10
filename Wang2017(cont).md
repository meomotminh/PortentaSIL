### Call Assembly Function from C

calling convention of C, the main() function passes the first 4 parameters a,b,c,d in r0-r3 and the remaining parameter e,f on stack. Upon entry to the called function, the stack top contains parameter e,f in the order of increasing addresses. Called function first establish the stack frame establish the stack frame by saving LR, FP on stack and letting FP(r12) point at the save LR. e and f are now at FP + 4 and FP + 8

### Call C Function from Assembly

ts.o: In function `start':
(.text+0x14): undefined reference to `sum'

### Inline Assembly

__asm__("assembly code"); or simply asm("assembly code");

asm("move %r0, %r1\n\t; add %r0,#10,r0\n\t");

Inline assembly code can also specify operands. template of such inline assembly code is
    asm ( assembler template
        : output operands
        : input operands
        : list of clobbered registers
    );

Assembly statements may specify output and input operands, referenced as %0, %1.

    int a, b = 10;
    asm("mov %1, %%r0; move %%r0,%0;" // use %%REG for register
        :"=r" (a)                     // output MUST have =
        :"r"  (b)                     // input 
        :"%r0"                        // clobbered registers  
    )
*****************************
    %0 refers to a, %1 refers to b, %%r0 refer to r0 register
    constraint operator "r" means to use a register for the operand


#### Device  Drivers
the emulated ARM Versatilepb board is a virtual machine, behaves just like real hardware system but no drivers for the emulated peripheral devices -> must implement device drivers to support basic I/O operations: UART serial ports, timers, LCD display, keyboard and multimedia SD card. practical device driver should use interrupts.

##### System Memory Map
ARM system architecture uses memory-mapped-I/O. each I/O device is assigned a block of contiguous memory in the system memory map. Internal registers of each I/O device are accessed as offsets from the device base address

##### GPIO Programming
most ARM based system boards provide GPIO pins as I/O interface to the system. Some GPIOs pin can be configured for inputs, other as outputs. A GPIO interface, e.g. LPC2129 GPIO MCU used in many early embedded system boards, consist of 4 32-bit registers
    - GPIODIR: set pin direction; 0 for input, 1 for output
    - GPIOSET: set pin voltage level to high (3.3 V)
    - GPIOCLR: set pin voltage level to low (0 V)
    - GPIOPIN: read this register returns the states of all pins


*********************Memory Map of ARM versatile****************************
/*
    MPMC Chip Select 0, 128 MB SRAM                 0x00000000          128 MB
    MPMC Chip Select 1, 128 MB expansion SRAM       0x08000000          128 MB
    System registers                                0x10000000          4 KB
    Secondary Interrupt Controller (SIC)            0x10003000          4 KB
    Multimedia Card Interface 0 (MMCID)             0x10005000          4 KB
    Keyboard/Mouse Interface 0 (Keyboard)           0x10006000          6 KB
    Reserved (UART3 Interface)                      0x10009000          4 KB
    Ethernet Interface                              0x10010000          64 KB
    USB Interface                                   0x10020000          64 KB
    Color LCD Controller                            0x10120000          64 KB
    DMA Controller                                  0x10130000          64 KB
    Vectored Interrupt Controller (PIC)             0x10140000          64 KB
    System Controller                               0x101E0000          4 KB
    Watchdog Interface                              0x101E1000          4 KB
    Timer modules 0 and 1 interface                 0x101E2000          4 KB
    (Timer 1 at 0x101E2020)                         0x101E2FFF
    Timer modules 2 and 3 interface                 0x101E3000          4 KB
    (Timer 3 at 0x101E3020)                         0x101E3FFF
    GPIO Interface (port 0)                         0x101E4000          4 KB
    GPIO Interface (port 1)                         0x101E5000          4 KB
    GPIO Interface (port 2)                         0x101E6000          4 KB
    UART 0 Interface                                0x101E1000          4 KB
    UART 1 Interface                                0x101E2000          4 KB
    UART 2 Interface                                0x101F3000          4 KB
    SSMC static expansion memory                    0x20000000          256 MB
*/

GPIO registers can be assessed as word offsets from a (memory mapped) base address. In GPIO registers, each bit corresponds to a GPIO pin. Depend on the direction setting in the IODIR, each pin can be connected to an appropriate I/O device.
As specific example, assume we want use GPIO pin0 for input, connected to a (de-bounced switch), and pin1 for output, connected to the (ground side) of an LED with its own 3.3V voltage source and a current-limiting resistor:
    GPIODIR: bit0 = 0 (input)
    GPIOSET: all bits = 0 (no pin high)
    GPIOCLR: bit1 = 1 (set to LOW)
    GPIOPIN: read pin state, check pin0 for input

GPIO interfaces are arranged in separate groups called ports (Port0 - Port2), 0x101E4000 - 0x101E6000
Each port provides 8 GPIO pins, controlled by 8-bit GPIODIR register and 8-bit GPIODATA register

#### UART Driver for Serial I/O
relying on QEMU monitor command to display register and memory contents is tedious -> simple UART driver for I/O on emulated serial terminal. The ARM Versatile board support 4 PL011 UART devices for Serial I/O. each UART device has base address in the system memory map:
    UART0: 0x101F1000
    UART1: 0x101F2000
    UART2: 0x101F3000
    UART3: 0x10090000
byte offsets of registers from the base address:
    0x00    UARTDR  Data register: read/write chars
    0x18    UARTFR  Flag register: TxEmpty, RxFull, etc
    0x24    UARIBRD Baud rate register: set baud rate
    0x2C    UARTLCR Line Control register: bits per char, parity,...
    0x38    UARTIMIS Interrupt mask register for TX and RX interrupts

Steps:
    - write a divisor value to the baud rate register for desired baud rate
        0x4 = 1152000, 0xC = 38400, 0x18 = 192000, 0x20 = 14400, 0x30 = 9600
    - write to Line Control register to specify number of bits per char and parity, e.g. 8 bits per char with no parity
    - write to Interrupt Mask register to enable/disable RX and TX interrupts

QEMU auto use default values for baud rate and line control parameters -> program Interrupt Mask register (if use interrupt) and check Flag register during serial I/O. implement UART I/O by polling, only check the Flag status register. need assembly code to access CPU registers adn interface hardware. However we shall use assembly code only if absolutely necessary. UART driver:

    (1) ts.s file: when ARM CPU starts, it is in Supervisor or SVC mode. the ts.s file set SVC mode stack pointer and call main() in C


#### Demo UART driver
