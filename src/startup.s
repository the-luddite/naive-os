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

cpu_0_code:
    // bl init_stack_pointer_regs
    adrp	x0, stack_top
    mov	sp, x0

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
    // debug message start
    adrp x0, uart
    add x0, x0, :lo12:uart
    ldr x0, [x0]

    mov w1, 104
    strb w1, [x0]

    mov w1, 101
    strb w1, [x0]

    mov w1, 108
    strb w1, [x0]

    mov w1, 108
    strb w1, [x0]
    // debug message end

    bl core_main
