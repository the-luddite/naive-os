#include "system.h"


void do_bad_sync_el1(void) 
{ 
    printf("do_bad_sync interrupt happened!\n");
    for(;;);
 }

void do_bad_irq_el1(void) 
{ 
    printf("do_bad_irq interrupt happened!\n");
    for(;;);
}

void do_bad_fiq_el1(void) 
{ 
    printf("do_bad_fiq interrupt happened!\n");
    for(;;);
}

void do_bad_error_el1(void) 
{ 
    printf("do_bad_error interrupt happened!\n");
    for(;;);
}

void do_sync_el1(void) 
{ 
    printf("do_sync interrupt happened!\n");
    for(;;);
}

void do_fiq_el1(void) 
{  
    printf("do_fiq interrupt happened!\n");
    for(;;);
}

void do_error_el1(void) 
{ 
    printf("do_error interrupt happened!\n");
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
    printf("do_bad_sync interrupt happened!\n");
    for(;;);
 }

void do_bad_irq_el0(void) 
{ 
    printf("do_bad_irq interrupt happened!\n");
    for(;;);
}

void do_bad_fiq_el0(void) 
{ 
    printf("do_bad_fiq interrupt happened!\n");
    for(;;);
}

void do_bad_error_el0(void) 
{ 
    printf("do_bad_error interrupt happened!\n");
    for(;;);
}

void do_sync_el0(void) 
{ 
    printf("do_sync interrupt happened!\n");
    for(;;);
}

void do_fiq_el0(void) 
{  
    printf("do_fiq interrupt happened!\n");
    for(;;);
}

void do_error_el0(void) 
{ 
    printf("do_error interrupt happened!\n");
    for(;;);
}

void do_irq_el0(void) 
{ 
    printf("do_irq_el0 interrupt happened!\n");
    for(;;);
}

void invalid_exception_context()
{
    printf("invalid_exception_context: looks like EL0 SYNC was called but no SVC was set, hang...");
    for(;;);
}

void invalid_syscall()
{
    printf("invalid syscall");
    for(;;);
}