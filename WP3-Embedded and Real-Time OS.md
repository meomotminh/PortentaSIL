ref: Wang, K.C. (2017), *Embedded and Real-Time Operating Systems*, Switzerland : Springer Nature

## Foreword
    - control programs of early embedded system are simple: super-loop or simple event-driven program structure

    - Chap 2: ARM architecture, ARM instructions, ARM programming    
    - Chap 3: interrupts and exceptions processing
    - Chap 4: models of embedded system
    - Chap 5: process management (process, multitasking)
    - Chap 6: ARM memory management unit (MMU) and virtual address space mappings
    - Chap 7: user mode processes and system calls
    - Chap 8: fully functional general purpose OS(GPOS)
    - Chap 9: multiprocessing in embedded system

## ARM Architecture and Programming
    - ARM is a family of Reduced Instruction Set Computing (RISC) microprocessors developed specifically for mobile and embedded computing env.
    - 2 types: classic cores and Cortex cores
    - ARM Cortex core can be classified:
        - Cortex M series: microcontroller-oriented processors (MCU) and SoC app
        - Cortex R series: real-time signal processing and control app
        - Cortex A series: General purpose App
    - Cortex A series processors are most powerful, Cortex-A8 and Cortex-A9 up to 4 CPUs
    - ARM processor has 7 operating modes, specified by 5 mode bits [4:0] in the Current Processor Status Register (CPSR):
        - USR mode: unprivileged User mode
        - SYS mode: System mode using the same set of registers as User mode
        - FIQ mode: Fast interrupt request processing mode
        - IRQ mode: normal interrupt request processing mode
        - SVC mode: Supervisor mode on reset or SWI
        - ABT mode: data exception abort mode
        - UND mode: undefined instruction exception mode
    
    - ARM CPU registers:
        - ARM processor has 37 registers in total, all of which are 32-bits wide, including:
            - 1 dedicated program counter (PC)
            - 1 dedicated current program status register (CPSR)
            - 5 dedicated saved program status register (SPSR)
            - 30 general purpose registers
    - General register
        - USR and SYS share the same set of registers 
        - R0-R12 are the same in all modes, except FIQ mode, which has its own separate register R8-R12
        - each mode has its own stack pointer (R13) and link register (R14)
        - PC or R15 and CPSR are the same in all mode
        - Each privileged mode (SVC or FIQ) has its own SPSR
    - Status Register CPSR: CPSR M[4:0]
        - USR: 10000 (0x10)
        - FIQ: 10001 (0x11)
        - IRQ: 10010 (0x12)
        - SVC: 10011 (0x13)
        - ABT: 10111 (0x17)
        - UND: 11011 (0x1B)
        - SYS: 11111 (0x1F)
    - Change ARM processor mode:
        - ARM processor change mode in response to exceptions or interrupts
            - SVC mode when power turn on, following reset or executing SWI instruction
            - Abort mode when memory access exception occur
            - UND mode when encounter undefined instruction
            - while in privileged mde, can change mode freely by simply altering mode bits in CPSR by using MSR and MRS instrucions
                - after reset, in SVC mode -> system initialization code must set up stack pointers of other modes -> change processor to appropriate mode, initialize stack pointer (R13) and SPSR
            MRS r0, cpsr        // get CPSR into r0
            BIC r1, r0, #01F    // Bit clear
            ORR r1, r1, #0x12   // change to IRQ mode   
            MSR cpsr, r1        // write to cpsr
        
            MSR cpsr, #0x92
        
        - Special use of SYS mode is to access User mode registers, R13 (stack pointer), R14 (link register) from privileged mode
        - In an OS, process usually run in unprivileged User mode. when process does system call (SWI), it enter the system in kernel in SVC mode. while in kernel mode, process may need to manipulate its User mode stack and return address to the User mode image -> process must be able to access it User mode sp and lr -> switching CPU to SYS mode.
        - in interrupt, IRQ mode to execute ISR to handle interrupt, if ISR allowed nested interrupts, it must switch processor from IRP to different privileged mode to handle nested interrupts

    - Instruction Pipeline: internal pipeline to increase rate of instruction flow to the processor, allow several operations to be undertaken simultaneously, rather than serially.  in most ARM processors, instruction pipeline consist of 3 stages: FETCH-DECODE-EXECUTE
        - PC    FETCH   Fetch instrucion from memory
        - PC-4  DECODE  Decode registers used in instrucion
        - PC-8  EXECUTE Execute the instruction
        - PC actually points to the instruction being fetched, rather than instrucion being executed -> function calls and interrupt handlers. When call a function using BL(branch with link, cause a branch to *label*) instruction, return address is PC-4. when return from ISR, return address is also PC-4 which must be self adjusted unless ISR is defined with __attribute__((interrupt)) attribute. Some exceptions, Abort return address PC-8 which point to original instruction that cause the exception

    - ARM instructions:
        - condition flags and conditions: in CPSR of ARM, NZVC are condition flags or simply condition code 
            N = negative, Z = zero, V = overflow, C = carry bit out
        - condition flags are set by comparison and TST operations. by default, data processing not affect condition flags -> posfix with S symbol (which set S bit in the instruction encoding)
        - in ARM 32-bit instruction encoding, the leading 4 bits [31:28] represent various combinations of the condition flag bits, which form the condition field of the instruction
            - 0000 : EQ Equal               (Z set)
            - 0001 : NE Not equal           (Z clear)
            - 0010 : CS Carry set           (C set)
            - 0101 : VS overflow set        (V set)
            - 1000 : HI Unsigned higher     (C set and Z clear)
            - 1001 : LS Unsigned lower or same (C clear or Z set)
            - 1011 : LT Signed less than       (C != V)
            - 1100 : GT Signed greater than    (Z=0 and N=V)
            - 1101 : LE signed less than or equal   (Z=1 or N!=V)
            - 1110 : AL always          
        - almost all instructions can be executed conditionally. ADDEQ r0, r1, r2 (if Z set then r0 = r1 + r2)
        - Branch instructions:
            - B instruction causes a direct branch to an offset relative to the current PC
            - BL instruction is for subroutine calls, it write PC-4 into LR of the current register bank and replaces PC with the entry address of the subroutine, causing CPU to enter subroutine. when subroutine finish, it return by the saved return address in the link register R14. other saving return address on stack, ARM copies PC-4 into R14 and branches to the called subroutine
                MOV PC, LR or BX LR
        - Arithmetic operations:
            - barrel shifter if second operand is immediate value
                ADD r0, r1, r2
                SUB r3, r3, #1
        - Comparison Operations
            - CMP: operand 1 - operand2 
            - TST: operand 1 AND operand2
            - TEQ: operand 1 EOR operand2
                CMP r0, r1
                TSTEQ r2, #5
        - Logical operations
            - AND, EOR, ORR, BIC
        - Data movement operations
            - MOV, MVN
                MOV r0, r1 // r0 = r1
                MOVS r2, #10    // r2 = 10 and set condition bits Z=0, N=0
                MOVNEQ r1, #0   // r1 = 0 only if Z bit != 0
        - Immediate value and Barrel shifer
            - Barrel shifter is unique feature of ARM processor, used to generate shift operations and immediate operands inside ARM processor. ARM processor don't have actual shift instructions, instead barrel shifter perform sifts as part of other instructions
                MOV r0, r0, LSL #1      // shift r0 left by 1 bit (x2)
                MOV r1, r1, LSR #2      // shift r1 right by 2 bits (/4)
                MOV r2, r2, ROR #4      // swap high and low 4 bits of r2
            - other processors allow loading CPU registers with immediate values, which form parts of the instruction stream making the instruction length variable. In contrast, all ARM instructions are 32 bits long. data processing instruction format has 12 bits available for operand2. if used directly, only give range 0-4095. instead, it is used to store a 4-bit rotate value and an 8-bit const in range 0-255. 8 bits can rotated right and even number of positions
                MOV r0, #0x40, 26   0x1000 = 0x40 ROR 26
            - instead of MOV, LDR allow loading an arbitrary 32-bit value into a register
                LDR rd, =numeric_const
        - Multiply instructions
            - MUL Rd, Rm, Rs  // Rd = Rm * Rs
            - MLA Rd, Rm, Rs, Rn    // Rd = Rm*Rs + Rn
        - LOAD and STORE instructions
            - 3 set of instrucion which interact with memory:
                - Single register data transfer (LDR/STR)
                - Block data transfer (LDM/STM)
                - Single data swap (SWP)
                    LDR / STR / LDRB / STRB
        - Base Register
            - Load/Store instructions use a base register as an index to specify the memory location to be accessed, index may include an offset in either pre-index or post-index addressing mode
                STR r0, [r1]  // store r0 to location pointed by r1
                LDR r2, [r1]  // load r2 from memory pointed by r1
                STR r0, [r1, #12] // pre-index addressing STR r0 to [r1 + 12]
                STR r0, [r1], #12 // post-index addressing STR r0 to [r1], r1 + 12
        - Block data transfer
            - base register is used to determine where memory access should occur, 4 addressing modes allow increment, decrement inclusive or exclusive of the base register location. 
            - base register can be updated if append "!"
        - Stack operations
            - a stack is a memory area which grows as new data is "pushed" onto the "top" of the stack and shrink as data "popped" off the top of the stack. 2 pointers are used to define current limits of the stack:
                - A base pointer: point to "bottom" of the stack (1st location)
                - A stack pointer: point to current "top" of the stack
            - stack is called descending if it grows downward in memory , last push value is at the lowest address, ascending if grows upward in memory
            - ARM support both ascending and descending stacks, also allow stack pointer to either point to the last occupied address (Full stack), or to the next occupied address (Empty stack). stack operations implemented by STM/LDM instructions. the stack type is determined by postfix in the STM/LDM instructions:
                - STMFD/LDMFD: Full Descending Stack
                - STMFA/LDMFA: Full Ascending Stack
                - STMED/LDMED: Empty Descending Stack
                - STMEA/LDMEA: Empty Ascending Stack
            - C Compiler always use Full Descending Stack
        
        - Stack and Subroutines
            - common usage of stack is to create temporary workspace for subroutines. when a subroutine begin, any registers to be preserved can be pushed onto the stack. when a subroutine end, it restore the saved registers by popping them off the stack before returning to the caller.
                STMFD sp!, {r0-r12, lr} // save all registers and return address
                {Code of Subroutine}
                LDMFD sp!, {r0-r12, pc} // restore saved registers and return by lr
            - if pop instruction has S bit set then transferring PC register while in a privileged mode also copies the saved SPSR to the previous mode CPSR, causing return to the previous mode prior to the exception (by SWI or IRQ)
        
        - Software Interrupt (SWI)
            - in ARM, SWI instruction is used to generate a software interrupt. after executing SWI instruction, ARM processor changes to SVC mode and execute from SVC vector address 0x08, causing it to execute the SWI handler, usually entry point of system calls to the OS kernel

        - PSR Transfer functions
            - MRS and MSR instructions allow contents of CPSR/SPSR to be transferred from appropriate status register to a general purpose register
        
        - Coprocessor Instrucion
            - treat many hardware components, MMU as coprocessors which are accessed by special coprocessor instruction

        - ARM toolchain: is a collection of programming tools for program development, from source code to binary executable files, consist of : assembler, compiler, linker, some utility programs (objcopy) for file conversions and a debugger
            - run on host machine and generates code for target machine. if host and target architecture are different, toolchain is called "cross toochain" or "cross compiler". Linux-based ARM-targeting cross compiler. we use arm-none-eabi toolchain as well as qemu-system-arm for ARM virtual machines

        - ARM System Emulators
            - QEMU support many emulated ARM machines: ARM926EJ-S 2016 as the platform for implementation and testing:
                - many peripheral devices
                - ARM Versatile board architecture is well documented
                - QEMU can boot up the emulated ARM Versatilepb virtual machine directly                


    