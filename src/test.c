#include "system.h"


// volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
// volatile unsigned int * const SYS_FLAGS = (unsigned int *)0x10000030;
// volatile unsigned int * const SYS_100HZ = (unsigned int *)0x10000024;



 
void core_main() {
    print_uart(UART0, "We have started\n");
    current_state = initializing;

}

