#include "system.h"

void process(char *array)
{
	while (1){
		for (volatile int i = 0; i < 5; i++){
			put_uart(array[i]);
			delay(10000000);
		}
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

	int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	if (res) {
		printf("error while starting process 1\n");
		return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"abcde");
	if (res) {
		printf("error while starting process 2\n");
		return;
	}

	while(1) {
		// wfi();	/* Wait for Interrupt */
		schedule();
	}

}
