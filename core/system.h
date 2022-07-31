#pragma once

#include "uart.h"
#include "stdint.h"
#include "gic_v3.h"
#include "debug.h"
#include "scheduler.h"
#include "printf.h"


typedef int32_t irq_no;


#define wfi()		asm volatile("wfi" : : : "memory")


/* DAIF, Interrupt Mask Bits */
#define DAIF_DBG_BIT		(1<<3)	/* Debug mask bit */
#define DAIF_ABT_BIT		(1<<2)	/* Asynchronous abort mask bit */
#define DAIF_IRQ_BIT		(1<<1)	/* IRQ mask bit */
#define DAIF_FIQ_BIT		(1<<0)	/* FIQ mask bit */


/* Timer */  
#define CNTV_CTL_ENABLE		(1 << 0)	/* Enables the timer */	
#define CNTV_CTL_IMASK		(1 << 1)	/* Timer interrupt mask bit */
#define CNTV_CTL_ISTATUS	(1 << 2)	/* The status of the timer interrupt. This bit is read-only */

extern void delay ( unsigned long);