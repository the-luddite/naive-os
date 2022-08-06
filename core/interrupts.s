/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	b.eq	\el3_label
	cmp	\xreg, 0x8
	b.eq	\el2_label
	cmp	\xreg, 0x4
	b.eq	\el1_label
.endm

.macro	exception_entry, el
	sub	sp, sp, #272
	stp	x0, x1, [sp, #16 * 0]
	stp	x2, x3, [sp, #16 * 1]
	stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]

	.if	\el == 0
	mrs	x21, sp_el0
	.else
	add	x21, sp, #272
	.endif /* \el == 0 */

	mrs	x22, elr_el1
	mrs	x23, spsr_el1

	stp	x30, x21, [sp, #16 * 15] 
	stp	x22, x23, [sp, #16 * 16]
.endm

.macro	exception_exit, el
	ldp	x22, x23, [sp, #16 * 16]
	ldp	x30, x21, [sp, #16 * 15] 

	.if	\el == 0
	msr	sp_el0, x21
	.endif /* \el == 0 */

	msr	elr_el1, x22			
	msr	spsr_el1, x23


	ldp	x0, x1, [sp, #16 * 0]
	ldp	x2, x3, [sp, #16 * 1]
	ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]
	add	sp, sp, #272		
	eret
.endm

/*
 * Exception vectors.
 */
	.align	11
	.globl	vectors
vectors:
	.align	7
	b	_do_bad_sync_el1	/* Current EL1 Synchronous Thread */

	.align	7
	b	_do_bad_irq_el1	/* Current EL1 IRQ Thread */

	.align	7
	b	_do_bad_fiq_el1	/* Current EL1 FIQ Thread */

	.align	7
	b	_do_bad_error_el1	/* Current EL1 Error Thread */

	.align	7
	b	_do_sync_el1	/* Current EL1 Synchronous Handler */

	.align	7
	b	_do_irq_el1		/* Current EL1 IRQ Handler */

	.align	7
	b	_do_fiq_el1		/* Current EL1 FIQ Handler */

	.align	7
	b	_do_error_el1	/* Current EL1 Error Handler */

	.align	7
	b	_do_bad_sync_el0	/* Current EL0 Synchronous Thread */

	.align	7
	b	_do_bad_irq_el0	/* Current EL0 IRQ Thread */

	.align	7
	b	_do_bad_fiq_el0	/* Current EL0 FIQ Thread */

	.align	7
	b	_do_bad_error_el0	/* Current EL0 Error Thread */

	.align	7
	b	_do_sync_el0	/* Current EL0 Synchronous Handler */

	.align	7
	b	_do_irq_el0		/* Current EL0 IRQ Handler */

	.align	7
	b	_do_fiq_el0		/* Current EL0 FIQ Handler */

	.align	7
	b	_do_error_el0	/* Current EL0 Error Handler */


_do_bad_sync_el1:
	exception_entry 1
	bl	do_bad_sync_el1
	exception_exit 1

_do_bad_irq_el1:
	exception_entry 1
	bl	do_bad_irq_el1
	exception_exit 1

_do_bad_fiq_el1:
	exception_entry 1
	bl	do_bad_fiq_el1
	exception_exit 1

_do_bad_error_el1:
	exception_entry 1
	bl	do_bad_error_el1
	exception_exit 1

_do_sync_el1:
	exception_entry 1
	bl	do_sync_el1
	exception_exit 1

_do_irq_el1:
	exception_entry 1
	bl	do_irq_el1
	exception_exit 1

_do_fiq_el1:
	exception_entry 1
	bl	do_fiq_el1
	exception_exit 1

_do_error_el1:
	exception_entry 1
	bl	do_error_el1
	exception_exit 1

/* 		EL0	 	*/

_do_bad_sync_el0:
	exception_entry 0
	bl	do_bad_sync_el0
	exception_exit 0

_do_bad_irq_el0:
	exception_entry 0
	bl	do_bad_irq_el0
	exception_exit 0

_do_bad_fiq_el0:
	exception_entry 0
	bl	do_bad_fiq_el0
	exception_exit 0

_do_bad_error_el0:
	exception_entry 0
	bl	do_bad_error_el0
	exception_exit 0

_do_sync_el0:
	exception_entry 0
	bl	do_sync_el0
	exception_exit 0

_do_irq_el0:
	exception_entry 0
	bl	do_irq_el0
	exception_exit 0

_do_fiq_el0:
	exception_entry 0
	bl	do_fiq_el0
	exception_exit 0

_do_error_el0:
	exception_entry 0
	bl	do_error_el0
	exception_exit 0