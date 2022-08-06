#include "system.h"


void core_main() 
{
    debug("entered core_main\n", DEBUG_TRACE);

	uart_init();

	init_printf(0, putc_uart);

	gic_v3_initialize();

    timer_init();

#ifdef TEST_SCHED
	test_sched_core_thread();
#endif

	enable_irq();

	while(1) {
		// wfi();	/* Wait for Interrupt */
		schedule();
	}

}
