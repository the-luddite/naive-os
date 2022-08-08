#include "exception.h"


void curr_el_sp0_sync(void) 
{ 
    printf("curr_el_sp0_sync interrupt happened!\n");
    for(;;);
 }

void curr_el_sp0_irq(void) 
{ 
    printf("curr_el_sp0_irq interrupt happened!\n");
    for(;;);
}

void curr_el_sp0_fiq(void) 
{ 
    printf("curr_el_sp0_fiq interrupt happened!\n");
    for(;;);
}

void curr_el_sp0_serror(void) 
{ 
    printf("curr_el_sp0_serror interrupt happened!\n");
    for(;;);
}

void curr_el_spx_sync(void) 
{ 
    printf("curr_el_spx_sync interrupt happened!\n");
    for(;;);
}

void curr_el_spx_fiq(void) 
{  
    printf("curr_el_spx_fiq interrupt happened!\n");
    for(;;);
}

void curr_el_spx_serror(void) 
{ 
    printf("curr_el_spx_serror interrupt happened!\n");
    for(;;);
}

void curr_el_spx_irq(void) 
{ 
    general_purpose_irq();
}


void lower_el_aarch32_sync(void) 
{ 
    printf("lower_el_aarch32_sync interrupt happened!\n");
    for(;;);
 }

void lower_el_aarch64_irq(void) 
{ 
    general_purpose_irq();
}

void lower_el_aarch64_fiq(void) 
{ 
    printf("lower_el_aarch64_fiq interrupt happened!\n");
    for(;;);
}

void lower_el_aarch64_serror(void) 
{ 
    printf("lower_el_aarch64_serror interrupt happened!\n");
    for(;;);
}

void lower_el_aarch64_sync()
{
    register void * syndrome asm("x25");
    asm ("" : "=r"(syndrome));

    if ((syndrome && INSTRUCTION_TRAPPED) == INSTRUCTION_TRAPPED)
    {
        printf("lower_el_aarch64_sync: 32-bit instruction trapped\n");
    } else {
        printf("lower_el_aarch64_sync: syndrome exception register isn't INSTRUCTION_TRAPPED: %u\n");
        for(;;);
    }   
}

void lower_el_aarch32_fiq(void) 
{  
    printf("lower_el_aarch32_fiq interrupt happened!\n");
    for(;;);
}

void lower_el_aarch32_serror(void) 
{ 
    printf("lower_el_aarch32_serror interrupt happened!\n");
    for(;;);
}

void lower_el_aarch32_irq(void) 
{ 
    printf("lower_el_aarch32_irq interrupt happened!\n");
    for(;;);
}

void invalid_syscall()
{
    printf("invalid syscall");
    for(;;);
}

void general_purpose_irq()
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