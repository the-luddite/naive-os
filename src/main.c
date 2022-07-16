#include "system.h"

#define TIMER_WAIT		1

// unsigned int *UART0DR = (unsigned int *) UARTDR;
// unsigned int *UART0FR = (unsigned int *) UARTFR;

static uint32_t cntfrq;	


void core_main() {
    print_uart("hello world\n");
    
    // __asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_IRQ_BIT)  : "memory");
    timer_test();
    // *(unsigned int *) UARTDR = '\t';
    // *(unsigned int *) UARTDR = '\n';
    // while(*s != '\0') { 
    //     *(unsigned int *) UARTDR = (unsigned int)(*s); 
    //     s++;
    // }
    // *(unsigned int *) UARTDR = '\t';
    // *(unsigned int *) UARTDR = '\n';
}

void timer_test(void)
{
	uint32_t val;
	uint64_t ticks, current_cnt;

    print_uart("timer_test\n");
	// GIC Init
	gic_v3_initialize();

    print_uart("CurrentEL = ");
	val = raw_read_current_el();
	print_uint(val);

    // print_uart("\nRVBAR_EL1 = ");
	// val = raw_read_rvbar_el1();
	// print_uint(val);

    // print_uart("\nVBAR_EL1 = ");
	// val = raw_read_vbar_el1();
	// print_uint(val);

    // print_uart("\nDAIF = ");
	// val = raw_read_daif();
	// print_uint(val);

	// // Disable the timer
	disable_cntv();
    print_uart("\nDisable the timer, CNTV_CTL_EL0 = ");
	val = raw_read_cntv_ctl();
	print_uint(val);
    print_uart("\nSystem Frequency: CNTFRQ_EL0 = ");
	cntfrq = raw_read_cntfrq_el0();
	print_uint(cntfrq);

	// Next timer IRQ is after n sec(s).
	ticks = TIMER_WAIT * cntfrq;
	// Get value of the current timer
	current_cnt = raw_read_cntvct_el0();
    print_uart("\nCurrent counter: CNTVCT_EL0 = ");
	print_uint(current_cnt);
	// Set the interrupt in Current Time + TimerTick
	raw_write_cntv_cval_el0(current_cnt + ticks);
    print_uart("\nAssert Timer IRQ after 1 sec: CNTV_CVAL_EL0 = ");
	// val = raw_read_cntv_cval_el0();
	// print_uint(val);

	// Enable the timer
	enable_cntv();
    print_uart("\nEnable the timer, CNTV_CTL_EL0 = ");
	val = raw_read_cntv_ctl();
	print_uint(val);

	// Enable IRQ 
	enable_irq();
    print_uart("\nEnable IRQ, DAIF = ");
	val = raw_read_daif();
	print_uint(val);
    print_uart("\n");

#if TIMER_DEBUG // Observe CNTP_CTL_EL0[2]: ISTATUS
	while(1){
		current_cnt = raw_read_cntvct_el0();
		val = raw_read_cntv_ctl();
		print_uart("\nCNTVCT_EL0 = ");
		print_uint(current_cnt);
		print_uart(", CNTV_CTL_EL0 = ");
		print_uint(val);
		val = raw_read_spsr_el1();
		print_uart(", SPSR_EL1 = ");
		print_uint(val);
		val = raw_read_isr_el1();
		print_uart(", ISR_EL1 = ");
		print_uint(val);
		print_uart("\n");
	}
#else
	while(1){
		wfi();	/* Wait for Interrupt */
	}
#endif
}
