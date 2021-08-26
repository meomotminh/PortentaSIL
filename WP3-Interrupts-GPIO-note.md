
| Name              |       Pin   | Note                  |
|------             |-----        |-----                  |                     
| PH_15             | GPIO_PIN_15 |                       |  
| D2N               | GPIO_PIN_14 |                       |  
| TX CAN1 (inner)   | GPIO_PIN_13 |                       |  
| D1P,SDA2          | GPIO_PIN_12 |                       |
| D1N,SCL2          | GPIO_PIN_11 |                       |  
| D0P               | GPIO_PIN_10 |                       |  
| D0N               | GPIO_PIN_9  |                       |
| PH_8,SDA0         | GPIO_PIN_8  |                       |  
| PH_7,SCL0         | GPIO_PIN_7  |                       |  
| PWM 9             | GPIO_PIN_6  |                       |  
|                   | GPIO_PIN_5  | *SPI5_NSS, I2C2_SDA   |  
|                   | GPIO_PIN_4  | *ADC3_INN15/ADC3_INP16, I2C_SCL, DCMI_D4 -->> Defect Serial Monitor|
|                   | GPIO_PIN_3  | *ADC3_INN14/ADC3_INP15, FDCAN1_TX-TX/ETH_COL, SAI2_MCLK-B/LTDC_R1|
|                   | GPIO_PIN_2  | *ADC3_INN13/ADC3_INP14, ETH_CRS, SAI2_SCK-B/LTDC_R0/DCMI_D3|
|                   | GPIO_PIN_1  | *ADC3_INP13, PH1-OSC_OUT (STANDBY)|
|                   | GPIO_PIN_0  | *PH0-OSC_IN, DCMI_D1 (REFCLK)|


## Getting started with STM32H74 hardware development

### Power Supplies
- require a 1.71 to 3.6V operating voltage supply (V_DD) which can be reduced down to 1.62V by using external power supervisor & connecting PDR_ON pin to V_SS

### STM32H745/755 advanced Arm-based 32-bit MCUs
#### Introduction
- each GPIO port has four 32-bit configuration registers (GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR and GPIOx_PUPDR), two 32-bit data registers (GPIOx_IDR and GPIOx_ODR) and a 32-bit set/reset register (GPIOx_BSRR).
- in addition all GPIOs have a 32-bit locking register (GPIOx_LCKR) and 2 32-bit **alternate function** selection register (GPIOx_AFRH and GPIOx_AFRL)

#### GPIO main features
- output states: push-pull or open drain + pull-up/down
- output data from output data register (GPIOx_ODR) or peripheral (alternate function output)
- Speed selection for each I/O
- input states: floating, pull-up/down, analog
- input data to input data register (GPIOx_IDR) or peripheral (alternate function input)
- Bit set and reset register (GPIOx_BSRR) for bitwise write access to GPIOx_ODR
- Locking mechanism (GPIOx_LCKR) provided to freeze the I/O port configurations
- Analog function
- Alternate function selection registers
- Fast toggle capable of changing every  clock cycles
- highly flexible pin multiplexing allows the use of I/O pins as GPIOs or as 1 of several peripheral functions

#### GPIO functional description
- subject to the specific hardware characteristics of each I/O port listed in the datasheet, each port bit of the general-purpose I/O (GPIO) ports can be individually configured by software in several modes
    - input floating
    - input pull-up
    - input-pull-down
    - analog
    - output open-drain with pull-up or pull-down capability
    - output push-pull with pull-up or pull-down capability
    - alternate function push-pull with pull-up or pull-down capability
    - alternate function open-drain with pull-up or pull-down capability
