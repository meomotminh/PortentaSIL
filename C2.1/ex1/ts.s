/********* ts.s file of C2.1 **********/
    .text
    .global start
start:
    mov r0, #1      @ r0 = 1
    MOV R1, #2      @ r1 = 2
    ADD R1, R1, R0  // r1 = r1 + r0
    ldr r2, =result // r2 = &result
    str r1, [r2]    /* result = r1*/
stop: b stop
    .data
result: .word 0 /* a word location */

/*
    load CPU register r0 with value 1
    r1 with value 2
    then add r0 to r1 and store into memory location labeled result
    in assembly, instructions are case insensitive
    @ or // to start comment
    

*/
