## Debug Infrastructure

device offer comprehensive set of debug and trace features on both cores to support software dev and system integration
- Breakpoint debugging
- Code execution tracing
- Software instrumentation
- JTAG debug port
- Serial-wire debug port
- Trigger input and output
- Serial-wire trace port
- Trace port
- ARM coresight debug and trace components

the debug can be controlled via JTAG/Serial-Wire debug access port -> allow debugging 1 core or both cores in parallel

trace port performs data capture for logging and analysis

4 KB embedded trace FIFO (ETF) allow recording data and sending them to any com port. IN TRace mode, trace is transferred by DMA to system RAM or to high-speed interface (SPI or USB), can be even be monitored by a software running on 1 of the cores. unlike hardware FIFO mode, this mode is invasive since it use system resources which are shared by processors

**JTAG interface**
- processor use JTAG interface to provide access to debug/emulation functinos
- 4 wires JTAG communications protocol, collective known as the Test Access Port (TAP) -> designed to interact with new registers added to devices to implement this method of testing
- main register added to device specifically for JTAG testing is called Boundary Scan Register (BSR)
- boundary scan cells can operate in 2 modes: 
    - functional mode: no effect on the operation of the device
    - test mode: disconnect the functional core of the device from the pins, putting boundary scan cells into test mode -> used to control the values being driven from an enabled device onto a net and also be used to monitor the value of the net.
- disconnect the control of the pins from functionality of the enabled device makes boundary scan test dev significantly easier than traditional functional test as no device config or booting is required to use the pins