/* -*- mode: c; coding:utf-8 -*- */
/**********************************************************************/
/*  OS kernel sample                                                  */
/*  Copyright 2014 Takeharu KATO                                      */
/*                                                                    */
/*  Arm Generic Interrupt Controller(PL390)                           */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*  armv8-bare-metal                                                  */
/*  Copyright 2018 Nienfeng Yao                                       */
/*                                                                    */
/*  Reference: ARM® Generic Interrupt Controller Architecture         */
/*      Specification GIC architecture version 3.0 and version 4.0    */
/*                                                                    */
/**********************************************************************/
#include "gic_v3.h"


#define IRQ_FOUND          (0)
#define IRQ_NOT_FOUND      (1)



/* Initialize GIC Controller */
static void init_gicc(void)
{
	u32 pending_irq;

    print_uart("init_gicc()\n");
	/* Disable CPU interface */
	*REG_GIC_GICC_CTLR = GICC_CTLR_DISABLE;

	/* Set the priority level as the lowest priority */
	/* Note: Higher priority corresponds to a lower Priority field value in the GIC_PMR.
	 * In addition to this, writing 255 to the GICC_PMR always sets it to the 
	 * largest supported priority field value.
	 */
	*REG_GIC_GICC_PMR = GICC_PMR_PRIO_MIN;

	/* Handle all of interrupts in a single group */
	*REG_GIC_GICC_BPR = GICC_BPR_NO_GROUP;

	/* Clear all of the active interrupts */
	for(pending_irq = ( *REG_GIC_GICC_IAR & GICC_IAR_INTR_IDMASK ); 
	    ( pending_irq != GICC_IAR_SPURIOUS_INTR );
	    pending_irq = ( *REG_GIC_GICC_IAR & GICC_IAR_INTR_IDMASK ) )
		*REG_GIC_GICC_EOIR = *REG_GIC_GICC_IAR;

	/* Enable CPU interface */
	*REG_GIC_GICC_CTLR = GICC_CTLR_ENABLE;
}

static void init_gicd(void)
{
	int32_t	i, regs_nr;

    print_uart("init_gicd()\n");
	/* Diable distributor */
	*REG_GIC_GICD_CTLR = GIC_GICD_CTLR_DISABLE;

	/* Disable all IRQs */
	regs_nr = (GIC_INT_MAX + GIC_GICD_INT_PER_REG - 1) / GIC_GICD_INT_PER_REG;
	for (i = 0; regs_nr > i; ++i)
		*REG_GIC_GICD_ICENABLER(i) = ~((u32)(0)); 

	/* Clear all pending IRQs */
	regs_nr = (GIC_INT_MAX + GIC_GICD_INT_PER_REG - 1) / GIC_GICD_INT_PER_REG;
	for (i = 0; regs_nr > i; ++i) 
		*REG_GIC_GICD_ICPENDR(i) = ~((u32)(0));

	/* Set all of interrupt priorities as the lowest priority */
	regs_nr = ( GIC_INT_MAX + GIC_GICD_IPRIORITY_PER_REG - 1) / 
		GIC_GICD_IPRIORITY_PER_REG ;
	for (i = 0; regs_nr > i; i++)
		*REG_GIC_GICD_IPRIORITYR(i) = ~((u32)(0));

	/* Set target of all of shared peripherals to processor 0 */
	for (i = GIC_INTNO_SPI0 / GIC_GICD_ITARGETSR_PER_REG;
	     ( (GIC_INT_MAX + (GIC_GICD_ITARGETSR_PER_REG - 1) ) / 
		 GIC_GICD_ITARGETSR_PER_REG ) > i; ++i) 
		*REG_GIC_GICD_ITARGETSR(i) = 
			(u32)GIC_GICD_ITARGETSR_CORE0_TARGET_BMAP;

	/* Set trigger type for all peripheral interrupts level triggered */
	for (i = GIC_INTNO_PPI0 / GIC_GICD_ICFGR_PER_REG;
	     (GIC_INT_MAX + (GIC_GICD_ICFGR_PER_REG - 1)) / GIC_GICD_ICFGR_PER_REG > i; ++i)
		*REG_GIC_GICD_ICFGR(i) = GIC_GICD_ICFGR_LEVEL;

	/* Enable distributor */
	*REG_GIC_GICD_CTLR = GIC_GICD_CTLR_ENABLE;
}

/** Disable IRQ
    @param[in] irq IRQ number
 */
void gicd_disable_int(irq_no irq) {
	*REG_GIC_GICD_ICENABLER( (irq / GIC_GICD_ICENABLER_PER_REG) ) = 
		1U << ( irq % GIC_GICD_ICENABLER_PER_REG );
}

/** Enable IRQ
    @param[in] irq IRQ number
 */
void gicd_enable_int(irq_no irq) {

	*REG_GIC_GICD_ISENABLER( (irq / GIC_GICD_ISENABLER_PER_REG) ) =
		1U << ( irq % GIC_GICD_ISENABLER_PER_REG );

}

