#include "system.h"


void __attribute__((interrupt)) do_bad_sync(void) 
{ 
    print_uart("do_bad_sync interrupt happened!\n");
 }
void __attribute__((interrupt)) do_bad_irq(void) 
{ 
    print_uart("do_bad_irq interrupt happened!\n");
}
void __attribute__((interrupt)) do_bad_fiq(void) 
{ 
    print_uart("do_bad_fiq interrupt happened!\n");
}

void __attribute__((interrupt)) do_bad_error(void) 
{ 
    print_uart("do_bad_error interrupt happened!\n");
}

void __attribute__((interrupt)) do_sync(void) 
{ 

    uint32_t buf[64];
    uint32_t c;

    c = lookup_pending_irqs(buf);
    print_uart("do_sync interrupt happened!\n");
    print_uart("sync irqs found: ");
    for (int i = 0; i < c; i++)
    {
        print_uart(buf[i]);
    }
    print_uart('\n');    
}

void __attribute__((interrupt)) do_fiq(void) 
{  
    print_uart("do_fiq interrupt happened!\n");
}

void __attribute__((interrupt)) do_error(void) 
{ 
    print_uart("do_error interrupt happened!\n");
}

void __attribute__((interrupt)) do_irq(void) 
{ 
    print_uart("do_irq interrupt happened!\n");
    if (gicd_probe_pending(TIMER_IRQ))
    {
        print_uart("and yes this is 27!\n");
        gicd_clear_pending(TIMER_IRQ);
    }
    if (gicd_probe_pending(UART_IRQ))
    {
        print_uart("and yes this is 33!\n");
        gicd_clear_pending(UART_IRQ);
    }
    
}