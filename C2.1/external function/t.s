	.cpu arm926ej-s
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"t.c"
	.text
	.align	2
	.global	func		// export func as global symbol
	.syntax unified
	.arm
	.fpu softvfp
	.type	func, %function
func:
	@ args = 8, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	// (1). Establish stack frame
	push	{fp, lr}	// save lr, fp in stack
	add	fp, sp, #4
	// (2). Shift SP downward 8 (4-byte) slots for locals and temps
	sub	sp, sp, #32
	// (3). Save r0-r3 (parameter a,b,c,d) in stack at -offsets(fp)
	str	r0, [fp, #-24]
	str	r1, [fp, #-28]
	str	r2, [fp, #-32]
	str	r3, [fp, #-36]
	// (4). execute x=1, y=2, z=3 show their locations on stack
	mov	r3, #1
	str	r3, [fp, #-8]	// x = 1 at -8(fp)
	mov	r3, #2
	str	r3, [fp, #-12]  // y = 2 at -12(fp)
	mov	r3, #3
	str	r3, [fp, #-16]	// z = 3 at -16(fp)
	// (5). Prepare to call g(x,y)
	ldr	r1, [fp, #-12]	// r1 = y
	ldr	r0, [fp, #-8]	// r0 = x
	bl	g				// call g(x,y)
	// (6). Compute a+e as return value in r0
	ldr	r2, [fp, #-24]	// r2 = a (saved at -24(fp))
	ldr	r3, [fp, #4]	// r3 = e at +4(fp)
	add	r3, r2, r3		// r3 = a+e
	mov	r0, r3			// r0 = return value in r0
	// (7). Return to caller
	sub	sp, fp, #4		// sp = fp - 4 (pointer at saved FP)	
	@ sp needed			
	pop	{fp, pc}
	.size	func, .-func
	.ident	"GCC: (15:6.3.1+svn253039-1build1) 6.3.1 20170620"