- Each I/O port bit is freely programmable, however the I/O port registes have to be accessed as 32-bit words, half-words or bytes. Purpose of GPIOx_BSRR register is to allow atomic read/modify accesses to any of the GPIOx_ODR registers. in this way, there is no risk of an IRQ occuring between the read and the modify access

 ### GPIO 
 - During and just after reset, the alternate functions are not active and most of the I/O ports are configured in analog mode
 - debug pins are in AF pull-up/pull-down after reset:
    - PA15: JTDI in pull-up
    - PA14: JTCK/SWCLK in pull-down
    - PA13: JTMS/SWDAT in pull-up
    - PB4: NJTRST in pull-up
    - PB3: JTDO in floating state
 - when the pin is configured as output, value written to output data register (GPIOx_ODR) is output on the I/O pin. It is possible to use the output driver in push-pull mode or open-drain mode (only the low level is driven, high level is HI-Z)
 - the input data register (GPIOx_IDR) captures the data present on the I/O pin at every AHB clock cycle
 - All GPIO pins have weak internal pull-up and pull-down resistors, which can be activated or not depending on the value in the GPIOx_PUPDR register
 
 #### I/O pin alternate function multiplexer and mapping
 - device I/O pins connected to on-board peripherals/modules through a multiplexer that allows only 1 peripheral alternate function (AF) connected to an I/O pin at a time -> no conflict between peripherals available on the same I/O pin

 - Each I/O pin has a multiplexer with up to 16 AF inputs (AF0 to AF15) that can be configured through GPIOx_AFRL(pin 0 to 7) and GPIOx_AFRH(8-15) registers
    - After reset multiplexer selection is AF0. I/Os are configured in AF mode through GPIOx_MODER 
    - specific AF assignments for each pin are detailed in the device datasheet
    - Cortex-M7 with FPU EVENTOUT is mapped on AF15

- In addition to I/O multiplexing architecture, each peripheral ahs AFs mapped onto different I/O pins

- To use an I/O in a given configuration, proceed as follows:   
    - Debug function: after each device reset these pins are assigned as alternate function pins immediately usable by the debugger host
    - System function: MCPx pin have to be configured in AF mode
    - GPIO: configure the desired I/O as output, input or analog in GPIOx_MODER 
    - Peripheral alternate function:
        - Connect I/O to the desired AFx in one of GPIOx_AFRL or GPIOx_AFRH
        - Select type, pull-up/pull-down and output speed via GPIOx_OTYPER, GPIOx_PUPDR and GPIOx_OSPEEDER
        - configure the desired I/O as an AF in the GPIOx_MODER register
    - Additional functions:
        - for ADC and DAC, configure the desired I/O in analog mode in the GPIOx_MODER and configure the required function in ADC and ADC registers
        - for additional functions RTC_OUT, RTC_TS,... and oscillators, configure the required function in the related RTC

- EVENTOUT
    - configure the I/O pin used to output the core EVENTOUT signal by connecting it to AF15

- I/O port control registers
    - Each of GPIO ports has 4 32-bit memory-mapped control registers (GPIOx_MODER, GPIOx_OTYPER, GPIOx_OSPEEDR, GPIOx_PUPDR) to configure up to 16 I/Os
    - GPIOx_MODER used to select the I/O mode (input, output, AF, analog)
    - GPIOx_OTYPER and GPIOx_OSPEEDR used to select the output type (push-pull or open-drain) and speed
    - GPIOx_PUPDR used to select the pull-up/pull-down whatever I/O direction

- I/O port data registers
    - each GPIO has 2 16-bit memory-mapped data registers: GPIOx_IDR and GPIOx_ODR. GPIOx_ODR store the data to be output, read/write accessible. Data input through I/O are stored into GPIOx_IDR (read only)

- I/O data bitwise handling
    - bit set reset register (GPIOx_BSRR) is a 32-bit register which allows the application to set and reset each individual it in the output data register (GPIOx), twice size of GPIOx_ODR
    - To each bit in GPIOx_ODR, correspond 2 control bits in GPIOx_BSRR: BS(i) and BR(i). BS(i) = 1 set ODR(i) bit while BR(i) = 1 reset ODR(i). = 0 make no effect
    - GPIOx_BSRR is 'one-shot' effect not lock GPIOx_ODR. it can always be accessed directly, no need to disable interrupts

