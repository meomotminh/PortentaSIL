## Introduction

- software debugger solution communicate via Ethernet or RS232 with a gdbserver/gdb stub using GDB remote Serial Protocol (RSP)

- connect GNU gdbserver to debug Linux user-space processes
- debug Linux kernel via KGDB
- connect any virtual target or debugger implementing a gdbstub
- as front end for UndoDB reversible debugger
- **Run Mode** 
    - debug Linux user-space process using GNU gdbserver
    - only selcted task is stopped when breakpoint is hit.

and **Stop Mode**
    - control whole system, not single process

- Configuration File
    - config.t32 in TRACE32 system directory
        ```
        PBI = GDB

        SYS = C:\T32
        TMP = C:\Temp

        SCREEN = 
        FONT = SMALL
        ```

- T32 start

- Connect to QEMU
    - start QEMU using 
        ```
        -gdb tcp::<port_number>
        ```

```
    RESET
    WinCLEAR
    SYStem.CPU *
    SYStem.PORT 192.168.188.50:2345

    SYStem.GDBconfig GDBSERVER ON

    SYStem.Option STEPSOFT ON

    SYStem.Mode Attach

    Data.LOAD.Elf sieve /NoCODE /NoClear
```

On-chip debug interface

- most common on-chip debug interface is JTAG
- number of on-chip breakpoints
- read/write access to memory while program is running
- additional feature as benchmark counter, trigger

- on-chip trace buffer
- trace port TPIU