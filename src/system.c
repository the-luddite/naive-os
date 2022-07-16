#include "system.h"


uint32_t raw_read_current_el(void)
{
	uint32_t current_el;

	__asm__ __volatile__("mrs %0, CurrentEL\n\t" : "=r" (current_el) :  : "memory");
	return current_el;
}

uint32_t raw_read_cntv_ctl(void)
{
	uint32_t cntv_ctl;

	__asm__ __volatile__("mrs %0, CNTV_CTL_EL0\n\t" : "=r" (cntv_ctl) :  : "memory");
	return cntv_ctl;
}

void disable_cntv(void)
{
	uint32_t cntv_ctl;

	cntv_ctl = raw_read_cntv_ctl();
	cntv_ctl &= ~CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

/*
CNTFRQ_EL0, Counter-timer Frequency register
	Holds the clock frequency of the system counter.
*/
uint32_t raw_read_cntfrq_el0(void)
{
	uint32_t cntfrq_el0;

	__asm__ __volatile__("mrs %0, CNTFRQ_EL0\n\t" : "=r" (cntfrq_el0) : : "memory");
	return cntfrq_el0;
}


/* CNTVCT_EL0, Counter-timer Virtual Count register
	Holds the 64-bit virtual count value.
*/
uint64_t raw_read_cntvct_el0(void)
{
	uint64_t cntvct_el0;

	__asm__ __volatile__("mrs %0, CNTVCT_EL0\n\t" : "=r" (cntvct_el0) : : "memory");
	return cntvct_el0;
}

void enable_cntv(void)
{
	uint32_t cntv_ctl;

	cntv_ctl = raw_read_cntv_ctl();
	cntv_ctl |= CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}


void raw_write_cntv_cval_el0(uint64_t cntv_cval_el0)
{
	__asm__ __volatile__("msr CNTV_CVAL_EL0, %0\n\t" : : "r" (cntv_cval_el0) : "memory");
}


void enable_irq(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_IRQ_BIT)  : "memory");
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
uint32_t raw_read_daif(void)
{
	uint32_t daif;

	__asm__ __volatile__("mrs %0, DAIF\n\t" : "=r" (daif) :  : "memory");
	return daif;
}