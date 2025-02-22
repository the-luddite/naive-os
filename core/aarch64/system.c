/*
 * Reference: ARM® Architecture Reference Manual ARMv8, for ARMv8-A architecture profile
 */
#include "system.h"

/* CurrentEL, Current Exception Level
	EL, bits [3:2]
		Current exception level. Possible values of this field are:
		00 EL0
		01 EL1
		10 EL2
		11 EL3
*/
u32 raw_read_current_el(void)
{
	u32 current_el;

	__asm__ __volatile__("mrs %0, CurrentEL\n\t" : "=r" (current_el) :  : "memory");
	return current_el;
}

u32 get_current_el(void)
{
	u32 current_el = raw_read_current_el();
	return ((current_el >> CURRENT_EL_SHIFT) & CURRENT_EL_MASK);
}

/* DAIF, Interrupt Mask Bits
	Allows access to the interrupt mask bits.

	D, bit [9]: Debug exceptions.
	A, bit [8]: SError (System Error) mask bit.
	I, bit [7]: IRQ mask bit.
	F, bit [6]: FIQ mask bit.
	value: 
		0 Exception not masked.
		1 Exception masked.
*/
u32 raw_read_daif(void)
{
	u32 daif;

	__asm__ __volatile__("mrs %0, DAIF\n\t" : "=r" (daif) :  : "memory");
	return daif;
}

void raw_write_daif(u32 daif)
{
	__asm__ __volatile__("msr DAIF, %0\n\t" : : "r" (daif) : "memory");
}

void enable_debug_exceptions(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_DBG_BIT)  : "memory");
}

void enable_serror_exceptions(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_ABT_BIT)  : "memory");
}

void enable_irq(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_IRQ_BIT)  : "memory");
}

void enable_fiq(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_FIQ_BIT)  : "memory");
}

void disable_debug_exceptions(void)
{
	__asm__ __volatile__("msr DAIFSet, %0\n\t" : : "i" (DAIF_DBG_BIT)  : "memory");
}

void disable_serror_exceptions(void)
{
	__asm__ __volatile__("msr DAIFSet, %0\n\t" : : "i" (DAIF_ABT_BIT)  : "memory");
}

void disable_irq(void)
{
	__asm__ __volatile__("msr DAIFSet, %0\n\t" : : "i" (DAIF_IRQ_BIT)  : "memory");
}

void disable_fiq(void)
{
	__asm__ __volatile__("msr DAIFSet, %0\n\t" : : "i" (DAIF_FIQ_BIT)  : "memory");
}

/* SPSR_EL1, Saved Program Status Register (EL1)
	Holds the saved processor state when an exception is taken to EL1.
*/
u32 raw_read_spsr_el1(void)
{
	u32 spsr_el1;

	__asm__ __volatile__("mrs %0, SPSR_EL1\n\t" : "=r" (spsr_el1) :  : "memory");
	return spsr_el1;
}

void raw_write_spsr_el1(u32 spsr_el1)
{
	__asm__ __volatile__("msr SPSR_EL1, %0\n\t" : : "r" (spsr_el1) : "memory");
}


/* 
ISR_EL1, Interrupt Status Register
	Shows whether an IRQ, FIQ, or SError interrupt is pending.
*/
u32 raw_read_isr_el1(void)
{
	u32 isr_el1;

	__asm__ __volatile__("mrs %0, ISR_EL1\n\t" : "=r" (isr_el1) :  : "memory");
	return isr_el1;
}

/* 
RVBAR_EL1, Reset Vector Base Address Register (if EL2 and EL3 not implemented)
	If EL1 is the highest exception level implemented, contains the 
	IMPLEMENTATION DEFINED address that execution starts from after reset when
	executing in AArch64 state.
*/
u64 raw_read_rvbar_el1(void)
{
	u64 rvbar_el1;

	__asm__ __volatile__("mrs %0, RVBAR_EL1\n\t" : "=r" (rvbar_el1) :  : "memory");
	return rvbar_el1;
}

void raw_write_rvbar_el1(u64 rvbar_el1)
{
	__asm__ __volatile__("msr RVBAR_EL1, %0\n\t" : : "r" (rvbar_el1) : "memory");
}

/* VBAR_EL1, Vector Base Address Register (EL1)
	Holds the exception base address for any exception that is taken to EL1.
*/
u64 raw_read_vbar_el1(void)
{
	u64 vbar_el1;

	__asm__ __volatile__("mrs %0, VBAR_EL1\n\t" : "=r" (vbar_el1) :  : "memory");
	return vbar_el1;
}

void raw_write_vbar_el1(u64 vbar_el1)
{
	__asm__ __volatile__("msr VBAR_EL1, %0\n\t" : : "r" (vbar_el1) : "memory");
}

/* CNTV_CTL_EL0, Counter-timer Virtual Timer Control register
	Control register for the virtual timer.

	ISTATUS, bit [2]:	The status of the timer interrupt.
	IMASK, bit [1]:		Timer interrupt mask bit.
	ENABLE, bit [0]:	Enables the timer.
*/
u32 raw_read_cntv_ctl(void)
{
	u32 cntv_ctl;

	__asm__ __volatile__("mrs %0, CNTV_CTL_EL0\n\t" : "=r" (cntv_ctl) :  : "memory");
	return cntv_ctl;
}

void disable_cntv(void)
{
	u32 cntv_ctl;

	cntv_ctl = raw_read_cntv_ctl();
	cntv_ctl &= ~CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

void enable_cntv(void)
{
	u32 cntv_ctl;

	cntv_ctl = raw_read_cntv_ctl();
	cntv_ctl |= CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

/*
CNTFRQ_EL0, Counter-timer Frequency register
	Holds the clock frequency of the system counter.
*/
u32 raw_read_cntfrq_el0(void)
{
	u32 cntfrq_el0;

	__asm__ __volatile__("mrs %0, CNTFRQ_EL0\n\t" : "=r" (cntfrq_el0) : : "memory");
	return cntfrq_el0;
}

void raw_write_cntfrq_el0(u32 cntfrq_el0)
{
	__asm__ __volatile__("msr CNTFRQ_EL0, %0\n\t" : : "r" (cntfrq_el0) : "memory");
}

/* CNTVCT_EL0, Counter-timer Virtual Count register
	Holds the 64-bit virtual count value.
*/
u64 raw_read_cntvct_el0(void)
{
	u64 cntvct_el0;

	__asm__ __volatile__("mrs %0, CNTVCT_EL0\n\t" : "=r" (cntvct_el0) : : "memory");
	return cntvct_el0;
}

/* CNTV_CVAL_EL0, Counter-timer Virtual Timer CompareValue register
	Holds the compare value for the virtual timer.
*/
u64 raw_read_cntv_cval_el0(void)
{
	u64 cntv_cval_el0;

	__asm__ __volatile__("mrs %0, CNTV_CVAL_EL0\n\t" : "=r" (cntv_cval_el0) : : "memory");
	return cntv_cval_el0;
}

void raw_write_cntv_cval_el0(u64 cntv_cval_el0)
{
	__asm__ __volatile__("msr CNTV_CVAL_EL0, %0\n\t" : : "r" (cntv_cval_el0) : "memory");
}

u32 pending_irq(irq_no *pending_irq)
{
	for(irq_no i = 0; GIC_INT_MAX > i; ++i) 
    {
		if (gicd_probe_pending(i)) 
		{
				*pending_irq = i;
				return 1;
		}
    }
    return 0;

}