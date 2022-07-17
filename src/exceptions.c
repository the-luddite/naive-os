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


static uint32_t cntfrq;	
void __attribute__((interrupt)) do_irq(void) 
{ 
    irq_no irq;
    extern pl011_T * const UART0;
    uint64_t ticks, current_cnt;

    // print_uart("do_irq interrupt happened!\n");
    
    if (pending_irq(&irq))
    {
        gicd_disable_int(irq);
        switch (irq)
        {
        case TIMER_IRQ:
            print_uart("and yes this is 27!\n");
            cntfrq = get_cntfrq();

            // Next timer IRQ is after n sec(s).
            ticks = cntfrq;
            // Get value of the current timer
            current_cnt = get_cntvct();
            // Set the interrupt in Current Time + TimerTick
            put_cntv_cval(current_cnt + ticks);
            goto clear;
            break;
        
        case UART_IRQ:
            // print_uart("and yes this is 33!\n");
            uart_disable_irq();

            // print_uint(UART0->DR);
            handle_uart_irq();

            uart_enable_irq();
            goto clear;
            break;
        default:
            print_uart("hm, there something else...   ");
            print_uint(irq);
            goto clear;
            break;
        }
    } else {
        print_uart("ERROR: that's weird, we've received irq but pending_irq returned nothing\n");
    }
clear:
    gicd_clear_pending(irq);
    gicd_enable_int(irq);
}