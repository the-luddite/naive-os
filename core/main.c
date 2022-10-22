#include "aarch64/system.h"
#include "printf.h"
#include "debug.h"
#include "userspace/init.h"


static void call_userspace_init()
{
    int err = move_to_user_mode((u64)&call_init);
}

void userspace_init()
{
    int res = copy_process(PF_KTHREAD, (u64)&call_userspace_init, 0, 0);
	if (res < 0) {
		printf("error while starting kernel process");
		return;
	}
}

void core_main() 
{
    debug("entered core_main\n", DEBUG_TRACE);

	uart_init();

	init_printf(0, putc_uart);

	gic_v3_initialize();

    timer_init();

	enable_irq();

	userspace_init();

#ifdef TEST_SCHED
	test_sched_core_thread();
#endif
	
	while(1) {
		// wfi();	/* Wait for Interrupt */
		schedule();
	}

}
