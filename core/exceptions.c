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
    print_uart("do_sync interrupt happened!\n");
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
    irq_no irq;
    extern pl011_T * const UART0;

    // print_uart("do_irq interrupt happened!\n");
    
    if (pending_irq(&irq))
    {
        gicd_disable_int(irq);
        switch (irq)
        {
        case TIMER_IRQ:
            debug("timer irq just triggered (27)\n", DEBUG_ALL);
            timer_irq_handler();
            goto clear;
            break;
        
        case UART_IRQ:
            debug("uart irq just triggered (33)\n", DEBUG_ALL);
            uart_disable_irq();
            // print_uint(UART0->DR);
            uart_irq_handler();
            uart_enable_irq();
            goto clear;
            break;
        default:
            debug("hm, there something else...   ", DEBUG_ALL);
            print_uint(irq);
            goto clear;
            break;
        }
    } else {
        debug("ERROR: that's weird, we've received irq but pending_irq returned nothing\n", DEBUG_ALL);
    }
clear:
    gicd_clear_pending(irq);
    gicd_enable_int(irq);
}