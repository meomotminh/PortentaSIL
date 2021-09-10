    .global setjmp, longjmp
setjmp: // int setjmp(int env[2]); save LR, FP in env[2]; return 0
        stmfd sp!, {fp, lr}
        add   fp, sp, #4
        ldr   r1, [fp]          // caller's return LR
        str   r1, [r0]          // save LR in env[0]
        ldr   r1, [fp, #-4]     // caller's FP
        str   r1, [r0, #4]      // save FP in env[1]
        mov   r0, #0            // return 0 to caller
        sub   sp, fp, #4
        ldmfd sp!, {fp, pc}

longjmp: // int longjmp(int env[2], int value)
        stmfd sp!, {fp, lr}
        add   fp, sp, #4
        ldr   r2, [r0]          // return function's LR
        str   r2, [fp]          // replace saved LR in stack frame
        ldr   r2, [r0, #4]      // return function's FP
        str   r2, [fp, #-4]     // replace saved FP in stack frame
        mov   r0, r1            // return value
        sub   sp, fp, #4
        ldmfd sp!, {fp, pc}     // return via REPLACED LR and FP
