#include "system.h"


void do_bad_sync_el1(void) 
{ 
    print_uart("do_bad_sync interrupt happened!\n");
    for(;;);
 }

void do_bad_irq_el1(void) 
{ 
    print_uart("do_bad_irq interrupt happened!\n");
    for(;;);
}

void do_bad_fiq_el1(void) 
{ 
    print_uart("do_bad_fiq interrupt happened!\n");
    for(;;);
}

void do_bad_error_el1(void) 
{ 
    print_uart("do_bad_error interrupt happened!\n");
    for(;;);
}

void do_sync_el1(void) 
{ 
    print_uart("do_sync interrupt happened!\n");
    for(;;);
}

void do_fiq_el1(void) 
{  
    print_uart("do_fiq interrupt happened!\n");
    for(;;);
}

void do_error_el1(void) 
{ 
    print_uart("do_error interrupt happened!\n");
    for(;;);
}

void do_irq_el1(void) 
{ 
    irq_no irq;

    disable_irq();
    
    if (pending_irq(&irq))
    {
        // this clear must be here, otherwise the function will be called many times in scheduler
        gicd_clear_pending(irq);
        switch (irq)
        {
        case TIMER_IRQ:
            debug("timer irq just triggered (27)\n", DEBUG_ALL);
            timer_irq_handler();
            break;
        
        case UART_IRQ:
            debug("uart irq just triggered (33)\n", DEBUG_ALL);
            uart_disable_irq();
            // print_uint(UART0->DR);
            uart_irq_handler();
            uart_enable_irq();
            break;
        default:
            debug("hm, there something else...   ", DEBUG_ALL);
            print_uint(irq);
            break;
        }
    } else {
        debug("ERROR: that's weird, we've received irq but pending_irq returned nothing\n", DEBUG_ALL);
    }
    
    enable_irq();
}


void do_bad_sync_el0(void) 
{ 
    print_uart("do_bad_sync interrupt happened!\n");
    for(;;);
 }

void do_bad_irq_el0(void) 
{ 
    print_uart("do_bad_irq interrupt happened!\n");
    for(;;);
}

void do_bad_fiq_el0(void) 
{ 
    print_uart("do_bad_fiq interrupt happened!\n");
    for(;;);
}

void do_bad_error_el0(void) 
{ 
    print_uart("do_bad_error interrupt happened!\n");
    for(;;);
}

void do_sync_el0(void) 
{ 
    print_uart("do_sync interrupt happened!\n");
    for(;;);
}

void do_fiq_el0(void) 
{  
    print_uart("do_fiq interrupt happened!\n");
    for(;;);
}

void do_error_el0(void) 
{ 
    print_uart("do_error interrupt happened!\n");
    for(;;);
}

void do_irq_el0(void) 
{ 
    print_uart("do_irq_el0 interrupt happened!\n");
    for(;;);
}