/** Clear a pending interrupt
    @param[in] irq IRQ number
 */
void gicd_clear_pending(irq_no irq) {

	*REG_GIC_GICD_ICPENDR( (irq / GIC_GICD_ICPENDR_PER_REG) ) = 
		1U << ( irq % GIC_GICD_ICPENDR_PER_REG );
}

/** Set an interrupt target processor
    @param[in] irq IRQ number
    @param[in] p   Target processor mask
    0x1 processor 0
    0x2 processor 1
    0x4 processor 2
    0x8 processor 3
 */
static void gicd_set_target(irq_no irq, u32 p)
{
	u32  shift;
	u32    reg;

	shift = (irq % GIC_GICD_ITARGETSR_PER_REG) * GIC_GICD_ITARGETSR_SIZE_PER_REG;

	reg = *REG_GIC_GICD_ITARGETSR(irq / GIC_GICD_ITARGETSR_PER_REG);
	reg &= ~( ((u32)(0xff)) << shift);
	reg |= (p << shift);
	*REG_GIC_GICD_ITARGETSR(irq / GIC_GICD_ITARGETSR_PER_REG) = reg;
}

/** Set an interrupt priority
    @param[in] irq  IRQ number
    @param[in] prio Interrupt priority in Arm specific expression
 */
static void gicd_set_priority(irq_no irq, u32 prio)
{
	u32  shift;
	u32    reg;

	shift = (irq % GIC_GICD_IPRIORITY_PER_REG) * GIC_GICD_IPRIORITY_SIZE_PER_REG;
	reg = *REG_GIC_GICD_IPRIORITYR(irq / GIC_GICD_IPRIORITY_PER_REG);
	reg &= ~(((u32)(0xff)) << shift);
	reg |= (prio << shift);
	*REG_GIC_GICD_IPRIORITYR(irq / GIC_GICD_IPRIORITY_PER_REG) = reg;
}

/** Configure IRQ 
    @param[in] irq     IRQ number
    @param[in] config  Configuration value for GICD_ICFGR
 */
static void gicd_config(irq_no irq, unsigned int config)
{
	u32	shift; 
	u32	  reg;

	shift = (irq % GIC_GICD_ICFGR_PER_REG) * GIC_GICD_ICFGR_SIZE_PER_REG; /* GICD_ICFGR has 16 fields, each field has 2bits. */

	reg = *REG_GIC_GICD_ICFGR( irq / GIC_GICD_ICFGR_PER_REG);

	reg &= ~( ( (u32)(0x03) ) << shift );  /* Clear the field */
	reg |= ( ( (u32)config ) << shift );  /* Set the value to the field correponding to irq */
	*REG_GIC_GICD_ICFGR( irq / GIC_GICD_ICFGR_PER_REG) = reg;
}

/** Send End of Interrupt to IRQ line for GIC
    @param[in] ctrlr   IRQ controller information
    @param[in] irq     IRQ number
 */
void gic_v3_eoi(irq_no irq) {
	gicd_clear_pending(irq);
}

/* Initialize GIC IRQ controller */
/* RyanYao: 2018/07/20
 *    I supppose the current access is security, because GICD_CTLR.DS is 0b0 and
 *    we can access.
 */ 
void gic_v3_initialize(void)
{

    print_uart("gic_v3_initialize()\n");
	init_gicd();
	init_gicc();
	gicd_config(TIMER_IRQ, GIC_GICD_ICFGR_EDGE);
	gicd_set_priority(TIMER_IRQ, 0 << GIC_PRI_SHIFT );  /* Set priority */
	gicd_set_target(TIMER_IRQ, 0x1);  /* processor 0 */
	gicd_clear_pending(TIMER_IRQ);
	gicd_enable_int(TIMER_IRQ);

	gicd_config(UART_IRQ, GIC_GICD_ICFGR_EDGE);
	gicd_set_priority(UART_IRQ, 0 << GIC_PRI_SHIFT );  /* Set priority */
	gicd_set_target(UART_IRQ, 0x1);  /* processor 0 */
	gicd_clear_pending(UART_IRQ);
	gicd_enable_int(UART_IRQ);

	// for (u32 i = 0; i < 64; i++)
	// {
	// 	gicd_config(i, GIC_GICD_ICFGR_EDGE);
	// 	gicd_set_priority(i, 0 << GIC_PRI_SHIFT );  /* Set priority */
	// 	gicd_set_target(i, 0x1);  /* processor 0 */
	// 	gicd_clear_pending(i);
	// 	gicd_enable_int(i);
	// }
}

/** Probe pending interrupt
    @param[in] irq IRQ number
 */
int gicd_probe_pending(irq_no irq) 
{
	return (0 != ( *REG_GIC_GICD_ISPENDR( (irq / GIC_GICD_ISPENDR_PER_REG) ) &
	    ( 1U << ( irq % GIC_GICD_ISPENDR_PER_REG ))));
}