.section ".startup"

.text

.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	b.eq	\el3_label
	cmp	\xreg, 0x8
	b.eq	\el2_label
	cmp	\xreg, 0x4
	b.eq	\el1_label
.endm

.global uart
uart:
    .xword 150994944

    .text
    .global _start
_start:
sleep_cores_1_2_3:
    mrs     x0, mpidr_el1   // Gets core number using the
                            // multiprocessor affinity register.
    and     x0, x0, #3      // Checks if the core id is 1, 2 or 3
    cbz     x0, cpu_0_code  // If core 0, it jumps to cpu_0_code.

infinite_loop:
    wfe                     // Puts the core to sleep (Wait for event)
    b       infinite_loop

.globl ret_from_fork
ret_from_fork:
	bl	schedule_tail
	mov	x0, x20
	blr	x19 		//should never return

.globl cpu_switch_to
cpu_switch_to:
	mov	x10, #0
	add	x8, x0, x10
	mov	x9, sp
	stp	x19, x20, [x8], #16		// store callee-saved registers
	stp	x21, x22, [x8], #16
	stp	x23, x24, [x8], #16
	stp	x25, x26, [x8], #16
	stp	x27, x28, [x8], #16
	stp	x29, x9, [x8], #16
	str	x30, [x8]
	add	x8, x1, x10
	ldp	x19, x20, [x8], #16		// restore callee-saved registers
	ldp	x21, x22, [x8], #16
	ldp	x23, x24, [x8], #16
	ldp	x25, x26, [x8], #16
	ldp	x27, x28, [x8], #16
	ldp	x29, x9, [x8], #16
	ldr	x30, [x8]
	mov	sp, x9
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret

cpu_0_code:
    adr	x0, vectors 
       
    switch_el x1, 3f, 2f, 1f

3:	msr	vbar_el3, x0
	mrs	x0, scr_el3
	orr	x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
	msr	scr_el3, x0
	msr	cptr_el3, xzr			/* Enable FP/SIMD */
	ldr	x0, =0x1800000          /* Set counter friequency to 24MHz */
	msr	cntfrq_el0, x0			/* Initialize CNTFRQ */
	b	0f
2:	msr	vbar_el2, x0
	mov	x0, #0x33ff
	msr	cptr_el2, x0			/* Enable FP/SIMD */
	b	0f
1:	msr	vbar_el1, x0
	mov	x0, #3 << 20
	msr	cpacr_el1, x0			/* Enable FP/SIMD */
0:
	adrp	x0, stack_top
    mov	sp, x0
    bl core_main
