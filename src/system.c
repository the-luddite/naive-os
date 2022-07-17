#include "system.h"


void get_current_el(uint32_t *current_el)
{
	__asm__ __volatile__("mrs %0, CurrentEL\n\t" : "=r" (current_el) :  : "memory");
}

void get_cntv_ctl(uint32_t *cntv_ctl)
{
	__asm__ __volatile__("mrs %0, CNTV_CTL_EL0\n\t" : "=r" (cntv_ctl) :  : "memory");
}

void disable_cntv(void)
{
	uint32_t cntv_ctl;

	get_cntv_ctl(&cntv_ctl);
	cntv_ctl &= ~CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

void get_cntfrq(uint32_t *cntfrq_el0)
{
	__asm__ __volatile__("mrs %0, CNTFRQ_EL0\n\t" : "=r" (cntfrq_el0) : : "memory");
}

 void get_cntvct(uint64_t *cntvct_el0)
{
	__asm__ __volatile__("mrs %0, CNTVCT_EL0\n\t" : "=r" (cntvct_el0) : : "memory");
}

void enable_cntv(void)
{
	uint32_t cntv_ctl;

	get_cntv_ctl(&cntv_ctl);
	cntv_ctl |= CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

void put_cntv_cval(uint64_t cntv_cval_el0)
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
void get_daif(uint32_t *daif)
{
	__asm__ __volatile__("mrs %0, DAIF\n\t" : "=r" (daif) :  : "memory");
}