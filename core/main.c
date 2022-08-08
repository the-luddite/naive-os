#include "system.h"
#include "syscalls.h"
#include "process.h"


void core_main() 
{
    debug("entered core_main\n", DEBUG_TRACE);

	uart_init();

	init_printf(0, putc_uart);

	gic_v3_initialize();

    timer_init();

	enable_irq();

#ifdef TEST_SCHED
	test_sched_core_thread();
#endif
	
	while(1) {
		// wfi();	/* Wait for Interrupt */
		schedule();
	}

}
