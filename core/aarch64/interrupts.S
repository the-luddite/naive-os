/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include "aarch64/registers.h"
#include "syscalls/syscalls.h"


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
	b	_curr_el_sp0_sync	/* Current EL1 Synchronous Thread */

	.align	7
	b	_curr_el_sp0_irq	/* Current EL1 IRQ Thread */

	.align	7
	b	_curr_el_sp0_fiq	/* Current EL1 FIQ Thread */

	.align	7
	b	_curr_el_sp0_serror	/* Current EL1 Error Thread */

	.align	7
	b	_curr_el_spx_sync	/* Current EL1 Synchronous Handler */

	.align	7
	b	_curr_el_spx_irq		/* Current EL1 IRQ Handler */

	.align	7
	b	_curr_el_spx_fiq		/* Current EL1 FIQ Handler */

	.align	7
	b	_curr_el_spx_serror	/* Current EL1 Error Handler */

	.align	7
	b	_lower_el_aarch64_sync	/* Current EL0 Synchronous Thread */

	.align	7
	b	_lower_el_aarch64_irq	/* Current EL0 IRQ Thread */

	.align	7
	b	_lower_el_aarch64_fiq	/* Current EL0 FIQ Thread */

	.align	7
	b	_lower_el_aarch64_serror	/* Current EL0 Error Thread */

	.align	7
	b	_lower_el_aarch32_sync	/* Current EL0 Synchronous Handler */

	.align	7
	b	_lower_el_aarch32_irq		/* Current EL0 IRQ Handler */

	.align	7
	b	_lower_el_aarch32_fiq		/* Current EL0 FIQ Handler */

	.align	7
	b	_lower_el_aarch32_serror	/* Current EL0 Error Handler */


_curr_el_sp0_sync:
	exception_entry 1
	bl	curr_el_sp0_sync
	exception_exit 1

_curr_el_sp0_irq:
	exception_entry 1
	bl	curr_el_sp0_irq
	exception_exit 1

_curr_el_sp0_fiq:
	exception_entry 1
	bl	curr_el_sp0_fiq
	exception_exit 1

_curr_el_sp0_serror:
	exception_entry 1
	bl	curr_el_sp0_serror
	exception_exit 1

_curr_el_spx_sync:
	exception_entry 1
	bl	curr_el_spx_sync
	exception_exit 1

_curr_el_spx_irq:
	exception_entry 1
	bl	curr_el_spx_irq
	exception_exit 1

_curr_el_spx_fiq:
	exception_entry 1
	bl	curr_el_spx_fiq
	exception_exit 1

_curr_el_spx_serror:
	exception_entry 1
	bl	curr_el_spx_serror
	exception_exit 1

/* 		EL0	 	*/

_lower_el_aarch64_sync:
	exception_entry 0
	mrs	x25, esr_el1				// read the syndrome register
	lsr	x24, x25, #ESR_ELx_EC_SHIFT		// exception class
	cmp	x24, #ESR_ELx_EC_SVC64			// SVC in 64-bit state
	b.eq	el0_svc
	bl lower_el_aarch64_sync
	exception_exit 0

_lower_el_aarch64_irq:
	exception_entry 0
	bl	lower_el_aarch64_irq
	exception_exit 0

_lower_el_aarch64_fiq:
	exception_entry 0
	bl	lower_el_aarch64_fiq
	exception_exit 0

_lower_el_aarch64_serror:
	exception_entry 0
	bl	lower_el_aarch64_serror
	exception_exit 0

_lower_el_aarch32_sync:
	exception_entry 0
	bl	lower_el_aarch32_sync
	exception_exit 0

_lower_el_aarch32_irq:
	exception_entry 0
	bl	lower_el_aarch32_irq
	exception_exit 0

_lower_el_aarch32_fiq:
	exception_entry 0
	bl	lower_el_aarch32_fiq
	exception_exit 0

_lower_el_aarch32_serror:
	exception_entry 0
	bl	lower_el_aarch32_serror
	exception_exit 0

/* End of exception handler difinition */

sc_nr	.req	x25					// number of system calls
scno	.req	x26					// syscall number
stbl	.req	x27					// syscall table pointer

el0_svc:
	adr	stbl, sys_call_table			// load syscall table pointer
	uxtw	scno, w8				// syscall number in w8
	mov	sc_nr, #__NR_syscalls
	bl	enable_irq
	cmp     scno, sc_nr                     	// check upper syscall limit
	b.hs	ni_sys

	ldr	x16, [stbl, scno, lsl #3]		// address in the syscall table
	blr	x16					// call sys_* routine
	b	ret_from_syscall
ni_sys:
	bl invalid_syscall
ret_from_syscall:
	bl	disable_irq				
	str	x0, [sp, #0]				// returned x0
	exception_exit 0

.globl ret_from_fork
ret_from_fork:
	bl	schedule_tail
	cbz	x19, ret_to_user			// not a kernel thread
	mov	x0, x20
	blr	x19
ret_to_user:
	bl disable_irq				
	exception_exit 0 