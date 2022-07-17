#include "system.h"



void core_main() 
{
    print_uart("hello world\n");

	uart_init();

	gic_v3_initialize();

    disable_cntv();

	enable_irq();

	while(1){
		wfi();	/* Wait for Interrupt */
	}

}
