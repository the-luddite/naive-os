#include "system.h"


void __attribute__((interrupt)) do_bad_sync(void) { for(;;); }
void __attribute__((interrupt)) do_bad_irq(void) { for(;;); }
void __attribute__((interrupt)) do_bad_fiq(void) { for(;;); }
void __attribute__((interrupt)) do_bad_error(void) { for(;;); }
void __attribute__((interrupt)) do_sync(void) { for(;;); }

void __attribute__((interrupt)) do_fiq(void) { for(;;); }
void __attribute__((interrupt)) do_error(void) { for(;;); }


void __attribute__((interrupt)) do_irq(void) { 
    print_uart("irq interrupt happened!\n");

}