#include "system.h"


void core_main() {

    print_uart(UART0, "We have started\n");
    current_state = initializing;

    VIC_INTENABLE = 1<<12;
    UART0->IMSC = 1<<4;

    current_state = running;
    
    timer_init();

    print_uart(UART0, "Initialized\n");
    for(;;);
}