- GPIO locking mechanism
    - freeze the GPIO control registers by applying a specific write sequence to GPIOx_LCKR

- I/O AF I/O
    - 2 registers are provided to select 1 of the AF input/output available for each I/O

- External interrupt/wakeup lines
    - all ports have external interrupt capability, port must be input mode
    - Extended Interrupt and Event Controller (EXTI)

- EXTI main features:
    - all event inputs allow CPU wakeup and generate a CPU interrupt and/or event
    - some event inputs allow user to wakeup D3 domain of autonomous Run mode and generate D3 domain
    - Asynchronous event input:
        - Configurable events (signal from I/O)
            - selectable active trigger edge
            - interrupt pending status register bit
            - individual interrupt and event generation mask
            - SW trigger possibility
            - configurable system D3 domain wakeup events have D3 pending mask and status register
        - Direct event
            - fixed rising edge active trigger
            - no interrupt pending status register bit in EXTI
    - EXTI block diagram consist of:
        - Register Block accessed via APB interface contain all EXTI register
        - Event Input Trigger block provide Event input edge triggering logic
        - Masking block provide event input distribution

    - EXTI connections between peripherals, CPU and D3:
        - wakeup event when system in Stop mode or CPU in CStop modeare connected to an EXTI configurable event input or direct event input
            - peripheral signals generate a pulse are connected to an EXTI configurable event input
            - peripheral interrupt and wakeup sources have to be cleared in the peripheral are connected to an EXTI Direct Event input

    - EXTI CPU interrupt procedure:
        - unmask teh event input interrupt by setting corresponding mask bits in EXTI_CnIMR
        - for configurable event inputs, enable event input by setting either 1 or both corresponding trigger edge enable bits in EXTI_RTSR and EXTI_FTSR
        - enable associated interrupt source in CPU(n) NVIC or use SEVONPEND, so an interrupt coming from CPU(n) interrupt signal is detectable by CPU after WFI/WFE instruction

- SYSCFG external interrupt configuration register 1 (SYSCFG_EXTICR1 : EXT2 EXT1 EXT0) SYSCFG_EXTICR2,SYSCFG_EXTICR3,...4
    - address offset : 0x08
    - reset value : 0x0000 0000
    - [15:0]: EXTIx[3:0] 
        - 0000: PA[x] pin
        - 0001: PB[x] pin
        - 0010: PC[x] pin
        - 0011: PD[x] pin
        - 0100: PE[x] pin
        - 0101: PF[x] pin
        - 0110: PG[x] pin
        - 0111: PH[x] pin
        - 1000: PI[x] pin
        - 1001: PJ[x] pin
        - 1010: PK[x] pin

- GPIO registers
    - GPIO port mode register (GPIOx_MODER x = A to K)
        - 32 bit -> 16 I/O 
            - 00: input
            - 01: general purpose output mode
            - 10: AF mode
            - 11: analog mode
    - GPIO port output type register (GPIOx_OTYPER x = A to K)
        - OT[15:0]
            - 0: output push-pull
            - 1: output open-drain
    - GPIO port output speed register (GPIOx_OSPEEDR x = A to K)
        - 32 bit -> 16 I/O
            - 00: low speed
            - 01: medium speed
            - 10: high speed
            - 11: very high speed
    - GPIO port pull-up/pull-down (GPIOx_PUPDR x = A to K)
        - 32 bit -> 16 I/O
            - 00: no pull-up, pull-down
            - 01: pull up
            - 10: pull down
            - 11: reserved
    - GPIO port input data register (GPIOx_IDR x = A to K)
        - IDR[15:0] readonly
    - GPIO port output data register (GPIOx_ODR x = A to K)
        - ODR[15:0] read/write
    -GPIO port bit set/reset register (GPIOx_BSRR x = A to K)
        - BR[15:0] write-only
            - 0 no action
            - 1 reset corresponding ODRx bit
        - BS[15:0] write-only
            - 0 no action
            - 1 set corresponding ODRx bit
            